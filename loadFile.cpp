#include "loadFile.h"
#include "imageregistration.h"


LoadFile::LoadFile(ImageRegistration *imregistration):
    myimageregistration_(imregistration)
{


}

LoadFile::~LoadFile(){
}

QStringList LoadFile::GetFileName(){
    return file_names_;
}

QString LoadFile::GetPath(){
    return path_;
}

QString LoadFile::GetSavePath()
{
    return save_path_;
}

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

void LoadFile::SaveDirectoryPath()
{
    save_path_ = QFileDialog::getExistingDirectory(myimageregistration_,
                                                  tr("Select Directory"), "",
                                                  QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);
}



