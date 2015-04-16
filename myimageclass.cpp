#include "myimageclass.h"
#include "imageregistration.h"

#include <QVTKWidget.h>
#include <itkImageSeriesWriter.h>

#include <itkCastImageFilter.h>

#include <itkNumericSeriesFileNames.h>


MyImageClass::MyImageClass(ImageRegistration *myimreg):
    imreg_(myimreg)
{
  imageViewerDCMSeriesX_ = vtkSmartPointer<vtkImageViewer2>::New();

}
MyImageClass::~MyImageClass()
{
}
//Loads the Dicomfiles into the memory
void MyImageClass::LoadDICOM()
{
  try
  {
    //check if a Fixed or Moving Series was selected
    if(imreg_->listWidget->currentIndex().isValid() == false &&
            imreg_->listWidgetMoving->currentIndex().isValid() == false)
    {
        throw exception_;
    }
    //Load the Fixed Image Series into Memory.
    if(imreg_->listWidget->currentIndex().isValid() == true && imreg_->stackedWidget->currentIndex() == 1)
    {
      const ReaderType::FileNamesContainer & filenames =
                             namesGenerator_->GetFileNames(imreg_->listWidget->currentItem()->
                                                          text().toStdString());
      reader_ = ReaderType::New();
      reader_->SetImageIO(gdcmIO_);
      reader_->SetFileNames (filenames);
    }
    //Load the Moving Image Series into Memory.
    else
    {
      const ReaderType::FileNamesContainer & filenames =
                                 namesGenerator_->GetFileNames(imreg_->listWidgetMoving->currentItem()->
                                                              text().toStdString());
      reader_ = ReaderType::New();
      reader_->SetImageIO(gdcmIO_);
      reader_->SetFileNames (filenames);
    }


    reader_->Update();
    }
    catch( itk::ExceptionObject &excp)
    {
        throw excp;
    }
    //Display the loaded Series
    DrawDICOMImg();
}


//Set the Path and Filename
void MyImageClass::SetFileName(QString mypath, QStringList myfiles)
{
    path_ = mypath;
    files_ = myfiles;
}

//Function to Display the loaded Series
void MyImageClass::DrawDICOMImg()
{
  //Connector_ connects the ITK Image to a VTK Image which can be displayed
  // with the VTK ImageViewer in a QVTK-Widget.
  connector_ = FilterType::New();

  connector_->SetInput(reader_->GetOutput());
  connector_->Update();

  imageViewerDCMSeriesX_->SetInputData(connector_->GetOutput());

  mMinSliderX_ = imageViewerDCMSeriesX_->GetSliceMin();
  mMaxSliderX_ = imageViewerDCMSeriesX_->GetSliceMax();

  //Check if the stored series is the Fixed series
  if(imreg_->stackedWidget->currentIndex() == 1)
  {
      imreg_->hSliderFixed->setMinimum(mMinSliderX_);
      imreg_->hSliderFixed->setMaximum(mMaxSliderX_);
      imageViewerDCMSeriesX_->SetRenderWindow(imreg_->vtkRenderer1->GetRenderWindow());
      imageViewerDCMSeriesX_->Render();
  }
  else
  {
      imreg_->hSliderMoving->setMinimum(mMinSliderX_);
      imreg_->hSliderMoving->setMaximum(mMaxSliderX_);
      imageViewerDCMSeriesX_->SetRenderWindow(imreg_->vtkRenderer2->GetRenderWindow());
      imageViewerDCMSeriesX_->Render();
  }
}

void MyImageClass::SetSlicePosition(int position)
{
    imageViewerDCMSeriesX_->SetSlice(position);;
    imageViewerDCMSeriesX_->Render();
}

//Identifies all Dicom Series in the specified folder and display them
void MyImageClass::GetDICOMSeries()
{
  gdcmIO_ = ImageIOType::New();
  namesGenerator_ = NamesGeneratorType::New();

  namesGenerator_->SetInputDirectory(path_.toLocal8Bit().data());

  std::vector<std::string > dcm_ids = namesGenerator_->GetSeriesUIDs();
  if(imreg_->stackedWidget->currentIndex() == 1)
  {
    imreg_->listWidget->clear();
    for(unsigned int i = 0; i < dcm_ids.size(); i++)
    {
      imreg_->listWidget->addItem(dcm_ids.at(i).c_str());
    }
  }
  else
  {
      imreg_->listWidgetMoving->clear();
      for(unsigned int i = 0; i < dcm_ids.size(); i++)
      {
        imreg_->listWidgetMoving->addItem(dcm_ids.at(i).c_str());
      }
  }
}

void MyImageClass::RedrawDICOMImg()
{
  imreg_->hSliderMoving->setMinimum(mMinSliderX_);
  imreg_->hSliderMoving->setMaximum(mMaxSliderX_);
  imageViewerDCMSeriesX_->Render();
  imreg_->vtkRenderer2->GetInteractor()->Render();
  imreg_->vtkRenderer2->update();
}

//Returns the Pointer to the Stored Dicom Series
ReaderType::Pointer MyImageClass::GetReader()
{
    return reader_;
}
itk::GDCMSeriesFileNames::Pointer MyImageClass::GetFileNames()
{
    return namesGenerator_;
}

void MyImageClass::SaveDICOMSeries(QString save_path, std::string subfolder)
{
    std::string output_directory = save_path.toStdString();
    output_directory +=subfolder;

    itksys::SystemTools::MakeDirectory( output_directory.c_str() );

    typedef signed short OutputPixelType;
    const unsigned int output_pixel_dimension = 2;

    typedef itk::Image < OutputPixelType, output_pixel_dimension > Image2Dtype;
    typedef itk::CastImageFilter<InternalImageType, ImageType> CastFilterType;

    typedef itk::ImageSeriesWriter < ImageType, Image2Dtype> SeriesWriterType;


    CastFilterType::Pointer caster = CastFilterType::New();
    SeriesWriterType::Pointer series_writer = SeriesWriterType::New();

    typedef itk::NumericSeriesFileNames NumericNamesGeneratorType;

    NumericNamesGeneratorType::Pointer namesGenerator = NumericNamesGeneratorType::New();

    itk::MetaDataDictionary &dict = gdcmIO_->GetMetaDataDictionary();
    std::string tagkey,value;
    tagkey = "0008|0060";
    value = "MR";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);
    tagkey = "0008|0008";
    value = "DERIVED\\SECONDARY";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);
    tagkey = "0008|0064";
    value = "DV";
    itk::EncapsulateMetaData<std::string>(dict,tagkey,value);

    caster->SetInput(reader_->GetOutput());
    series_writer->SetInput( caster->GetOutput());
    series_writer->SetImageIO(gdcmIO_);

    ImageType::RegionType region = reader_->GetOutput()->GetLargestPossibleRegion();
    ImageType::IndexType start = region.GetIndex();
    ImageType::SizeType size = region.GetSize();

    std::string format = output_directory;
    format += "/image%03d.dcm";

    namesGenerator->SetSeriesFormat(format.c_str());

    namesGenerator->SetStartIndex(start[2]);
    namesGenerator->SetEndIndex(start[2]+size[2]-1);
    namesGenerator->SetIncrementIndex(1);

    series_writer->SetFileNames (namesGenerator->GetFileNames());

    series_writer->SetMetaDataDictionaryArray(reader_->GetMetaDataDictionaryArray());

    try
    {
        series_writer->Update();
    }
    catch( itk::ExceptionObject &excp)
    {
        msg_box_.setText(excp.GetDescription());
        msg_box_.exec();
    }

}
