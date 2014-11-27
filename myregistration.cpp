#include "myregistration.h"

#include "imageregistration.h"
#include "myimageclass.h"

#include <string>

#include <QVTKWidget.h>
#include <itkImageSeriesWriter.h>


#include <itkCastImageFilter.h>

MyRegistration::MyRegistration(ImageRegistration *myimreg, MyImageClass* fixed_image,
                               MyImageClass* moving_images):
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
    typedef itk::CastImageFilter<ImageType, ImageType> CastFilterType;

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

    registration_->SetFixedImage(fixed_image_->GetReader()->GetOutput());
    registration_->SetMovingImage(moving_images_->GetReader()->GetOutput());
    registration_->SetFixedImageRegion(fixed_image_->GetReader()->GetOutput()->GetBufferedRegion());

    typedef RegistrationType::ParametersType ParametersType;
    ParametersType initial_parameters( transform_->GetNumberOfParameters());

    initial_parameters[0] = 0.0;
    initial_parameters[1] = 0.0;

    registration_->SetInitialTransformParameters (initial_parameters);

    optimizer_->SetMaximumStepLength(2.00);
    optimizer_->SetMinimumStepLength(0.01);
    optimizer_->SetNumberOfIterations(200);
    optimizer_->AddObserver( itk::IterationEvent(), observer_);
    try
    {
        registration_->Update();
    }
    catch(itk::ExceptionObject &err)
    {
        throw err;
    }

    ParametersType final_parameters = registration_->GetLastTransformParameters();

    const double x_trans = final_parameters[0];
    const double y_trans = final_parameters[1];
    const unsigned int number_of_it = optimizer_->GetCurrentIteration();
    const double best_value = optimizer_->GetValue();

    resampler_->SetInput(moving_images_->GetReader()->GetOutput());
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
    connector_result_->Update();


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
