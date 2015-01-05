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



    //Const interface ensures that all options invoked on the optimizer are read-only
    typedef const OptimizerType* OptimizerPointer;
    typedef const RegistrationType* RegistrationTypePointer;
    void Execute(itk::Object *caller, const itk::EventObject &event);
    void Execute(const itk::Object *object, const itk::EventObject &event);

    void SetSlicePositionObserver(int position);
    void setObserverWindow(std::unique_ptr<registrationObserver> *regobs);
    void setRegistration(RegistrationTypePointer registration);
private:
    std::unique_ptr<registrationObserver>* regobs_;
    MyImageClass* fixed_image_;
    std::unique_ptr<MyImageClass>* moving_image_;
    FilterType::Pointer connector_result_;
    QMessageBox msg_box_;
    const vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX_result_ = vtkSmartPointer<vtkImageViewer2>::New();
    RegistrationTypePointer registration_;

    const DifferenceFilterType::Pointer difference_ = DifferenceFilterType::New();
    const ResampleFilterType::Pointer resampler_ = ResampleFilterType::New();
    const RescalerType::Pointer rescaler_ = RescalerType::New();

    void ComputeDifference();
    void ShowResultingFit();
};

#endif // MYREGISTRATIONOBSERVER_H
