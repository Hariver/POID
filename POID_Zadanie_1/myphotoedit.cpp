#include <QImage>
#include <QColor>

#include "myphotoedit.h"
#include "algorytmy.h"
#include "qcustomplot.h"


QImage changeBrightness(QImage myImage, int myBrightness)
{
    //Deklaracja tablicy LUT (referencyjnej)
    int myLUT[256];
    int resultLUT;

    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed;
    int tempGreen;
    int tempBlue;

    //Wypełnienie tablicy LUT
    for(int i=0; i<=255; i++)
    {
        resultLUT = i + myBrightness;

        if(resultLUT < 0)
            myLUT[i] = 0;
        else if(resultLUT > 255)
            myLUT[i] = 255;
        else
            myLUT[i] = resultLUT;
    }

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Zmiana jasności w pętli
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            tempRed = myLUT[tempColor.red()];
            tempGreen = myLUT[tempColor.green()];
            tempBlue = myLUT[tempColor.blue()];

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRed, tempGreen, tempBlue);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);
        }
    }
    return myImage;
}

QImage changeContrast(QImage myImage, float myContrast)
{
    //Deklaracja tablicy LUT (referencyjnej)
    int myLUT[256];
    int resultLUT;

    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed;
    int tempGreen;
    int tempBlue;

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Wypełnienie tablicy LUT
    for(int i=0; i<=255; i++)
    {
        resultLUT = int(myContrast * ((float)i - 127.0) + 127.0);

        if(resultLUT < 0)
            myLUT[i] = 0;
        else if(resultLUT > 255)
            myLUT[i] = 255;
        else
            myLUT[i] = resultLUT;
    }

    //Zmiana kontrastu w pętli
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            tempRed = myLUT[tempColor.red()];
            tempGreen = myLUT[tempColor.green()];
            tempBlue = myLUT[tempColor.blue()];

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRed, tempGreen, tempBlue);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);
        }
    }
    return myImage;
}

QImage negativeImage(QImage myImage)
{
    //Deklaracja tablicy LUT (referencyjnej)
    int myLUT[256];
    int resultLUT;

    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed;
    int tempGreen;
    int tempBlue;

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Wypełnienie tablicy LUT
    for(int i=0; i<=255; i++)
    {
        resultLUT = 255 - i;
        myLUT[i] = resultLUT;
    }

    //Negacja w pętli
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            tempRed = myLUT[tempColor.red()];
            tempGreen = myLUT[tempColor.green()];
            tempBlue = myLUT[tempColor.blue()];

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRed, tempGreen, tempBlue);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);
        }
    }
    return myImage;
}

QImage arithmeticMeanFilter(QImage myImage, int size)
{
    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed = 0;
    int tempGreen = 0;
    int tempBlue = 0;

    //Zmienne pomocnicze
    int smallCounter = 0;

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Filtr w pętli
    for(int i=size; i<(myImage.width()-size); i++)
    {
        for(int j=size; j<(myImage.height()-size); j++)
        {
            //Obliczanie średniej arytmetycznej
            for(int k=(-size); k <= size; k++)
            {
                for(int l=(-size); l <= size; l++)
                {
                   if(!(k == 0 && l == 0))
                   {
                   tempColorType = myImage.pixel(i+k,j+l);
                   tempColor.setRgb(tempColorType);
                   tempRed += tempColor.red();
                   tempGreen += tempColor.green();
                   tempBlue += tempColor.blue();
                   smallCounter++;
                   }
                }
            }

            //Obliczenie średnie arytmetyczniej dla każdego koloru
            tempRed = int((float)tempRed / (float)smallCounter);
            tempGreen = int((float)tempGreen / (float)smallCounter);
            tempBlue = int((float)tempBlue / (float)smallCounter);

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRed, tempGreen, tempBlue);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);

            //Reset danych
            smallCounter = 0;
            tempRed = 0;
            tempGreen = 0;
            tempBlue = 0;
        }
    }
    return myImage;
}

QImage medianFilter(QImage myImage, int size)
{
    //Ilosc pikseli w obwodzie
    int arraySize = (1+2*size)*(1+2*size)-1;

    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int *tempRed = new int[arraySize];
    int *tempGreen = new int[arraySize];
    int *tempBlue = new int[arraySize];
    int tempRedMedian = 0;
    int tempGreenMedian = 0;
    int tempBlueMedian = 0;

    //Zmienne pomocnicze
    int smallCounter = 0;

    //ONLY DEBUG VARIABLE
    int debugRed = 0;

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Filtr w pętli
    for(int i=size; i<(myImage.width()-size); i++)
    {
        for(int j=size; j<(myImage.height()-size); j++)
        {
            //Obliczanie średniej arytmetycznej
            for(int k=(-size); k <= size; k++)
            {
                for(int l=(-size); l <= size; l++)
                {
                   if(!(k == 0 && l == 0))
                   {
                   tempColorType = myImage.pixel(i+k,j+l);
                   tempColor.setRgb(tempColorType);

                   //ONLY DEBUG VARIABLE
                   debugRed = tempColor.red();

                   tempRed[smallCounter] = tempColor.red();
                   tempGreen[smallCounter] = tempColor.green();
                   tempBlue[smallCounter] = tempColor.blue();
                   smallCounter++;
                   }
                }
            }

            //Obliczenie średnie arytmetyczniej dla każdego koloru
            tempRedMedian = median(tempRed, arraySize);
            tempGreenMedian = median(tempGreen, arraySize);
            tempBlueMedian = median(tempBlue, arraySize);

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRedMedian, tempGreenMedian, tempBlueMedian);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);

            //Reset danych
            smallCounter = 0;
        }
    }
    return myImage;
}

void plotHistogram(QImage myImage, QCustomPlot *myPlot)
{
    //Kolor
    QColor tempColor;
    QRgb tempColorType;

    //Definicje zmiennych
    int xSize = 256;
    int ySize = 256;
    int maxValue;
    QVector<double> xAxis(xSize);
    QVector<double> yGreen(ySize), yRed(ySize), yBlue(ySize);

    //Inicjalizacja wektorów
    yRed.fill(0);
    yGreen.fill(0);
    yBlue.fill(0);
    for(int i=0; i < xSize; i++)
    {
        xAxis[i] = i;
    }

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Zczytanie ilości kolorów do narysowania histogramu
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            yRed[tempColor.red()] += 1;
            yGreen[tempColor.green()] += 1;
            yBlue[tempColor.blue()] += 1;
        }
    }
    //Skalowanie histogramu do max 1!


    //Rysowanie histogramu DO POPRAWY
    myPlot->addGraph();
    myPlot->graph(0)->setData(xAxis, yRed);
    myPlot->addGraph();
    myPlot->graph(1)->setData(xAxis, yGreen);
    myPlot->xAxis->setRange(0,256);
    myPlot->yAxis->setRange(0,3000);
    myPlot->replot();
}
