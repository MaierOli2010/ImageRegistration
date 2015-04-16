#ifndef REGISTRATIONOBSERVER_H
#define REGISTRATIONOBSERVER_H

#include <QWidget>


namespace Ui {

class RegistrationObserver;
}
/*!
 * \brief Connection between Qt design sheet and the program.
 * This class takes care of creating the observer window.
 */
class RegistrationObserver : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationObserver(QWidget *parent = 0);
    ~RegistrationObserver();

//private:
    Ui::RegistrationObserver *ui;
};

#endif // REGISTRATIONOBSERVER_H
