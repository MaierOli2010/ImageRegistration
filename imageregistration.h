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
class registrationObserver;
class RegistrationThread;

/*!
 * \brief This is the main class of the Registration Programm.
 *  This class handles all user input, controls the UI and starts the registration.
 *  Furthermore it takes care of the threads, the registration runs in.
 */


class ImageRegistration : public QMainWindow, public Ui::mainWindow
{
    Q_OBJECT

public:
    /*!
     * \brief ImageRegistration Constructor
     * \param parent QMainWindow Pointer
     * This constructor initialises all Ui windows and connects the signals from the UI to functions.
     */
    explicit ImageRegistration(QMainWindow *parent = 0);
    ~ImageRegistration();
    /*!
     * \brief GetObserverWindow Enables other classes to use the observer window.
     * \return Unique Pointer pointing to the Observer Window.
     */
    std::unique_ptr<registrationObserver>* GetObserverWindow();
    
private:
    /*!
     * \brief fixed_image_ Holding the fixed image series.
     */
    MyImageClass *fixed_image_;
    /*!
     * \brief moving_image_vec_ Vector containing all moving image series.
     */
    std::vector<std::unique_ptr<MyImageClass>> moving_image_vec_;
    /*!
     * \brief msg_box_ A QMessageBox to display Error messages.
     */
    QMessageBox msg_box_;
    /*!
     * \brief load_files_hdd_ Object handling the file input.
     */
    LoadFile *load_files_hdd_;
    /*!
     * \brief registration_ Object handling the registration and file save.
     */
    std::vector<std::unique_ptr<MyRegistration>> registration_;
    /*!
     * \brief regobs_window_ Pointer containing the Observer Window.
     */
    std::unique_ptr<registrationObserver> regobs_window_;
    /*!
     * \brief reg_thread_ Pointer containing the registration thread.
     */
    std::unique_ptr<RegistrationThread> reg_thread_;
    /*!
     * \brief selected_series_ The currently selected moving series.
     */
    int selected_series_;


private slots:
    /*!
     * \brief ShowFileLoad Shows the dialog for file input.
     */
    void ShowFileLoad();
    /*!
     * \brief ShowMoving Shows the dialog for selecting the moving series.
     */
    void ShowMoving();
    /*!
     * \brief ShowComputing Shows the dialog for visualy compare the loaded series.
     */
    void ShowComputing();
    /*!
     * \brief SliderMovedFixed Slider for selecting an image in the fixed series.
     * \param position Returning an integer representing the selected image.
     */
    void SliderMovedFixed(int position);
    /*!
     * \brief SliderMovedMoving  Slider for selecting an image in the moving series.
     * \param position Returning an integer representing the selected image.
     */
    void SliderMovedMoving(int position);
    /*!
     * \brief SaveFiles Opens the dialog to save the registrated files.
     */
    void SaveFiles();
    /*!
     * \brief StartRegistration Starts the registration process and displays the observer window.
     */
    void StartRegistration();
    /*!
     * \brief ShowResultingFit Shows the final registration by the means of a difference image.
     */
    void ShowResultingFit();
    /*!
     * \brief SliderMovedResult Selects the image to be displayed in the result depending on the position of the slider.
     */
    void SliderMovedResult(int);
    /*!
     * \brief AddMovingSeries Adds another moving series to the moving series vector.
     */
    void AddMovingSeries();
    /*!
     * \brief SelectMovingSeries Selects a moving series depending on the position of the slider.
     */
    void SelectMovingSeries(int);
    /*!
     * \brief SliderObserverMoved Slider for selecting an image in the observed series.
     * \param position Returning an integer representing the selected image.
     */
    void SliderObserverMoved(int position);
    /*!
     * \brief MovingSeriesObserverSelected Sets the observed series depending on the selected one.
     * \param number The selected series.
     */
    void MovingSeriesObserverSelected(int number);

};

#endif // IMAGEREGISTRATION_H
