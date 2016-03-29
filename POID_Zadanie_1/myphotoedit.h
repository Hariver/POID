#ifndef MYPHOTOEDIT_H
#define MYPHOTOEDIT_H

#include "qcustomplot.h"

#include <QImage>
#include <QProgressBar>

QImage changeBrightness(QImage myImage, int myBrightness);
QImage changeContrast(QImage myImage, float myContrast);
QImage negativeImage(QImage myImage);
QImage arithmeticMeanFilter(QImage myImage, int size = 1);
QImage medianFilter(QImage myImage, int size = 1);
void plotHistogram(QImage myImage, QCustomPlot *myPlot);
QImage lowPassFilter(QImage myImage, int size);

#endif MYPHOTOEDIT_H
