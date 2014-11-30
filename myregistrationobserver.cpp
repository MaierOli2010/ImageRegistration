#include "myregistrationobserver.h"

#include "ui_registrationobserver.h"
#include <QMessageBox>

#include <QWidget>
#include <string>

MyRegistrationObserver::MyRegistrationObserver()
{
}
void MyRegistrationObserver::Execute(itk::Object *caller, const itk::EventObject &event)
{
    Execute( (const itk::Object *)caller, event);
}

void MyRegistrationObserver::Execute(const itk::Object *object, const itk::EventObject &event)
{
    OptimizerPointer optimizer = dynamic_cast< OptimizerPointer>( object );
    if( typeid( &event ) == typeid( itk::StartEvent ))
    {

    }
    if (! (typeid( &event ) == typeid( itk::IterationEvent )))
    {
//        QMessageBox msg_box;
//        msg_box.setText("no iteration event");
//        msg_box.show();
        std::string iteration = std::to_string(optimizer->GetCurrentIteration());
        (*regobs_)->ui->listWidget->addItem(iteration.c_str());
        return;
    }
//    QMessageBox msg_box;
//    msg_box.setText("iteration event");
//    msg_box.show();
    std::string iteration = std::to_string(optimizer->GetCurrentIteration());
    (*regobs_)->ui->listWidget->addItem(iteration.c_str());


}

MyRegistrationObserver::~MyRegistrationObserver()
{

}

void MyRegistrationObserver::setObserverWindow(std::unique_ptr<registrationObserver>* regobs)
{
    regobs_ = regobs;
}
