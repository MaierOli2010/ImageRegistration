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
#include <itkNumericSeriesFileNames.h>

class MyImageClass;
class ImageRegistration;
class registrationObserver;
/*!
 * \brief Takes care of the registration process and saves the final result.
 * In the current build a multistage multiresolution registration is performed. The initial registration
 * uses a translation transform with low resolution for initial correction of wider misalignements.
 * Afterwards a non-rigid BSpline transformation combined with a LBFGSB optimizer is used to get the final
 * result.
 */
class MyRegistration
{
public:
    /*!
     * \brief constructor
     * \param myimreg pointer to the main imageregistration object
     * \param fixed_image pointer to the fixed series
     * \param moving_images pointer to the moving series
     */
    explicit MyRegistration(ImageRegistration *myimreg, MyImageClass* fixed_image,
                            std::unique_ptr<MyImageClass> *moving_images);
    ~MyRegistration();
    /*!
     * \brief saves the registered series.
     * \param save_path path to the main output folder.
     * \param subfolder subfolder where images are saved.
     */
    void SaveDICOMSeries(QString save_path, std::string subfolder);
    /*!
     * \brief starts the registration process.
     * This function contains all the itk code, which is need to register the images.
     * First, an intensity rescaling is performed to minimize the difference and
     * enhance the registration process. Second is a translation registration to account
     * for possible shifts between the series and minimize the difference to speed up
     * the non rigid transform which follow last. The output of the last stage is passed
     * to a resample image filter, which uses the last(best) transform parameter from
     * the non rigid transform to transform the moving series.
     */
    void StartRegistration();
    /*!
     * \brief sisplays the final registration result using a difference image.
     */
    void ShowResultingFit();
    /*!
     * \brief sets the image to be displayed in the UI.
     * \param position number of image to be displayed (Slice).
     */
    void SetSlicePositionResult(int position);
    /*!
     * \brief sets the image to be displayed in the observer UI.
     * \param position number of image to be displayed (Slice).
     */
    void SetSlicePositionObserver(int position);


private:
    QMessageBox msg_box_;
    void ComputeDifference();
    ImageRegistration* imreg_;
    MyImageClass* fixed_image_;
    std::unique_ptr<MyImageClass>* moving_images_;


    FilterType::Pointer connector_result_;
    itk::GDCMImageIO::Pointer gdcmIO_;
    vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX_result_;
    ImageCasterType::Pointer fixedImageCaster_;
    ImageCasterType::Pointer movingImageCaster_;
    MatchingFilterType::Pointer matcher_;
    MetricType::Pointer Tmetric_;




    TTransformType::Pointer Ttransform_;
    TOptimizerType::Pointer Toptimizer_;
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
    CompositeTransformType::Pointer compositeTransform_;
    TRegistrationType::Pointer Tregistration_;

    std::shared_ptr<registrationObserver> regobs_window_;

    int mMinSliderX_result_;
    int mMaxSliderX_result_;

};

#endif // MYREGISTRATION_H
