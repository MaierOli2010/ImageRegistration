  cmake_minimum_required(VERSION 2.8.8)
  project(image_registration)

  # Tell CMake to run moc when necessary:
  set(CMAKE_AUTOMOC ON)

  #set(CMAKE_AUTOUIC ON)
  #set(CMAKE_AUTORCC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
  set(CMAKE_INCLUDE_CURRENT_DIR ON)


  set(CMAKE_CXX_FLAGS "-std=gnu++11 -fpermissive")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")


  ##### USE QT5 as default change to 4 in cmake gui or here to use QT4 #####
  set(QT_VERSION 5 CACHE STRING "Please set the Qt version to use(4/5):")


  if(${QT_VERSION} EQUAL 5)
  # Widgets finds its own dependencies.
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)

  elseif(${QT_VERSION} EQUAL 4)
    find_package(Qt4 REQUIRED QtCore QtGui)
    set(MY_QT_VERSION 4)
    add_definitions(-DMY_QT_VERSION=${MY_QT_VERSION})
    include_directories(${QT_INCLUDES})

  endif(${QT_VERSION} EQUAL 5)


  find_package(VTK REQUIRED)
  find_package(ITK REQUIRED)

  set(imreg_SOURCES imageregistration main.cpp imageregistration.cpp
                  loadFile.cpp imageregistration.h loadFile.h
                 itkImageToVTKImageFilter.txx itkVTKImageToImageFilter.txx itktypesandincludes.h
                 myimageclass.cpp myimageclass.h myregistration.cpp
                 myregistration.h registrationobserver.cpp myregistrationobserver.cpp registrationthread.cpp)

  set(image_registration_FORMS imageregistration.ui registrationobserver.ui)


  if(${QT_VERSION} EQUAL 5)
    QT5_WRAP_UI(imreg_FORMS_HEADERS ${image_registration_FORMS})

  elseif(${QT_VERSION} EQUAL 4)
    QT4_WRAP_UI(imreg_FORMS_HEADERS ${image_registration_FORMS})

  endif(${QT_VERSION} EQUAL 5)


  include(${ITK_USE_FILE})
  include(${VTK_USE_FILE})

  set_source_files_properties(${imreg_SOURCES} PROPERTIES OBJECT_DEPENDS "${imreg_FORMS_HEADERS}")


  add_executable(${imreg_SOURCES})

  if(${QT_VERSION} EQUAL 5)
    target_link_libraries(imageregistration Qt5::Widgets Qt5::Core Qt5::Gui)

  elseif(${QT_VERSION} EQUAL 4)
    target_link_libraries(imageregistration ${QT_LIBRARIES})

  endif(${QT_VERSION} EQUAL 5)

  target_link_libraries(imageregistration ${ITK_LIBRARIES} ${VTK_LIBRARIES})



