#-------------------------------------------------
#
# Project created by QtCreator 2020-11-12T16:48:23
#
#-------------------------------------------------

QT       += core gui serialport
QT       += charts

VERSION = 1.0.0

QMAKE_TARGET_PRODUCT="MDP_M01"
QMAKE_TARGET_COMPANY = "e_Design"
QMAKE_TARGET_DESCRIPTION = ""
QMAKE_TARGET_COPYRIGHT = "MDP_M01_Upper_Computer"


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MDP_M01
TEMPLATE = app

TRANSLATIONS = ui.ts

RC_ICONS = app.ico
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 resources_big

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    threadcom.cpp \
    processingdata.cpp \
    machine.cpp \
    MyChartView.cpp \
    switchcom.cpp \
    automatchexplain.cpp

HEADERS += \
        mainwindow.h \
    threadcom.h \
    processingdata.h \
    machine.h \
    MyChartView.h \
    switchcom.h \
    automatchexplain.h

FORMS += \
        mainwindow.ui \
    switchcom.ui \
    automatchexplain.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
