QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    RacheFile/RByteSet/RByteSet.cpp \
    RacheFile/RByteSet/RByteSetFile.cpp \
    RacheFile/RByteSet/RBytes.cpp \
    RacheFile/FileDocument.cpp \
    RacheFile/FileItemGroup.cpp \
    RacheFile/FileItemObject.cpp \
    RacheFile/DocumentSearcher.cpp \
    RacheFile/ResourceSet.cpp \
    RacheFile/SubFile.cpp \
    Viewer/SubFileViewer.cpp \
    Viewer/ViewerWidgetSet.cpp \
    Viewer/ResourceViewer.cpp \
    Dialog/ItemEditor.cpp \
    Dialog/OrderListWidget.cpp \
    Dialog/StartUI.cpp \
    Dialog/FileDocumentTree.cpp \
    Dialog/Editor.cpp \
    Dialog/FindDialog.cpp \
    Dialog/ResourceDialog.cpp \
    Dialog/ResourceViewerDialog.cpp \
    ToolFunction.cpp \
    Widget/EditableLabel.cpp


HEADERS += \
    mainwindow.h \
    RacheFile/RByteSet/RBytes.h \
    RacheFile/RByteSet/RByteSetFile.h \
    RacheFile/RByteSet/RByteSet.h \
    RacheFile/FileDocument.h \
    RacheFile/FileItemGroup.h \
    RacheFile/FileItemObject.h \
    RacheFile/DocumentSearcher.h \
    RacheFile/SubFile.h \
    Viewer/SubFileViewer.h \
    Viewer/ViewerWidgetSet.h \
    Viewer/ResourceViewer.h \
    Dialog/ItemEditor.h \
    Dialog/OrderListWidget.h \
    Dialog/StartUI.h \
    Dialog/Editor.h \
    Dialog/FileDocumentTree.h \
    Dialog/FindDialog.h \
    Dialog/ResourceDialog.h \
    Dialog/ResourceViewerDialog.h \
    ToolFunction.h \
    globals.h \
    Widget/EditableLabel.h

TRANSLATIONS += \
    RacheFile_zh_CN.ts \
    RacheFile_zh_HK.ts \
    RacheFile_en_US.ts

RC_ICONS = resource\icon\Icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
