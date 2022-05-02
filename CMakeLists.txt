cmake_minimum_required(VERSION 3.16)
project(RacheFile)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-D_CMAKE)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)

set(QT_USE_QTMULTIMEDIA TRUE)

add_executable(RacheFile main.cpp mainwindow.h mainwindow.cpp RacheFile/FileItemObject.cpp RacheFile/FileItemObject.h RacheFile/FileItemGroup.cpp RacheFile/FileItemGroup.h RacheFile/SubFile.cpp RacheFile/SubFile.h RacheFile/FileDocument.cpp RacheFile/FileDocument.h Viewer/SubFileViewer.cpp Viewer/SubFileViewer.h Dialog/ItemEditor.cpp Dialog/ItemEditor.h Dialog/StartUI.cpp Dialog/StartUI.h Dialog/OrderListWidget.cpp Dialog/OrderListWidget.h Dialog/Editor.cpp Dialog/Editor.h RacheFile/RByteSet/RByteSet.cpp RacheFile/RByteSet/RByteSet.h RacheFile/RByteSet/RBytes.cpp RacheFile/RByteSet/RBytes.h RacheFile/RByteSet/md5/md5.h RacheFile/RByteSet/md5/md5.cpp Viewer/ViewerWidgetSet.cpp Viewer/ViewerWidgetSet.h globals.h globals.h ToolFunction.cpp ToolFunction.h Dialog/FileDocumentTree.cpp Dialog/FileDocumentTree.h Dialog/FindDialog.cpp Dialog/FindDialog.h RacheFile/DocumentSearcher.cpp RacheFile/DocumentSearcher.h RacheFile/ResourceSet.cpp RacheFile/ResourceSet.h Dialog/ResourceDialog.cpp Dialog/ResourceDialog.h init.h Viewer/ResourceViewer.cpp Viewer/ResourceViewer.h Dialog/ResourceViewerDialog.cpp Dialog/ResourceViewerDialog.h RacheFile/RByteSet/RByteSetFile.cpp RacheFile/RByteSet/RByteSetFile.h RacheFile/RByteSet/test.h Dialog/ProgressBarDialog.cpp Dialog/ProgressBarDialog.h Widget/EditableLabel.cpp Widget/EditableLabel.h)
target_link_libraries(RacheFile Qt5::Widgets Qt5::Multimedia Qt5::MultimediaWidgets)