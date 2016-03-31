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
    QVector<double> yGreen(ySize), yRed(ySize), yBlue(ySize), yBrightness(ySize);

    //Inicjalizacja wektorów
    yRed.fill(0);
    yGreen.fill(0);
    yBlue.fill(0);
    yBrightness.fill(0);
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
            yRed[tempColor.red()] += 1.0;
            yGreen[tempColor.green()] += 1.0;
            yBlue[tempColor.blue()] += 1.0;
        }
    }

    //Jasność pikseli
    for(int i=0; i<=255; i++)
        yBrightness[i] = (yRed[i]*0.299 + yGreen[i]*0.587 + yBlue[i]*0.114);

    //Skalowanie histogramu do max 1!
    maxValue = yRed[0];

    for(int i=0; i <= 255; i++)
    {
        if(maxValue < yRed[i])
            maxValue = yRed[i];
        if(maxValue < yGreen[i])
            maxValue = yGreen[i];
        if(maxValue < yBlue[i])
            maxValue = yBlue[i];
    }

    for(int i=0; i <= 255; i++)
    {
        yRed[i] = yRed[i] / (double)maxValue;
        yGreen[i] = yGreen[i] / (double)maxValue;
        yBlue[i] = yBlue[i] / (double)maxValue;
        yBrightness[i] = yBrightness[i] / (double)maxValue;

    }

    //Rysowanie histogramu DO POPRAWY
    if(!myImage.allGray())
    {
        myPlot->clearGraphs();
        myPlot->addGraph();
        myPlot->graph(0)->setPen(QPen(Qt::red));
        myPlot->graph(0)->setData(xAxis, yRed);
        myPlot->addGraph();
        myPlot->graph(1)->setPen(QPen(Qt::green));
        myPlot->graph(1)->setData(xAxis, yGreen);
        myPlot->addGraph();
        myPlot->graph(2)->setPen(QPen(Qt::blue));
        myPlot->graph(2)->setData(xAxis, yBlue);
        myPlot->addGraph();
        myPlot->graph(3)->setPen(QPen(Qt::black));
        myPlot->graph(3)->setData(xAxis, yBrightness);
        myPlot->xAxis->setLabel("Poziom jasności.");
        myPlot->yAxis->setLabel("Poziom wypełnienia jasności.");
        myPlot->xAxis->setRange(-1,256);
        myPlot->yAxis->setRange(0,1.05);
        myPlot->replot();
    }
    else
    {
        myPlot->clearGraphs();
        myPlot->addGraph();
        myPlot->graph(0)->setPen(QPen(Qt::black));
        myPlot->graph(0)->setData(xAxis, yBrightness);
        myPlot->xAxis->setLabel("Poziom jasności.");
        myPlot->yAxis->setLabel("Poziom wypełnienia jasności.");
        myPlot->xAxis->setRange(-1,256);
        myPlot->yAxis->setRange(0,1.05);
        myPlot->replot();
    }
}

QImage lowPassFilter(QImage myImage, int size)
{
    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed = 0;
    int tempGreen = 0;
    int tempBlue = 0;

    //SetUp filter mask
    int filterDeno = 0;
    int filterSize = 2 * size + 1;
    int **filterMask = new int *[filterSize];
    for(int i=0; i < filterSize; i++)
        filterMask[i] = new int[filterSize];

    for(int i=0; i < filterSize; i++)
    {
        for(int j=0; j < filterSize; j++)
        {
            if(j < ((filterSize / 2) + 1) && i < ((filterSize / 2) + 1))
                filterMask[i][j] = (i + 1) * (j + 1);
            else if(j >= ((filterSize / 2) + 1) && i < ((filterSize / 2) + 1))
                filterMask[i][j] = filterMask[i][filterSize - j - 1];
            else if(j < ((filterSize / 2) + 1) && i >= ((filterSize / 2) + 1))
                filterMask[i][j] = filterMask[filterSize - i - 1][j];
            else
                filterMask[i][j] = filterMask[filterSize - i - 1][filterSize - j - 1];
            filterDeno += filterMask[i][j];
        }
    }

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
                   tempColorType = myImage.pixel(i+k,j+l);
                   tempColor.setRgb(tempColorType);
                   tempRed += filterMask[k + size][l + size] * tempColor.red();
                   tempGreen += filterMask[k + size][l + size] * tempColor.green();
                   tempBlue += filterMask[k + size][l + size] * tempColor.blue();
                   smallCounter++;
                }
            }

            //Obliczenie średnie arytmetyczniej dla każdego koloru
            tempRed = int((float)tempRed / (float)filterDeno);
            tempGreen = int((float)tempGreen / (float)filterDeno);
            tempBlue = int((float)tempBlue / (float)filterDeno);

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

    //Zwalnianie pamięci
    for(int i=0; i < filterSize; i++)
        delete [] filterMask[i];
    delete [] filterMask;
    filterMask = NULL;

    return myImage;
}

QImage operatorRobertsV2(QImage myImage)
{
    //Kolor
    QColor tempColor[4];
    QRgb tempColorType[4];
    int tempRed;
    int tempGreen;
    int tempBlue;

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);


    //Roberts
    for(int i=1; i<(myImage.width() - 1); i++)
    {
        for(int j=1; j<(myImage.height() - 1); j++)
        {
            tempColorType[0] = myImage.pixel(i, j);
            tempColorType[1] = myImage.pixel(i + 1, j + 1);
            tempColorType[2] = myImage.pixel(i + 1, j);
            tempColorType[3] = myImage.pixel(i, j + 1);
            tempColor[0].setRgb(tempColorType[0]);
            tempColor[1].setRgb(tempColorType[1]);
            tempColor[2].setRgb(tempColorType[2]);
            tempColor[3].setRgb(tempColorType[3]);
            tempRed = abs(tempColor[0].red() - tempColor[1].red()) + abs(tempColor[2].red() - tempColor[3].red());
            tempRed /= 2;
            tempGreen = abs(tempColor[0].green() - tempColor[1].green()) + abs(tempColor[2].green() - tempColor[3].green());
            tempGreen /= 2;
            tempBlue = abs(tempColor[0].blue() - tempColor[1].blue()) + abs(tempColor[2].blue() - tempColor[3].blue());
            tempBlue /= 2;

            //Debugowanie
            if(tempRed > 255 || tempGreen > 255 || tempBlue > 255)
            {
                qDebug() << "Dana x:" << i;
                qDebug() << "Dana y" << j;
                qDebug() << "Dana RED:" << tempRed << "| Param [0]:" << tempColor[0].red() << "| Param [1]:" << tempColor[1].red() << "| Param [2]:" << tempColor[2].red() << "| Param [3]:" << tempColor[3].red() ;
                qDebug() << "Dana GREEN:" << tempGreen;
                qDebug() << "Dana BLUE:" << tempBlue;
            }

            //Przypisanie nowego piksela.
            tempColor[0].setRgb(tempRed, tempGreen, tempBlue);
            tempColorType[0] = tempColor[0].rgb();
            myImage.setPixel(i, j, tempColorType[0]);
        }
    }
    return myImage;
}

QImage distributionRayleigh(QImage myImage, int gMin, double alfa)
{
    //Kolor
    QColor tempColor;
    QRgb tempColorType;
    int tempRed;
    int tempGreen;
    int tempBlue;

    //Deklaracja tablicy LUT (referencyjnej)
    int myLUT[3][256];
    float resultLUT[3][256];

    //Definicje zmiennych
    int xSize = 256;
    int ySize = 256;
    long allPixels = myImage.width() * myImage.height();
    float cumulativeDistribution[256];
    QVector<double> xAxis(xSize);
    QVector<double> yGreen(ySize), yRed(ySize), yBlue(ySize), yBrightness(ySize);

    //Inicjalizacja wektorów
    yRed.fill(0);
    yGreen.fill(0);
    yBlue.fill(0);
    yBrightness.fill(0);
    for(int i=0; i < xSize; i++)
    {
        xAxis[i] = i;
    }

    //Sprawdzenie formatu
    if(myImage.format() != QImage::Format_RGB32)
        myImage = myImage.convertToFormat(QImage::Format_RGB32);

    //Zczytanie ilości kolorów
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            yRed[tempColor.red()] += 1.0;
            yGreen[tempColor.green()] += 1.0;
            yBlue[tempColor.blue()] += 1.0;
        }
    }

    //Wypełnienie tablicy LUT (RED) - inicjalizacja
    cumulativeDistribution[0] = 0;
    resultLUT[0][0] = 0;

    //Wypełnienie tablicy LUT (RED)
    for(int i=1; i<=255; i++)
    {
        //Dystrybuanta
        cumulativeDistribution[i] = cumulativeDistribution[i-1] + (float)yRed[i];
        if(cumulativeDistribution[i] != 0.0)
            resultLUT[0][i] = float(gMin + pow(2*(alfa*alfa)*log(pow(cumulativeDistribution[i] / (float)allPixels, -1)), 0.5));
        else
            resultLUT[0][i] = 0;

        if(abs(resultLUT[0][i] - resultLUT[0][i - 1]) * allPixels < 0)
            myLUT[0][i] = 0;
        else if(abs(resultLUT[0][i] - resultLUT[0][i - 1]) * allPixels > 255)
            myLUT[0][i] = 255;
        else
            myLUT[0][i] = abs(resultLUT[0][i] - resultLUT[0][i - 1]) * allPixels;

        //Debug
        qDebug() << "CZERWONY! Jasność:"<< i << ". Ilość pikseli (baza):" << yRed[i] << ". Wartość dystrybuanty:" << cumulativeDistribution[i];
        qDebug() << "--------  Logarytm:" << log(pow(cumulativeDistribution[i] / (float)allPixels, -1));
        qDebug() << "--------  Wynik równania:" << resultLUT[0][i];
        qDebug() << "--------  Wartość piksela:" << myLUT[0][i];
        qDebug() << "--------  Testowa:" << abs(resultLUT[0][i] - resultLUT[0][i - 1])*allPixels;
        qDebug() << " ";
    }

    //Wypełnienie tablicy LUT (GREEN) - inicjalizacja
    cumulativeDistribution[0] = 0;
    resultLUT[1][0] = 0;

    //Wypełnienie tablicy LUT (GREEN)
    for(int i=1; i<=255; i++)
    {
        //Dystrybuanta
        cumulativeDistribution[i] = cumulativeDistribution[i-1] + (float)yGreen[i];
        if(cumulativeDistribution[i] != 0.0)
            resultLUT[1][i] = float(gMin + pow(2*(alfa*alfa)*log(pow(cumulativeDistribution[i] / (float)allPixels, -1)), 0.5));
        else
            resultLUT[1][i] = 0;

        if(abs(resultLUT[1][i] - resultLUT[1][i - 1]) * allPixels < 0)
            myLUT[1][i] = 0;
        else if(abs(resultLUT[1][i] - resultLUT[1][i - 1]) * allPixels > 255)
            myLUT[1][i] = 255;
        else
            myLUT[1][i] = abs(resultLUT[1][i] - resultLUT[1][i - 1]) * allPixels;
    }

    //Wypełnienie tablicy LUT (BLUE) - inicjalizacja
    cumulativeDistribution[0] = 0;
    resultLUT[2][0] = 0;

    //Wypełnienie tablicy LUT (BLUE)
    for(int i=1; i<=255; i++)
    {
        //Dystrybuanta
        cumulativeDistribution[i] = cumulativeDistribution[i-1] + (float)yGreen[i];
        if(cumulativeDistribution[i] != 0.0)
            resultLUT[2][i] = float(gMin + pow(2*(alfa*alfa)*log(pow(cumulativeDistribution[i] / (float)allPixels, -1)), 0.5));
        else
            resultLUT[2][i] = 0;

        if(abs(resultLUT[2][i] - resultLUT[2][i - 1]) * allPixels < 0)
            myLUT[2][i] = 0;
        else if(abs(resultLUT[1][i] - resultLUT[2][i - 1]) * allPixels > 255)
            myLUT[2][i] = 255;
        else
            myLUT[2][i] = abs(resultLUT[2][i] - resultLUT[2][i - 1]) * allPixels;
    }

    //Przypisanie nowych wartości po wyrównaniu histogramu
    for(int i=0; i<myImage.width(); i++)
    {
        for(int j=0; j<myImage.height(); j++)
        {
            tempColorType = myImage.pixel(i,j);
            tempColor.setRgb(tempColorType);
            tempRed = myLUT[0][tempColor.red()];
            tempGreen = myLUT[1][tempColor.green()];
            tempBlue = myLUT[2][tempColor.blue()];

            //Przypisanie nowego piksela.
            tempColor.setRgb(tempRed, tempGreen, tempBlue);
            tempColorType = tempColor.rgb();
            myImage.setPixel(i, j, tempColorType);
        }
    }
    return myImage;
}
