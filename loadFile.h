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
/*!
 * \brief Handles user interaction to specify file IO locations.
 * The class LoadFile handles all user interaction to specify the location where to load and save files.
 * In order to do this, it opens Qt file dialogs. The specified locations are stored in member variables and
 * are accessable through "get" functions.
 */
class LoadFile : public QWidget{
        Q_OBJECT
        public:
                /*!
                 * \brief constructor
                 */
                LoadFile (ImageRegistration*);
                /*!
                 * \brief returns the path where the files are stored.
                */
                QString GetPath(void);
                /*!
                 * \brief returns the filenames.
                 */
                QStringList GetFileName(void);
                /*!
                 * \brief returns the path to the save directory.
                 */
                QString GetSavePath(void);
                ~LoadFile();
                /*!
                 * \brief opens a file dialog for choosing the files.
                 */
                void LoadFileNames();
                /*!
                 * \brief opens a file dialog for choosing the save directory.
                 */
                void SaveDirectoryPath();

        private:
                /*!
                * \brief myimageregistration_ pointer to the main ImageRegistration object.
                */
                ImageRegistration *myimageregistration_;
                QStringList file_names_;
                QString path_;
                QString save_path_;


};

#endif // LOADFILE_H
