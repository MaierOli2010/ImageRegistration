#ifndef MYREGISTRATIONOBSERVER_H
#define MYREGISTRATIONOBSERVER_H

#include <itkCommand.h>
#include "itktypesandincludes.h"
#include <QWidget>
#include <string>



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

    std::string getIteration(void);
    std::string getValue(void);

private:

    std::string iteration_ = NULL;
    std::string value_ = NULL;
};

#endif // MYREGISTRATIONOBSERVER_H
