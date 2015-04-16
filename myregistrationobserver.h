#ifndef MYREGISTRATIONOBSERVER_H
#define MYREGISTRATIONOBSERVER_H

#include <itkCommand.h>
#include "itktypesandincludes.h"
#include <memory>
#include <QMessageBox>

#include "registrationobserver.h"

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>

#include "itktypesandincludes.h"

class MyImageClass;
/*!
 * \brief Provides an observer to visually compare the difference of the 2 series at every iteration.
 * This class is a observer class which is used to provide a visual compare tool during the registration process.
 * Everytime the optimizer triggers an iteration event the observer routine is called and the observer GUI is updated.
 * The visual tool used is a difference image between the moving and fixed series.
 *
 * In order to work properly, the observer has to be added to a optimizer.
 *
 * For further details regarding observers please read the ITK-Documentation.
 */
class MyRegistrationObserver : public itk::Command
{
protected:
    MyRegistrationObserver();
public:
    ~MyRegistrationObserver();
    typedef MyRegistrationObserver Self;
    typedef itk::Command Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    itkNewMacro(Self)
    void setFixedImage(MyImageClass* image);
    void setMovingImage(std::unique_ptr<MyImageClass>* image);



    ///Const interface ensures that all options invoked on the optimizer are read-only
    typedef const OptimizerType* OptimizerPointer;
    ///Const interface ensures that all options invoked on the optimizer are read-only
    typedef const TOptimizerType* ToptimizerPointer;
    typedef const RegistrationType* RegistrationTypePointer;
    typedef const TRegistrationType* TRegistrationTypePointer;
    /*!
     * \brief Execute observer is not intended to modifie the optimizer, so this function just returns without any actions.
     * \param caller itk optimizer to be observed
     * \param event itk event to trigger the observer
     */
    void Execute(itk::Object *caller, const itk::EventObject &event);
    /*!
     * \brief Execute checks if the event is an iteration event and updates the UI.
     * \param object itk optimizer to be observed.
     * \param event itk event to trigger the observer.
     */
    void Execute(const itk::Object *object, const itk::EventObject &event);

    void SetSlicePositionObserver(int position);
    void setObserverWindow(std::unique_ptr<RegistrationObserver> *regobs);
    void setRegistration(RegistrationTypePointer registration);
    void setRegistration(TRegistrationTypePointer registration);
private:
    std::unique_ptr<RegistrationObserver>* regobs_;
    MyImageClass* fixed_image_;
    std::unique_ptr<MyImageClass>* moving_image_;
    FilterType::Pointer connector_result_;
    QMessageBox msg_box_;
    const vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX_result_ = vtkSmartPointer<vtkImageViewer2>::New();
    RegistrationTypePointer registration_ = NULL;
    TRegistrationTypePointer Tregistration_ =  NULL;

    const DifferenceFilterType::Pointer difference_ = DifferenceFilterType::New();
    const ResampleFilterType::Pointer resampler_ = ResampleFilterType::New();
    const RescalerType::Pointer rescaler_ = RescalerType::New();


    void ComputeDifference();
    void ShowResultingFit();
};

#endif // MYREGISTRATIONOBSERVER_H
