#include "myregistration.h"

#include "imageregistration.h"
#include "myimageclass.h"

#include <string>

#include <QVTKWidget.h>
#include <itkImageSeriesWriter.h>


#include <itkCastImageFilter.h>
#include <itkMetaDataObject.h>

#include "registrationobserver.h"
#include "ui_registrationobserver.h"

MyRegistration::MyRegistration(ImageRegistration *myimreg, MyImageClass* fixed_image,
                               std::unique_ptr<MyImageClass>* moving_images):
    imreg_(myimreg),
    fixed_image_(fixed_image),
    moving_images_(moving_images)
{
    imageViewerDCMSeriesX_result_ = vtkSmartPointer<vtkImageViewer2>::New();

    metric_ = MetricType::New();
    transform_ = TransformType::New();
    optimizer_ = OptimizerType::New();
    interpolator_ = InterpolatorType::New();
    registration_ = RegistrationType::New();
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
    Ttransform_ = TransformType_1st::New();
    Toptimizer_ = OptimizerType_1st::New();
    compositeTransform_ = CompositeTransformType::New();
    Tregistration_ = TRegistrationType::New();
    Tmetric_ = MetricType::New();
}

MyRegistration::~MyRegistration()
{
}
void MyRegistration::SaveDICOMSeries(QString save_path, std::string subfolder)
{
    std::string output_directory = save_path.toStdString();
    output_directory +=subfolder;

    itksys::SystemTools::MakeDirectory( output_directory.c_str() );

    typedef signed short OutputPixelType;
    const unsigned int output_pixel_dimension = 2;

    typedef itk::Image < OutputPixelType, output_pixel_dimension > Image2Dtype;
    typedef itk::CastImageFilter<InternalImageType, ImageType> CastFilterType;

    typedef itk::ImageSeriesWriter < ImageType, Image2Dtype> SeriesWriterType;


    CastFilterType::Pointer caster = CastFilterType::New();
    SeriesWriterType::Pointer series_writer = SeriesWriterType::New();

    typedef itk::NumericSeriesFileNames NumericNamesGeneratorType;

    NumericNamesGeneratorType::Pointer namesGenerator = NumericNamesGeneratorType::New();

    itk::MetaDataDictionary &dict = gdcmIO_->GetMetaDataDictionary();
    std::string tagkey,value;
    tagkey = "0008|0060";
    value = "MR";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);
    tagkey = "0008|0008";
    value = "DERIVED\\SECONDARY";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);
    tagkey = "0008|0064";
    value = "DV";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);

    caster->SetInput(resampler_->GetOutput());
    series_writer->SetInput( caster->GetOutput());
    series_writer->SetImageIO(gdcmIO_);

    ImageType::RegionType region = fixed_image_->GetReader()->GetOutput()->GetLargestPossibleRegion();
    ImageType::IndexType start = region.GetIndex();
    ImageType::SizeType size = region.GetSize();

    std::string format = output_directory;
    format += "/image%03d.dcm";

    namesGenerator->SetSeriesFormat(format.c_str());

    namesGenerator->SetStartIndex(start[2]);
    namesGenerator->SetEndIndex(start[2]+size[2]-1);
    namesGenerator->SetIncrementIndex(1);

    series_writer->SetFileNames (namesGenerator->GetFileNames());

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
    //Intensity Rescaling and casting of Images to internal Image Type (float)
    fixedImageCaster_->SetInput(fixed_image_->GetReader()->GetOutput());
    movingImageCaster_->SetInput((*moving_images_)->GetReader()->GetOutput());
    matcher_->SetInput(movingImageCaster_->GetOutput());
    matcher_->SetReferenceImage(fixedImageCaster_->GetOutput());
    matcher_->SetNumberOfHistogramLevels(128);
    matcher_->SetNumberOfMatchPoints(7);
    matcher_->ThresholdAtMeanIntensityOn();


    Tregistration_->SetFixedImage(fixed_image_->GetReader()->GetOutput());
    Tregistration_->SetMovingImage(matcher_->GetOutput());

    Tmetric_->SetNumberOfHistogramBins(20);
    Tmetric_->SetUseMovingImageGradientFilter(false);
    Tmetric_->SetUseFixedImageGradientFilter(false);
    Tmetric_->SetUseFixedSampledPointSet(false);

    typedef OptimizerType_1st::ParametersType TParametersType;
    TParametersType TinitialParameters(Ttransform_->GetNumberOfParameters());

    TinitialParameters[0] = 0;
    TinitialParameters[1] = 0;
    TinitialParameters[2] = 0;

    Ttransform_->SetParameters(TinitialParameters);

    Tregistration_->SetMovingInitialTransform(Ttransform_);

    compositeTransform_->AddTransform(Ttransform_);

    const unsigned int numberOfLevels = 1;

    TRegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel1;
    shrinkFactorsPerLevel1.SetSize(numberOfLevels);
    shrinkFactorsPerLevel1[0] = 3;

    TRegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel1;
    smoothingSigmasPerLevel1.SetSize( numberOfLevels);
    smoothingSigmasPerLevel1[0] = 2;

    Tregistration_->SetNumberOfLevels(numberOfLevels);
    Tregistration_->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel1);
    Tregistration_->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel1);


    Toptimizer_->SetNumberOfIterations(200);
    Toptimizer_->SetRelaxationFactor(0.5);
    Toptimizer_->SetLearningRate(16);
    Toptimizer_->SetMinimumStepLength(1.5);

    Toptimizer_->AddObserver( itk::IterationEvent(), observer_);
    observer_->setRegistration(Tregistration_);

    Tregistration_->SetOptimizer(Toptimizer_);
    Tregistration_->SetMetric(Tmetric_);

    try
    {
        Tregistration_->Update();
    }
    catch(itk::ExceptionObject &err)
    {
        throw err;
    }
    compositeTransform_->AddTransform(Tregistration_->GetModifiableTransform());

    registration_->SetMovingInitialTransform(compositeTransform_);

    RegistrationType::MetricSamplingStrategyType samplingStrategy = RegistrationType::RANDOM;
    double samplingPercentage = 0.01;


//    metric_->SetUseMovingImageGradientFilter(false);
//    metric_->SetUseFixedImageGradientFilter(false);
//    metric_->SetUseFixedSampledPointSet(false);
    metric_->SetNumberOfHistogramBins(64);


    registration_->SetMetric(metric_);
    registration_->SetMetricSamplingStrategy(samplingStrategy);
    registration_->SetMetricSamplingPercentage(samplingPercentage);
    registration_->SetOptimizer(optimizer_);
    //registration_->SetTransform(transform_);
    //registration_->SetInterpolator(interpolator_);

    TransformType::PhysicalDimensionsType    fixedPhysicalDimensions;
    TransformType::MeshSizeType    meshSize;
    TransformType::OriginType    fixedOrigin;

    //Default Nodes = 8
    unsigned int numberOfGridNodesInOneDimension = 8;


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




    registration_->SetFixedImage(fixed_image_->GetReader()->GetOutput());
    registration_->SetMovingImage(matcher_->GetOutput());
  
    typedef TransformType::ParametersType ParametersType;

    const unsigned int numberOfParameters = transform_->GetNumberOfParameters();

    ParametersType parameters( numberOfParameters );
    parameters.Fill( 0.0 );

    registration_->SetInitialTransform(transform_);


    const unsigned int numberOfLevels2 = 1;

    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel2;
    shrinkFactorsPerLevel2.SetSize( numberOfLevels2 );
    shrinkFactorsPerLevel2[0] = 1;
    shrinkFactorsPerLevel2[1] = 2;
   shrinkFactorsPerLevel2[2] = 3;

    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel2;
    smoothingSigmasPerLevel2.SetSize( numberOfLevels2 );
    smoothingSigmasPerLevel2[0] = 0;
    smoothingSigmasPerLevel2[1] = 0;
    smoothingSigmasPerLevel2[2] = 0;

    registration_->SetNumberOfLevels( numberOfLevels2 );
    registration_->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel2);
    registration_->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel2);

//    RegistrationType::TransformParametersAdaptorsContainerType adaptors;



    OptimizerType::BoundSelectionType boundSelect( transform_->GetNumberOfParameters() );
    OptimizerType::BoundValueType upperBound( transform_->GetNumberOfParameters() );
    OptimizerType::BoundValueType lowerBound( transform_->GetNumberOfParameters() );

    boundSelect.Fill( OptimizerType::UNBOUNDED );
    upperBound.Fill( 0.0 );
    lowerBound.Fill( 0.0 );

    optimizer_->SetBoundSelection( boundSelect );
    optimizer_->SetUpperBound( upperBound );
    optimizer_->SetLowerBound( lowerBound );
    optimizer_->SetCostFunctionConvergenceFactor( 1.e+7 ); //1e7
    optimizer_->SetGradientConvergenceTolerance( 1e-6 ); //1e-6
    optimizer_->SetNumberOfIterations( 20 ); //500
    optimizer_->SetMaximumNumberOfFunctionEvaluations( 20 ); //500
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


    compositeTransform_->AddTransform(registration_->GetModifiableTransform());
    resampler_->SetInput((*moving_images_)->GetReader()->GetOutput());
    resampler_->SetTransform(compositeTransform_);
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
