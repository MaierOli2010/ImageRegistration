#ifndef MYREGISTRATIONOBSERVER_H
#define MYREGISTRATIONOBSERVER_H

#include <itkCommand.h>
#include "itktypesandincludes.h"
#include <memory>

#include "registrationobserver.h"


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


    //Const interface ensures that all options invoked on the optimizer are read-only
    typedef const OptimizerType* OptimizerPointer;
    void Execute(itk::Object *caller, const itk::EventObject &event);
    void Execute(const itk::Object *object, const itk::EventObject &event);


    void setObserverWindow(std::unique_ptr<registrationObserver> *regobs);

private:
    std::unique_ptr<registrationObserver>* regobs_;
};

#endif // MYREGISTRATIONOBSERVER_H
