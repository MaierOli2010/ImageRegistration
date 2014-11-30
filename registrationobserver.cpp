#include "registrationobserver.h"
#include "ui_registrationobserver.h"


registrationObserver::registrationObserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registrationObserver)
{
    ui->setupUi(this);
}

registrationObserver::~registrationObserver()
{
    delete ui;
}
