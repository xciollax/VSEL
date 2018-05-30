#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T13:36:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VSEL
TEMPLATE = app

#Icon for macos
ICON = vsel.icns

#Icon for Windows
RC_ICONS = vsel.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#VSEL VERSION
DEFINES += VSEL_V='\\"0.2\\"'
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    abstractform.cpp \
    video.cpp \
    scene.cpp \
    createscenedialog.cpp \
    confirmdialog.cpp \
    scenemodel.cpp \
    sceneform.cpp \
    noteeditdelegate.cpp \
    blurtypedelegate.cpp \
    basepathform.cpp \
    installationhelper.cpp \
    sceneslistform.cpp \
    vsexception.cpp \
    mainwindow.cpp \
    splashform.cpp

HEADERS += \
        mainwindow.h \
    abstractform.h \
    video.h \
    scene.h \
    createscenedialog.h \
    confirmdialog.h \
    scenemodel.h \
    sceneform.h \
    noteeditdelegate.h \
    blurtypedelegate.h \
    basepathform.h \
    installationhelper.h \
    sceneslistform.h \
    vsexception.h \
    splashform.h

FORMS += \
        mainwindow.ui \
    formfilesystemlist.ui \
    createscenedialog.ui \
    confirmdialog.ui \
    sceneform.ui \
    basepathform.ui \
    splashform.ui

RESOURCES += \
    vsel.qrc

DISTFILES +=
