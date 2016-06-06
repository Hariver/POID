#-------------------------------------------------
#
# Project created by QtCreator 2016-05-19T22:22:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Frontend
TEMPLATE = app

LIBS += -lfftw3

SOURCES += main.cpp\
        mainwindow.cpp \
    myfft.cpp \
    mysegmentation.cpp \
    my_qlabel.cpp

HEADERS  += mainwindow.h \
    myfft.h \
    mysegmentation.h \
    my_qlabel.h

FORMS    += mainwindow.ui

RESOURCES += \
    image.qrc
