#include "myregistrationobserver.h"


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
    if (! itk::IterationEvent().CheckEvent (&event))
    {
        return;
    }
    iteration_ = optimizer->GetCurrentIteration();
    value_ = optimizer->GetValue();
}


std::string MyRegistrationObserver::getIteration(void)
{
    return iteration_;
}

std::string MyRegistrationObserver::getValue(void)
{
    return value_;
}
MyRegistrationObserver::~MyRegistrationObserver()
{

}
