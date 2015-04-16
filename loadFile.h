#ifndef LOADFILE_H
#define LOADFILE_H

#include <QtWidgets>
#include <QFileDialog>
#include <QStringList>


class ImageRegistration;
/*!
 * \brief This class handles the file IO.
 * The class LoadFile handles all user interaction to specify the location where to load and save files.
 * In order to do this, it opens Qt file dialogs. The specified locations are stored in member variables and
 * are accessable through "get" functions.
 */
class LoadFile : public QWidget{
        Q_OBJECT
         /*!
         * \brief myimageregistration_ pointer to the main class ImageRegistration.
         */
        ImageRegistration *myimageregistration_;
        public:
                /*!
                 * \brief LoadFile Constructor
                 */
                LoadFile (ImageRegistration*);
                /*!
                 * \brief GetPath Retruns the Path where the files are stored.
                */
                QString GetPath(void);
                /*!
                 * \brief GetFileName Returns the filenames.
                 */
                QStringList GetFileName(void);
                /*!
                 * \brief GetSavePath Returns the Path to the save directory.
                 */
                QString GetSavePath(void);
                ~LoadFile();
                /*!
                 * \brief LoadFileNames Opens a file dialog for choosing the files.
                 */
                void LoadFileNames();
                /*!
                 * \brief SaveDirectoryPath Opens a file dialog for choosing the save directory.
                 */
                void SaveDirectoryPath();

        private:
                QStringList file_names_;
                QString path_;
                QString save_path_;


};

#endif // LOADFILE_H
