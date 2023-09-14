#-------------------------------------------------
#
# Project created by QtCreator 2022-10-03T22:22:25
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    form.cpp \
    drawxx.cpp \
    drawyy.cpp \
    drawzz.cpp \
    config.cpp \
    BLH_NEU.cpp \
    Find_best_satellite.cpp \
    Satellite_Position.cpp \
    Time_Transformation.cpp \
    Trop_Delay.cpp \
    XYZ_BLH.cpp \
    drawxy.cpp

HEADERS += \
        mainwindow.h \
    form.h \
    drawxx.h \
    drawyy.h \
    drawzz.h \
    config.h \
    File_Read.h \
    Function.h \
    drawxy.h

FORMS += \
        mainwindow.ui \
    form.ui \
    drawxx.ui \
    drawyy.ui \
    drawzz.ui \
    config.ui \
    drawxy.ui
INCLUDEPATH += E:\eigen-3.3.9

RESOURCES += \
    _resource.qrc
CONFIG += resources_big

DISTFILES += \
    icon.rc

RC_FILE += icon.rc
