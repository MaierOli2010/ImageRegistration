#include "registrationthread.h"

RegistrationThread::RegistrationThread()
{
}

void RegistrationThread::RegistrationThread::run()
{
    for(unsigned int i = 0; i < registration_->size(); i++)
    {
      (*registration_)[i]->StartRegistration();
    }
}

void RegistrationThread::setRegistration(std::vector<std::unique_ptr<MyRegistration> >* myreg)
{
    registration_ = myreg;
}
