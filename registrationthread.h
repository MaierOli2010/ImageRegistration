#ifndef REGISTRATIONTHREAD_H
#define REGISTRATIONTHREAD_H

#include <QThread>
#include <myregistration.h>

/*!
 * \brief Puts the registration process into a own thread.
 */
class RegistrationThread : public QThread
{
public:
    /*!
     * \brief constructor
     */
    RegistrationThread();
    /*!
     * \brief sets the internal registration vector to the specified.
     * \param myreg vector containing all registration objects
     */
    void setRegistration(std::vector<std::unique_ptr<MyRegistration> > *myreg);
private:
    std::vector<std::unique_ptr<MyRegistration>>* registration_;
protected:
    /*!
     * \brief If run is invoked the registration is started for all registration objects,
     * one after another.
     */
    void run();
};

#endif // REGISTRATIONTHREAD_H
