#include "registrationobserver.h"
#include "ui_registrationobserver.h"


RegistrationObserver::RegistrationObserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationObserver)
{
    ui->setupUi(this);

}

RegistrationObserver::~RegistrationObserver()
{
    delete ui;
}

