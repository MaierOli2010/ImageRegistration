#ifndef MYREGISTRATION_H
#define MYREGISTRATION_H
#include <QWidget>
#include <QString>
#include <QStringList>
#include <QMessageBox>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>

#include "itktypesandincludes.h"
#include "myregistrationobserver.h"

class MyImageClass;
class ImageRegistration;
class registrationObserver;

class MyRegistration
{
public:
    explicit MyRegistration(ImageRegistration *myimreg, MyImageClass* fixed_image,
                            std::unique_ptr<MyImageClass> *moving_images);
    ~MyRegistration();
    void SaveDICOMSeries(QString save_path);
    void StartRegistration();
    void ShowResultingFit();
    void SetSlicePositionResult(int position);
    void SetSlicePositionObserver(int position);

private:
    QMessageBox msg_box_;
    void ComputeDifference();
    ImageRegistration* imreg_;
    MyImageClass* fixed_image_;
    std::unique_ptr<MyImageClass>* moving_images_;


    FilterType::Pointer connector_result_;
    itk::GDCMImageIO::Pointer gdcmIO_;
    itk::GDCMSeriesFileNames::Pointer namesGenerator_;
    vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX_result_;
    ImageCasterType::Pointer fixedImageCaster_;
    ImageCasterType::Pointer movingImageCaster_;
    MatchingFilterType::Pointer matcher_;




    MetricType::Pointer metric_;
    TransformType::Pointer transform_;
    OptimizerType::Pointer optimizer_;
    InterpolatorType::Pointer interpolator_;
    RegistrationType::Pointer registration_;
    ResampleFilterType::Pointer resampler_;
    DifferenceFilterType::Pointer difference_;
    RescalerType::Pointer intensity_rescaler_;
    MyRegistrationObserver::Pointer observer_;
    InitializerType::Pointer transformInitializer_;

    std::shared_ptr<registrationObserver> regobs_window_;

    int mMinSliderX_result_;
    int mMaxSliderX_result_;

};

#endif // MYREGISTRATION_H
