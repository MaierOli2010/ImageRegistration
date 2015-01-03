#include "loadFile.h"
#include "imageregistration.h"


LoadFile::LoadFile(ImageRegistration *imregistration):
    myimageregistration_(imregistration)
{


}

LoadFile::~LoadFile(){
}
//Returns the names of the selected files
QStringList LoadFile::GetFileName(){
    return file_names_;
}
//Returns the Path of the selected files
QString LoadFile::GetPath(){
    return path_;
}
//Returns the Path to the folder for saving the files
QString LoadFile::GetSavePath()
{
    return save_path_;
}
//Opens the Filedialog. Choosing multiple files and the file path
void LoadFile::LoadFileNames(){
    file_names_ = QFileDialog::getOpenFileNames(myimageregistration_, tr("Open File"),
                                         "",
                                         tr("Files (*.*)"));
    if(file_names_.isEmpty() == false)
    {
        QFileInfo dcm_info = QFileInfo(file_names_.at(0));
        path_ = dcm_info.absolutePath();
    }

}
//Specifies the path for saving the files
void LoadFile::SaveDirectoryPath()
{
    save_path_ = QFileDialog::getExistingDirectory(myimageregistration_,
                                                  tr("Select Directory"), "",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
}



