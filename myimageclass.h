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
/*!
 * \brief Loads,saves and displays a DICOM series and keeps it in memory for processing.
 * This class is the container for all image series to read. It takes care of loading the series into the memory and provides
 * a function to display the images in the UI. It also provides a function to save the loaded series to a directory.
 * In order to display the itk-Images they have to be connected to a VTK-image stream which can be displayed in a QVTK-Widget
 * in the GUI.
 */
class MyImageClass
{
public:
    /*!
     * \brief constructor.
     * \param myimreg pointer to the main imageregistration object.
     */
    explicit MyImageClass(ImageRegistration *myimreg);
        ~MyImageClass();
    /*!
         * \brief specifies the file fames to be read.
         * \param mypath Path to the files.
         * \param myfiles List of all files to be read.
         */
        void SetFileName(QString mypath, QStringList myfiles);
        /*!
         * \brief loads all files into the memory and displays them in the GUI.
         */
        void LoadDICOM();
        /*!
         * \brief sets the image to be displayed in the UI.
         * \param position number of image to be displayed (slice).
         */
        void SetSlicePosition(int position);
        /*!
         * \brief identifies all DICOM series in the specified folder and displays the
         * series ID in the GUI.
         * The folder is specified by the SetFileName function.
         */
        void GetDICOMSeries();
        /*!
         * \brief triggers the rendering process when a series is selected to be displayed.
         */
        void RedrawDICOMImg();
        ReaderType::Pointer GetReader();
        itk::GDCMSeriesFileNames::Pointer GetFileNames();
        /*!
         * \brief saves the stored series into the folder specified by save_path/subfolder
         * \param save_path path to the main output folder.
         * \param subfolder subfolder where images are saved.
         */
        void SaveDICOMSeries(QString save_path, std::string subfolder);

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
        /*!
         * \brief DrawDICOMImg Displays the Image in the UI
         */
        void DrawDICOMImg();


        int mMinSliderX_;
        int mMaxSliderX_;




};

#endif // MYIMAGECLASS_H
