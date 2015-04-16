#ifndef REGISTRATIONTHREAD_H
#define REGISTRATIONTHREAD_H

#include <QThread>
#include <myregistration.h>

/*!
 * \brief The RegistrationThread class This class wraps the registration process itsel into a own thread.
 */
class RegistrationThread : public QThread
{
public:
    /*!
     * \brief RegistrationThread Constructor
     */
    RegistrationThread();
    /*!
     * \brief setRegistration Sets the internal registration vector to the specified.
     * \param myreg Vector containing all registration objects
     */
    void setRegistration(std::vector<std::unique_ptr<MyRegistration> > *myreg);
private:
    std::vector<std::unique_ptr<MyRegistration>>* registration_;
protected:
    /*!
     * \brief run When run is invoked the registration is started for all registration objects
     * one after another.
     */
    void run();
};

#endif // REGISTRATIONTHREAD_H
