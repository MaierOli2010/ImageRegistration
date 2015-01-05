#include "myregistrationobserver.h"

#include "ui_registrationobserver.h"
#include <QMessageBox>

#include <QWidget>
#include <string>

#include "myimageclass.h"

MyRegistrationObserver::MyRegistrationObserver()
{
}
void MyRegistrationObserver::Execute(itk::Object *caller, const itk::EventObject &event)
{
    Execute( (const itk::Object *)caller, event);
}
//If an iteration event occurs, the following code computes the actual difference and shows it in the observer
// window.
void MyRegistrationObserver::Execute(const itk::Object *object, const itk::EventObject &event)
{
    OptimizerPointer optimizer = dynamic_cast< OptimizerPointer>( object );
    if ( !(typeid( event ) == typeid( itk::IterationEvent )))
    {
        return;
    }
    (*regobs_)->show();
    (*regobs_)->ui->lcdNumber->display(static_cast<int>(optimizer->GetCurrentIteration()));
    resampler_->SetInput((*moving_image_)->GetReader()->GetOutput());
    resampler_->SetTransform(registration_->GetOutput()->Get());
    resampler_->SetSize(fixed_image_->GetReader()->GetOutput()->GetLargestPossibleRegion().GetSize());
    resampler_->SetOutputOrigin(fixed_image_->GetReader()->GetOutput()->GetOrigin());
    resampler_->SetOutputSpacing(fixed_image_->GetReader()->GetOutput()->GetSpacing());
    resampler_->SetOutputDirection(fixed_image_->GetReader()->GetOutput()->GetDirection());
    resampler_->SetDefaultPixelValue(100);
    ShowResultingFit();

}

MyRegistrationObserver::~MyRegistrationObserver()
{

}

void MyRegistrationObserver::setObserverWindow(std::unique_ptr<registrationObserver>* regobs)
{
    regobs_ = regobs;
}

void MyRegistrationObserver::setFixedImage(MyImageClass* image)
{
    fixed_image_ = image;
}

void MyRegistrationObserver::setMovingImage(std::unique_ptr<MyImageClass>* image)
{
    moving_image_ = image;
}

void MyRegistrationObserver::ComputeDifference()
{
    difference_->SetInput1(fixed_image_->GetReader()->GetOutput());
    difference_->SetInput2(resampler_->GetOutput());


    rescaler_->SetInput(difference_->GetOutput());
    rescaler_->SetOutputMinimum(0);
    rescaler_->SetOutputMaximum(255);
    resampler_->SetDefaultPixelValue(1);
}

void MyRegistrationObserver::ShowResultingFit()
{
    try
    {
    ComputeDifference();
    connector_result_ = FilterType::New();

    connector_result_->SetInput(rescaler_->GetOutput());
    try
    {
      connector_result_->Update();
    }
    catch(itk::ExceptionObject &err)
    {
        throw err;
    }
    catch(std::exception &err)
    {
        throw err;
    }

    (*regobs_)->ui->sliderObserver->setMinimum(imageViewerDCMSeriesX_result_->GetSliceMin());
    (*regobs_)->ui->sliderObserver->setMaximum(imageViewerDCMSeriesX_result_->GetSliceMax());
    imageViewerDCMSeriesX_result_->SetInputData(connector_result_->GetOutput());
    imageViewerDCMSeriesX_result_->SetRenderWindow((*regobs_)->ui->vtkRendererObserver->GetRenderWindow());
    imageViewerDCMSeriesX_result_->Render();
    }
    catch(itk::ExceptionObject &err)
    {
        msg_box_.setText(err.GetDescription());
        msg_box_.exec();
    }
}

void MyRegistrationObserver::setRegistration(RegistrationTypePointer registration)
{
    registration_ = registration;
}

void MyRegistrationObserver::SetSlicePositionObserver(int position)
{
    imageViewerDCMSeriesX_result_->SetSlice(position);
    imageViewerDCMSeriesX_result_->Render();
}
