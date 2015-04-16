#include "imageregistration.h"
#include "ui_registrationobserver.h"

#include <QFileDialog>
//#include <itkImage.h>

#include "myimageclass.h"
#include "loadFile.h"
#include "registrationobserver.h"

#include "registrationthread.h"

#include "myregistration.h"


ImageRegistration::ImageRegistration(QMainWindow *parent) :
    QMainWindow(parent)
{
    //Setup Main UI
    setupUi(this);
    load_files_hdd_ = new LoadFile(this);
    fixed_image_ = new MyImageClass(this);
    //create Observerwindow
    regobs_window_ =  std::unique_ptr<registrationObserver> (new registrationObserver);
    reg_thread_ = std::unique_ptr<RegistrationThread> (new RegistrationThread);
    regobs_window_->hide();
    regobs_window_->ui->spinBox->setMinimum(0);
    regobs_window_->ui->spinBox->setMaximum(0);
    selected_series_ = 0;

    //Connect Signals from the UI to SLOTS
    connect(actionLoad_Files, SIGNAL(triggered()), this, SLOT(ShowFileLoad()));
    connect(actionSave_Files, SIGNAL(triggered()), this, SLOT(SaveFiles()));
    connect(actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(loadNew_Button, SIGNAL(clicked()), this, SLOT(ShowFileLoad()));
    connect(nextToMoving, SIGNAL(clicked()), this, SLOT(ShowMoving()));
    connect(nextToComp, SIGNAL(clicked()), this, SLOT(ShowComputing()));
    connect(hSliderFixed, SIGNAL(valueChanged(int)), this, SLOT(SliderMovedFixed(int)));
    connect(hSliderMoving, SIGNAL(valueChanged(int)), this, SLOT(SliderMovedMoving(int)));
    connect(startComputing_Button, SIGNAL(clicked()),  this, SLOT(StartRegistration()));
    connect(showFit, SIGNAL(clicked()), this, SLOT(ShowResultingFit()));
    connect(hSliderResult, SIGNAL(valueChanged(int)), this, SLOT(SliderMovedResult(int)));
    connect(ButtonAddMoving, SIGNAL(clicked()), this, SLOT(AddMovingSeries()));
    connect(verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(SelectMovingSeries(int)));
    connect(regobs_window_->ui->sliderObserver, SIGNAL(valueChanged(int)), this, SLOT(SliderObserverMoved(int)));
    connect(regobs_window_->ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(MovingSeriesObserverSelected(int)));

}

ImageRegistration::~ImageRegistration()
{
    delete fixed_image_;
    delete load_files_hdd_;
}


void ImageRegistration::ShowFileLoad()
{
    load_files_hdd_->LoadFileNames();
    //Check if a Files was selected and change to next Window
    if(load_files_hdd_->GetFileName().isEmpty() == false)
    {
       stackedWidget -> setCurrentIndex(1);
       fixed_image_->SetFileName(load_files_hdd_->GetPath(), load_files_hdd_->GetFileName());
       fixed_image_->GetDICOMSeries();
    }
    //check if a moving series is already loaded. If true clear it.
    if(moving_image_vec_.empty() == false)
    {
        moving_image_vec_.clear();
    }
}

void ImageRegistration::ShowMoving()
{
    if(load_files_hdd_->GetFileName().isEmpty() == true)
    {
        msg_box_.setText("Please choose a DICOM File");
        msg_box_.exec();
    }
    //Select a Moving Series and load it into Memory. The series gets stored in the moving_image_vec_ Vektor.
    else
    {
        try
        {
          fixed_image_->LoadDICOM();
        }
        catch(itk::ExceptionObject &excp)
        {
            msg_box_.setText(excp.GetDescription());
            msg_box_.exec();
            return;
        }
        catch(std::exception &e)
        {
            msg_box_.setText(e.what());
            msg_box_.exec();
            return;
        }
        load_files_hdd_->LoadFileNames();
        if(load_files_hdd_->GetFileName().isEmpty() == false)
        {
           stackedWidget -> setCurrentIndex(2);
           moving_image_vec_.push_back(std::unique_ptr<MyImageClass>(new MyImageClass(this)));
           moving_image_vec_.back()->SetFileName(load_files_hdd_->GetPath(), load_files_hdd_->GetFileName());
           moving_image_vec_.back()->GetDICOMSeries();
        }
    }
}

void ImageRegistration::AddMovingSeries()
{
    load_files_hdd_->LoadFileNames();
    if(load_files_hdd_->GetFileName().isEmpty() == false)
    {
        try
        {
          moving_image_vec_.back()->LoadDICOM();
        }
        catch(itk::ExceptionObject &excp)
        {
            msg_box_.setText(excp.GetDescription());
            msg_box_.exec();
            return;
        }
        catch(std::exception &e)
        {
            msg_box_.setText(e.what());
            msg_box_.exec();
            return;
        }

       moving_image_vec_.push_back(std::unique_ptr<MyImageClass>(new MyImageClass(this)));
       moving_image_vec_.back()->SetFileName(load_files_hdd_->GetPath(), load_files_hdd_->GetFileName());
       moving_image_vec_.back()->GetDICOMSeries();
    }
}

// Show both Images and visualy compare them.
void ImageRegistration::ShowComputing()
{
    regobs_window_->show();
    verticalSlider->setMinimum(0);
    verticalSlider->setMaximum(moving_image_vec_.size()-1);
    regobs_window_->ui->spinBox->setMaximum(moving_image_vec_.size()-1);
    try
    {
      moving_image_vec_.back()->LoadDICOM();
    }
    catch(itk::ExceptionObject &excp)
    {
        msg_box_.setText(excp.GetDescription());
        msg_box_.exec();
        return;
    }
    catch(std::exception &e)
    {
        msg_box_.setText(e.what());
        msg_box_.exec();
        return;
    }
    stackedWidget -> setCurrentIndex(3);
}

//Changes the current Fixed Image depending on slider position
void ImageRegistration::SliderMovedFixed(int position)

{
    fixed_image_->SetSlicePosition(position);
}
//Changes the current Moving Image depending on slider position
void ImageRegistration::SliderMovedMoving(int position)

{
  moving_image_vec_[verticalSlider->value()]->SetSlicePosition(position);
}
//Changes the current Resulting Image depending on slider position
void ImageRegistration::SliderMovedResult(int position)

{
    registration_[0]->SetSlicePositionResult(position);
}

//Saves the registrated images to a specified folder
void ImageRegistration::SaveFiles()
{
    load_files_hdd_->SaveDirectoryPath();
    if(load_files_hdd_->GetSavePath().isEmpty() == false)
    {
        std::string subfolder = "/fixed_series";
        fixed_image_->SaveDICOMSeries(load_files_hdd_->GetSavePath(), subfolder);
        for(unsigned int i = 0; i < moving_image_vec_.size(); ++i)
        {
          subfolder = "/moving_series_";
          subfolder += std::to_string(i);
          moving_image_vec_[i]->SaveDICOMSeries(load_files_hdd_->GetSavePath(),subfolder);
          subfolder = "/registered_series_";
          subfolder += std::to_string(i);
          registration_[i]->SaveDICOMSeries(load_files_hdd_->GetSavePath(),subfolder);
        }
    }
}

//Registrate the Moving Series to the fixed one
void ImageRegistration::StartRegistration()
{

    if(registration_.empty())
    {
        for(unsigned int i = 0; i < moving_image_vec_.size(); ++i)
        {
            registration_.push_back(std::unique_ptr<MyRegistration>(new MyRegistration(this, fixed_image_, &moving_image_vec_[i])));
        }
    }
    else
    {
      registration_.clear();
      for(unsigned int i = 0; i < moving_image_vec_.size(); ++i)
      {
          registration_.push_back(std::unique_ptr<MyRegistration>(new MyRegistration(this, fixed_image_, &moving_image_vec_[i])));
      }
    }
    reg_thread_->setRegistration(&registration_);
    reg_thread_->start();
}
//Shows the Resulting fit for the registrated Series. At the moment only the first series is displayed because
//there is a bug when visualizing more than 1 series. Still more work needed here
void ImageRegistration::ShowResultingFit()
{
    stackedWidget -> setCurrentIndex(4);
    registration_[0]->ShowResultingFit();
}
//Selext the moving series which should be displayed depending on the slider position. Doesn't work properly!!!
void ImageRegistration::SelectMovingSeries(int position)
{
    moving_image_vec_[position]->RedrawDICOMImg();
}
//Provides a pointer to the observer window
std::unique_ptr<registrationObserver>* ImageRegistration::GetObserverWindow()
{
    return &regobs_window_;
}
//Changes the current Observed Image depending on slider position
void ImageRegistration::SliderObserverMoved(int position)

{
    registration_[selected_series_]->SetSlicePositionObserver(position);
}

void ImageRegistration::MovingSeriesObserverSelected(int number)
{
    selected_series_ = number;
}
