  cmake_minimum_required(VERSION 2.8.8)
  project(image_registration)

  # Tell CMake to run moc when necessary:
  set(CMAKE_AUTOMOC ON)

  #set(CMAKE_AUTOUIC ON)
  #set(CMAKE_AUTORCC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  set(CMAKE_CXX_FLAGS "-std=c++11")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

  # Widgets finds its own dependencies.
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Gui REQUIRED)
  find_package(VTK REQUIRED)
  find_package(ITK REQUIRED)

  set(imreg_SOURCES imageregistration main.cpp imageregistration.cpp
                  loadFile.cpp imageregistration.h loadFile.h
                 itkImageToVTKImageFilter.txx itkVTKImageToImageFilter.txx itktypesandincludes.h
                 myexceptions.cpp myexceptions.h myimageclass.cpp myimageclass.h myregistration.cpp
                 myregistration.h registrationobserver.cpp myregistrationobserver.cpp registrationthread.cpp)

  set(image_registration_FORMS imageregistration.ui registrationobserver.ui)

  QT5_WRAP_UI(imreg_FORMS_HEADERS ${image_registration_FORMS})

  include(${ITK_USE_FILE})
  include(${VTK_USE_FILE})

  set_source_files_properties(${imreg_SOURCES} PROPERTIES OBJECT_DEPENDS "${imreg_FORMS_HEADERS}")


  add_executable(${imreg_SOURCES})

  target_link_libraries(imageregistration Qt5::Widgets Qt5::Core Qt5::Gui)
  target_link_libraries(imageregistration ${ITK_LIBRARIES} ${VTK_LIBRARIES})
