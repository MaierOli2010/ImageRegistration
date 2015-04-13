#include "myregistration.h"

#include "imageregistration.h"
#include "myimageclass.h"

#include <string>

#include <QVTKWidget.h>
#include <itkImageSeriesWriter.h>


#include <itkCastImageFilter.h>

#include "registrationobserver.h"
#include "ui_registrationobserver.h"

MyRegistration::MyRegistration(ImageRegistration *myimreg, MyImageClass* fixed_image,
                               std::unique_ptr<MyImageClass>* moving_images):
    imreg_(myimreg),
    fixed_image_(fixed_image),
    moving_images_(moving_images),
    registration_(RegistrationType::New())
{
    imageViewerDCMSeriesX_result_ = vtkSmartPointer<vtkImageViewer2>::New();

    metric_ = MetricType::New();
    transform_ = TransformType::New();
    optimizer_ = OptimizerType::New();
    interpolator_ = InterpolatorType::New();
    //registration_ = RegistrationType::New();
    resampler_ = ResampleFilterType::New();
    difference_ = DifferenceFilterType::New();
    intensity_rescaler_ = RescalerType::New();
    observer_ = MyRegistrationObserver::New();
    fixedImageCaster_ = ImageCasterType::New();
    movingImageCaster_ = ImageCasterType::New();
    matcher_ = MatchingFilterType::New();
    observer_->setObserverWindow(myimreg->GetObserverWindow());
    observer_->setFixedImage(fixed_image_);
    observer_->setMovingImage(moving_images_);
    transformInitializer_ = InitializerType::New();
    gdcmIO_ = ImageIOType::New();
    namesGenerator_ = NamesGeneratorType::New();
}

MyRegistration::~MyRegistration()
{
}
void MyRegistration::SaveDICOMSeries(QString save_path)
{
    std::string output_directory = save_path.toStdString();

    itksys::SystemTools::MakeDirectory( output_directory.c_str() );

    typedef signed short OutputPixelType;
    const unsigned int output_pixel_dimension = 2;

    typedef itk::Image < OutputPixelType, output_pixel_dimension > Image2Dtype;
    typedef itk::CastImageFilter<InternalImageType, ImageType> CastFilterType;

    typedef itk::ImageSeriesWriter < ImageType, Image2Dtype> SeriesWriterType;


    CastFilterType::Pointer caster = CastFilterType::New();
    SeriesWriterType::Pointer series_writer = SeriesWriterType::New();

    caster->SetInput(resampler_->GetOutput());
    series_writer->SetInput( caster->GetOutput());
    series_writer->SetImageIO(gdcmIO_);class MyRegistrationObserver;

    namesGenerator_->SetOutputDirectory( output_directory );
    series_writer->SetFileNames (namesGenerator_->GetOutputFileNames());

    series_writer->SetMetaDataDictionaryArray(fixed_image_->GetReader()->GetMetaDataDictionaryArray());

    try
    {
        series_writer->Update();
    }
    catch( itk::ExceptionObject &excp)
    {
        msg_box_.setText(excp.GetDescription());
        msg_box_.exec();
    }

}



void MyRegistration::StartRegistration()
{
    registration_->SetMetric(metric_);
    registration_->SetOptimizer(optimizer_);
    registration_->SetTransform(transform_);
    registration_->SetInterpolator(interpolator_);

    TransformType::PhysicalDimensionsType    fixedPhysicalDimensions;
    TransformType::MeshSizeType    meshSize;
    TransformType::OriginType    fixedOrigin;

    //Default Nodes = 8
    unsigned int numberOfGridNodesInOneDimension = 4;


    for( unsigned int i=0; i< SpaceDimension; i++ )
    {
        fixedOrigin[i] = fixed_image_->GetReader()->GetOutput()->GetOrigin()[i];
        fixedPhysicalDimensions[i] = fixed_image_->GetReader()->GetOutput()->GetSpacing()[i] *
        static_cast<double>(
        fixed_image_->GetReader()->GetOutput()->GetLargestPossibleRegion().GetSize()[i] - 1 );
    }


    meshSize.Fill( numberOfGridNodesInOneDimension - SplineOrder );

    transformInitializer_->SetTransform( transform_ );
    transformInitializer_->SetImage( fixed_image_->GetReader()->GetOutput() );
    transformInitializer_->SetTransformDomainMeshSize( meshSize );
    transformInitializer_->InitializeTransform();

    transform_->SetTransformDomainOrigin( fixedOrigin );
    transform_->SetTransformDomainPhysicalDimensions( fixedPhysicalDimensions );
    transform_->SetTransformDomainMeshSize( meshSize );
    transform_->SetTransformDomainDirection( fixed_image_->GetReader()->GetOutput()->GetDirection() );




    //Intensity Rescaling and casting of Images to internal Image Type (float)
    fixedImageCaster_->SetInput(fixed_image_->GetReader()->GetOutput());
    movingImageCaster_->SetInput((*moving_images_)->GetReader()->GetOutput());

    matcher_->SetInput(movingImageCaster_->GetOutput());
    matcher_->SetReferenceImage(fixedImageCaster_->GetOutput());
    matcher_->SetNumberOfHistogramLevels(1024);
    matcher_->SetNumberOfMatchPoints(7);
    matcher_->ThresholdAtMeanIntensityOn();

    registration_->SetFixedImage(fixedImageCaster_->GetOutput());
    registration_->SetMovingImage(matcher_->GetOutput());
    registration_->SetFixedImageRegion(fixed_image_->GetReader()->GetOutput()->GetBufferedRegion());

    typedef RegistrationType::ParametersType ParametersType;

    const unsigned int numberOfParameters = transform_->GetNumberOfParameters();

    ParametersType parameters( numberOfParameters );
    parameters.Fill( 0.0 );

    transform_->SetParameters( parameters );
    registration_->SetInitialTransformParameters( transform_->GetParameters() );
    //registration_->InPlaceOn();

//    const unsigned int numberOfLevels = 3;

//    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
//    shrinkFactorsPerLevel.SetSize( numberOfLevels );
//    shrinkFactorsPerLevel[0] = 3;
//    shrinkFactorsPerLevel[1] = 2;
//    shrinkFactorsPerLevel[2] = 1;

//    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
//    smoothingSigmasPerLevel.SetSize( numberOfLevels );
//    smoothingSigmasPerLevel[0] = 2;
//    smoothingSigmasPerLevel[1] = 1;
//    smoothingSigmasPerLevel[2] = 0;

//    registration_->SetNumberOfLevels( numberOfLevels );
//    registration_->SetSmoothingSigmasLevel (smoothingSigmasPerLevel);
//    registration_->SetShrinkFactorsPerLevel(shrinkFactorsperLevel);

//    RegistrationType::TransformParametersAdaptorsContainerType adaptors;



    //Parameters for translation registration
    //ParametersType initial_parameters( transform_->GetNumberOfParameters());
    //initial_parameters[0] = 0.0;
    //initial_parameters[1] = 0.0;

    //registration_->SetInitialTransformParameters (initial_parameters);

    //uncomment for single resolution
    //registration_->SetInitialTransformParameters(transform_->GetParameters());

    //Optimizer for rigid transform
    //optimizer_->SetMaximumStepLength(2.00);
    //optimizer_->SetMinimumStepLength(0.01);
    //optimizer_->SetNumberOfIterations(200);
    //optimizer_->AddObserver( itk::IterationEvent(), observer_);

    //optimizer_->SetGradientConvergenceTolerance( 0.05 );
    //optimizer_->SetLineSearchAccuracy( 0.9 );
    //optimizer_->SetDefaultStepLength( 1.5 );
    //optimizer_->TraceOn();
    //optimizer_->SetMaximumNumberOfFunctionEvaluations( 1000 );

    OptimizerType::BoundSelectionType boundSelect( transform_->GetNumberOfParameters() );
    OptimizerType::BoundValueType upperBound( transform_->GetNumberOfParameters() );
    OptimizerType::BoundValueType lowerBound( transform_->GetNumberOfParameters() );

    boundSelect.Fill( 0 );
    upperBound.Fill( 0.0 );
    lowerBound.Fill( 0.0 );

    optimizer_->SetBoundSelection( boundSelect );
    optimizer_->SetUpperBound( upperBound );
    optimizer_->SetLowerBound( lowerBound );
    optimizer_->SetCostFunctionConvergenceFactor( 1.e2 ); //1e7
    optimizer_->SetProjectedGradientTolerance( 1.e-1 ); //1e-6
    optimizer_->SetMaximumNumberOfIterations( 500 );
    optimizer_->SetMaximumNumberOfEvaluations( 500 );
    optimizer_->SetMaximumNumberOfCorrections( 3 ); //should be between 3 and 20
    optimizer_->AddObserver( itk::IterationEvent(), observer_);
    observer_->setRegistration(registration_);
    //registration_->AddObserver( itk::AnyEvent(), observer_);
    try
    {
        registration_->Update();
    }
    catch(itk::ExceptionObject &err)
    {
        throw err;
    }

    ParametersType final_parameters = registration_->GetLastTransformParameters();

    //const double x_trans = final_parameters[0];
    //const double y_trans = final_parameters[1];
    //const unsigned int number_of_it = optimizer_->GetCurrentIteration();
    //const double best_value = optimizer_->GetValue();

    resampler_->SetInput((*moving_images_)->GetReader()->GetOutput());
    resampler_->SetTransform(registration_->GetOutput()->Get());
    resampler_->SetSize(fixed_image_->GetReader()->GetOutput()->GetLargestPossibleRegion().GetSize());
    resampler_->SetOutputOrigin(fixed_image_->GetReader()->GetOutput()->GetOrigin());
    resampler_->SetOutputSpacing(fixed_image_->GetReader()->GetOutput()->GetSpacing());
    resampler_->SetOutputDirection(fixed_image_->GetReader()->GetOutput()->GetDirection());
    resampler_->SetDefaultPixelValue(100);

}

void MyRegistration::ShowResultingFit()
{
    try
    {
    ComputeDifference();
    connector_result_ = FilterType::New();

    connector_result_->SetInput(intensity_rescaler_->GetOutput());
    try
    {
      connector_result_->Update();
    }
    catch(itk::ExceptionObject &err)
    {
        throw err;
    }

    imageViewerDCMSeriesX_result_->SetInputData(connector_result_->GetOutput());

    mMinSliderX_result_ = imageViewerDCMSeriesX_result_->GetSliceMin();
    mMaxSliderX_result_ = imageViewerDCMSeriesX_result_->GetSliceMax();

    imreg_->hSliderResult->setMinimum(mMinSliderX_result_);
    imreg_->hSliderResult->setMaximum(mMaxSliderX_result_);


    imageViewerDCMSeriesX_result_->SetRenderWindow(imreg_->vtkRendererResult->GetRenderWindow());
    imageViewerDCMSeriesX_result_->Render();
    }
    catch(itk::ExceptionObject &err)
    {
        msg_box_.setText(err.GetDescription());
        msg_box_.exec();
    }
}
void MyRegistration::SetSlicePositionResult(int position)
{
    imageViewerDCMSeriesX_result_->SetSlice(position);
    imageViewerDCMSeriesX_result_->Render();
}

void MyRegistration::ComputeDifference()
{
    difference_->SetInput1(fixed_image_->GetReader()->GetOutput());
    difference_->SetInput2(resampler_->GetOutput());


    intensity_rescaler_->SetInput(difference_->GetOutput());
    intensity_rescaler_->SetOutputMinimum(0);
    intensity_rescaler_->SetOutputMaximum(255);
    resampler_->SetDefaultPixelValue(1);
}

void MyRegistration::SetSlicePositionObserver(int position)
{
    observer_->SetSlicePositionObserver(position);
}
