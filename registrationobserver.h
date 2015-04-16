#ifndef REGISTRATIONOBSERVER_H
#define REGISTRATIONOBSERVER_H

#include <QWidget>


namespace Ui {

class registrationObserver;
}
/*!
 * \brief The registrationObserver class. Header created by Qt Designer.
 * This class takes care of creating the observer window.
 */
class registrationObserver : public QWidget
{
    Q_OBJECT

public:
    explicit registrationObserver(QWidget *parent = 0);
    ~registrationObserver();

//private:
    Ui::registrationObserver *ui;
};

#endif // REGISTRATIONOBSERVER_H
