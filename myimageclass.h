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
 * \brief The MyImageClass class.
 * This class is the container for all image series to read. It takes care of loading the series into the memory and provides
 * a function to display the images in the UI. It also provides a function to save the loaded series to a directory.
 * In order to display the itk-Images they have to be connected to a VTK-image stream which can be displayed in a QVTK-Widget
 * in the GUI.
 */
class MyImageClass
{
public:
    /*!
     * \brief MyImageClass Constructor.
     * \param myimreg Pointer to the main registration object.
     */
    explicit MyImageClass(ImageRegistration *myimreg);
        ~MyImageClass();
    /*!
         * \brief SetFileName Specify the File Names to be read.
         * \param mypath Path to the files.
         * \param myfiles List of all files to be read.
         */
        void SetFileName(QString mypath, QStringList myfiles);
        /*!
         * \brief LoadDICOM Loads all files into the memory and displays them in the GUI.
         */
        void LoadDICOM();
        /*!
         * \brief SetSlicePosition Sets the image to be displayed in the UI.
         * \param position Number of Image to be displayed (Slice).
         */
        void SetSlicePosition(int position);
        /*!
         * \brief GetDICOMSeries Identifies all Dicom Series in the specified folder and display the
         * series ID in the GUI.
         * The folder is specified by SetFileName.
         */
        void GetDICOMSeries();
        /*!
         * \brief RedrawDICOMImg Triggers the rendering process when a series is selected to be displayed.
         */
        void RedrawDICOMImg();
        ReaderType::Pointer GetReader();
        itk::GDCMSeriesFileNames::Pointer GetFileNames();
        /*!
         * \brief SaveDICOMSeries Saves the stored series into the folder specified by save_path/subfolder
         * \param save_path The path to the main output folder.
         * \param subfolder The subfolder where images are saved.
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
