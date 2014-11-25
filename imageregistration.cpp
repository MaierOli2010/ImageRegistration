#include "imageregistration.h"

#include <QFileDialog>
#include <itkImage.h>

#include "myimageclass.h"
#include "loadFile.h"
#include "myregistration.h"




ImageRegistration::ImageRegistration(QMainWindow *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    load_files_hdd_ = new LoadFile(this);
    fixed_image_ = new MyImageClass(this);

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

}

ImageRegistration::~ImageRegistration()
{
    delete fixed_image_;
    delete load_files_hdd_;
}


void ImageRegistration::ShowFileLoad()
{
    load_files_hdd_->LoadFileNames();
    if(load_files_hdd_->GetFileName().isEmpty() == false)
    {
       stackedWidget -> setCurrentIndex(1);
       fixed_image_->SetFileName(load_files_hdd_->GetPath(), load_files_hdd_->GetFileName());
       fixed_image_->GetDICOMSeries();
    }
}

void ImageRegistration::ShowMoving()
{
    if(load_files_hdd_->GetFileName().isEmpty() == true)
    {
        msg_box_.setText("Please choose a DICOM File");
        msg_box_.exec();
    }
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
           moving_image_vec_.push_back(new MyImageClass(this));
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

       moving_image_vec_.push_back(new MyImageClass(this));
       moving_image_vec_.back()->SetFileName(load_files_hdd_->GetPath(), load_files_hdd_->GetFileName());
       moving_image_vec_.back()->GetDICOMSeries();
    }
}

void ImageRegistration::ShowComputing()
{
    verticalSlider->setMinimum(0);
    verticalSlider->setMaximum(moving_image_vec_.size()-1);
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

void ImageRegistration::SliderMovedFixed(int position)

{
    fixed_image_->SetSlicePosition(position);
}

void ImageRegistration::SliderMovedMoving(int position)

{
  moving_image_vec_[verticalSlider->value()]->SetSlicePosition(position);
}

void ImageRegistration::SliderMovedResult(int position)

{
    registration_[0]->SetSlicePositionResult(position);
}

void ImageRegistration::SaveFiles()
{
    load_files_hdd_->SaveDirectoryPath();
    if(load_files_hdd_->GetSavePath().isEmpty() == false)
    {
        for(int i = 0; i < moving_image_vec_.size(); ++i)
        {
          registration_[i]->SaveDICOMSeries(load_files_hdd_->GetSavePath());
        }
    }
}
void ImageRegistration::StartRegistration()
{
    if(registration_.empty())
    {
        for(int i = 0; i < moving_image_vec_.size(); ++i)
        {
            registration_.push_back(new MyRegistration(this, fixed_image_, moving_image_vec_[i]));
        }
    }
    else
    {
      registration_.clear();
      for(int i = 0; i < moving_image_vec_.size(); ++i)
      {
          registration_.push_back(new MyRegistration(this, fixed_image_, moving_image_vec_[i]));
      }
    }
    for(int i = 0; i < moving_image_vec_.size(); ++i)
    {
      registration_[i]->StartRegistration();
    }

}

void ImageRegistration::ShowResultingFit()
{
    stackedWidget -> setCurrentIndex(4);
    registration_[0]->ShowResultingFit();
}

void ImageRegistration::SelectMovingSeries(int position)
{
    moving_image_vec_[position]->DrawDicomImg();
}
