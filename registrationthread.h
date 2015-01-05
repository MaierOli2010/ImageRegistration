#ifndef REGISTRATIONTHREAD_H
#define REGISTRATIONTHREAD_H

#include <QThread>
#include <myregistration.h>


class RegistrationThread : public QThread
{
public:
    RegistrationThread();
    void setRegistration(std::vector<std::unique_ptr<MyRegistration> > *myreg);
private:
    std::vector<std::unique_ptr<MyRegistration>>* registration_;
protected:
    void run();
};

#endif // REGISTRATIONTHREAD_H
