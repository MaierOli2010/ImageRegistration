#ifndef MYIMAGECLASS_H
#define MYIMAGECLASS_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QMessageBox>
#include <exception>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>

#include "itktypesandincludes.h"


class ImageRegistration;
class MyImageClass
{
public:
    explicit MyImageClass(ImageRegistration *myimreg);
        ~MyImageClass();

        void SetFileName(QString mypath, QStringList myfiles);
        void LoadDICOM();
        void SetSlicePosition(int position);
        void GetDICOMSeries();
        void DrawDicomImg();
        ReaderType::Pointer GetReader();

    private:

        std::exception exception_;
        ReaderType::Pointer reader_;
        FilterType::Pointer connector_;


        itk::GDCMImageIO::Pointer gdcmIO_;
        itk::GDCMSeriesFileNames::Pointer namesGenerator_;
        vtkSmartPointer<vtkImageViewer2> imageViewerDCMSeriesX_;

        ImageRegistration *imreg_;
        QString path_;
        QStringList files_;
        QMessageBox msg_box_;
        void DrawDICOMImg();


        int mMinSliderX_;
        int mMaxSliderX_;




};

#endif // MYIMAGECLASS_H
