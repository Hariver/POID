QT += core
QT += gui

CONFIG += c++11

TARGET = FFT_TEST
CONFIG += console
CONFIG -= app_bundle

LIBS += -lfftw3

TEMPLATE = app

INCLUDEPATH += /usr/include/ImageMagick-6/
INCLUDEPATH +=  /usr/include/x86_64-linux-gnu/ImageMagick-6/

SOURCES += \
    main.cpp
