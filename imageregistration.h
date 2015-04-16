#ifndef IMAGEREGISTRATION_H
#define IMAGEREGISTRATION_H

#include "ui_imageregistration.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QVTKWidget.h>
#include <vector>
#include <memory>


#define FIXED 1
#define MOVING 2


class MyRegistration;
class LoadFile;
class MyImageClass;
class RegistrationObserver;
class RegistrationThread;

/*!
 * \brief Controls all user interaction with the UI and starts registration accordingly.
 *  This class handles all user input, controls the UI and starts the registration.
 *  Furthermore it takes care of the threads, the registration runs in.
 */


class ImageRegistration : public QMainWindow, public Ui::mainWindow
{
    Q_OBJECT

public:
    /*!
     * Initialises all UI windows and connects the signals from the UI to functions.
     * \param parent QMainWindow Pointer
     *
     */
    explicit ImageRegistration(QMainWindow *parent = 0);
    ~ImageRegistration();
    /*!
     * \brief GetObserverWindow enables other classes to use the observer window.
     * \return Unique Pointer to the observer window.
     */
    std::unique_ptr<RegistrationObserver>* GetObserverWindow();
    
private:
    /*!
     * \brief fixed_image_ holding the fixed image series.
     */
    MyImageClass *fixed_image_;
    /*!
     * \brief moving_image_vec_ containing all moving image series.
     */
    std::vector<std::unique_ptr<MyImageClass>> moving_image_vec_;
    /*!
     * \brief msg_box_ a QMessageBox to display Error messages.
     */
    QMessageBox msg_box_;
    /*!
     * \brief load_files_hdd_ object handling the file input.
     */
    LoadFile *load_files_hdd_;
    /*!
     * \brief registration_ object handling the registration and file save.
     */
    std::vector<std::unique_ptr<MyRegistration>> registration_;
    /*!
     * \brief regobs_window_ pointer containing the Observer Window.
     */
    std::unique_ptr<RegistrationObserver> regobs_window_;
    /*!
     * \brief reg_thread_ pointer containing the registration thread.
     */
    std::unique_ptr<RegistrationThread> reg_thread_;
    /*!
     * \brief selected_series_ the currently selected moving series.
     */
    int selected_series_;


private slots:
    /*!
     * \brief ShowFileLoad shows the dialog for file input.
     */
    void ShowFileLoad();
    /*!
     * \brief ShowMoving shows the dialog for selecting the moving series.
     */
    void ShowMoving();
    /*!
     * \brief ShowComputing shows the dialog for visualy compare the loaded series.
     */
    void ShowComputing();
    /*!
     * \brief SliderMovedFixed slider for selecting an image in the fixed series.
     * \param position an integer representing the selected image.
     */
    void SliderMovedFixed(int position);
    /*!
     * \brief SliderMovedMoving slider for selecting an image in the moving series.
     * \param position an integer representing the selected image.
     */
    void SliderMovedMoving(int position);
    /*!
     * \brief SaveFiles opens the dialog to save the registrated files.
     */
    void SaveFiles();
    /*!
     * \brief StartRegistration starts the registration process and displays the observer window.
     */
    void StartRegistration();
    /*!
     * \brief ShowResultingFit shows the final registration by the means of a difference image.
     */
    void ShowResultingFit();
    /*!
     * \brief SliderMovedResult selects the image to be displayed in the result depending on the position of the slider.
     */
    void SliderMovedResult(int);
    /*!
     * \brief AddMovingSeries adds (another) moving series to the moving series vector.
     */
    void AddMovingSeries();
    /*!
     * \brief SelectMovingSeries selects a moving series depending on the position of the slider.
     */
    void SelectMovingSeries(int);
    /*!
     * \brief SliderObserverMoved slider for selecting an image in the observed series.
     * \param position an integer representing the selected image.
     */
    void SliderObserverMoved(int position);
    /*!
     * \brief MovingSeriesObserverSelected sets the observed series depending on the selected one.
     * \param number the selected series.
     */
    void MovingSeriesObserverSelected(int number);

};

#endif // IMAGEREGISTRATION_H
