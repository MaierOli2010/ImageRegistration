#ifndef IMAGEREGISTRATION_H
#define IMAGEREGISTRATION_H

#include "ui_imageregistration.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QVTKWidget.h>
#include <vector>
#include <memory>


#define FIXED 1
#define MOVING 2


class MyRegistration;
class LoadFile;
class MyImageClass;
class registrationObserver;
class RegistrationThread;

class ImageRegistration : public QMainWindow, public Ui::mainWindow
{
    Q_OBJECT

public:
    explicit ImageRegistration(QMainWindow *parent = 0);
    ~ImageRegistration();
    std::unique_ptr<registrationObserver>* GetObserverWindow();
    
private:
    MyImageClass *fixed_image_;
    std::vector<std::unique_ptr<MyImageClass>> moving_image_vec_;
    QMessageBox msg_box_;
    LoadFile *load_files_hdd_;
    std::vector<std::unique_ptr<MyRegistration>> registration_;
    std::unique_ptr<registrationObserver> regobs_window_;
    std::unique_ptr<RegistrationThread> reg_thread_;
    int selected_series_;


private slots:
    void ShowFileLoad();
    void ShowMoving();
    void ShowComputing();
    void SliderMovedFixed(int position);
    void SliderMovedMoving(int position);
    void SaveFiles();
    void StartRegistration();
    void ShowResultingFit();
    void SliderMovedResult(int);
    void AddMovingSeries();
    void SelectMovingSeries(int);
    void SliderObserverMoved(int position);
    void MovingSeriesObserverSelected(int number);

};

#endif // IMAGEREGISTRATION_H
