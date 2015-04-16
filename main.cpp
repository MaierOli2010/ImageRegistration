#include "imageregistration.h"

#if MY_QT_VERSION == 4
    #include <QtGui/QApplication>
#else
    #include <QtWidgets/QApplication>
#endif



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageRegistration w;

    w.show();
    return a.exec();
}
