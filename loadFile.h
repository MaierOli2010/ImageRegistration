#ifndef LOADFILE_H
#define LOADFILE_H


#if MY_QT_VERSION == 4
    #include <QtGui/QWidget>
#else
   #include <QtWidgets>
#endif

#include <QFileDialog>
#include <QStringList>


class ImageRegistration;

class LoadFile : public QWidget{
        Q_OBJECT
        ImageRegistration *myimageregistration_;
        public:
                LoadFile (ImageRegistration*);
                QString GetPath(void);
                QStringList GetFileName(void);
                QString GetSavePath(void);
                ~LoadFile();
                void LoadFileNames();
                void SaveDirectoryPath();

        private:
                QStringList file_names_;
                QString path_;
                QString save_path_;


};

#endif // LOADFILE_H
