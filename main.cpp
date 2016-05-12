#include <fftw3.h>
#include <math.h>
#include <complex>
#include <iostream>
#include <QString>
#include <QImage>
#include <QDebug>

using namespace std;

void fft(int squareSize, QImage *myImage, QImage *myImageMag, QImage *myImagePhase);

void swapQuadrants(int squareSize, QImage *myImage);

void ift(int squareSize, QImage *myImageMag, QImage *myImagePhase, QImage *myImageOut);

int main(int argc, char** argv)
{
//    if(argc < 4) {
//            cout << "usage:\t" << argv[0] << " in outMag outPhase" << endl
//                    << "\t" << argv[0] << " -ift inMag inPhase out" << endl;
//            return 0;
//        }

    //Deklaracja zmiennych
    QImage myImage, myImageMag, myImagePhase, myImageOut;
    QString myImagePath = "/home/adampelc/Downloads/in.png";
    int squareSize;

//    ***************************
//    FORWARD FOURIER TRANSFORM
//    ***************************

    myImage.load(myImagePath);
    squareSize = myImage.width() < myImage.height() ? myImage.height() : myImage.width();

    //TYMCZASOWE - zamiast wyrównywania rozmiaru obrazu.
    if(myImage.width() != myImage.height())
    {
        cout << "/nWysokosc i szerokosc obrazu nie sa takie same.";
        return 0;
    }

    myImageMag = myImage;
    myImageMag.fill(Qt::red);
    myImagePhase = myImage;
    myImagePhase.fill(Qt::red);
    myImageOut = myImage;
    myImageOut.fill(Qt::red);

    fft(squareSize, &myImage, &myImageMag, &myImagePhase);
    ift(squareSize, &myImageMag, &myImagePhase, &myImageOut);

    //-------------------------------------
    //DEBUG - Testowy podgląd obrazów!
    //-------------------------------------
    myImage.save("/home/adampelc/Documents/myImage.png");
    myImageMag.save("/home/adampelc/Documents/myImageMag.png");
    myImagePhase.save("/home/adampelc/Documents/myImagePhase.png");
    myImageOut.save("/home/adampelc/Documents/myImageOut.png");
    //-------------------------------------

}

void fft(int squareSize, QImage *myImage, QImage *myImageMag, QImage *myImagePhase)
{
    fftw_plan planR, planG, planB;
    fftw_complex *inR, *inG, *inB, *outR, *outG, *outB;
    QColor tempColor;
    int tempCounter = 0;
    double realR, realG, realB,
            imagR, imagG, imagB,
            magR, magG, magB,
            phaseR, phaseG, phaseB,
            magRMax, magGMax, magBMax;


    //    -----------------------------------------------
    //
    //    Alokacja wektorów i egzekucja FFT.
    //
    //    -----------------------------------------------

    inR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    inG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    inB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);

    outR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    outG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    outB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);

    planG = fftw_plan_dft_2d(squareSize, squareSize, inG, outG, FFTW_FORWARD, FFTW_ESTIMATE);
    planB = fftw_plan_dft_2d(squareSize, squareSize, inB, outB, FFTW_FORWARD, FFTW_ESTIMATE);
    planR = fftw_plan_dft_2d(squareSize, squareSize, inR, outR, FFTW_FORWARD, FFTW_ESTIMATE);

    for(int i=0; i < squareSize; i++)
    {
        for(int j=0; j < squareSize; j++)
        {
            tempColor.setRgb(myImage->pixel(i, j));
            inR[tempCounter][0] = (double)tempColor.red();
            inG[tempCounter][0] = (double)tempColor.green();
            inB[tempCounter][0] = (double)tempColor.blue();

            tempCounter++;
        }
    }

    fftw_execute(planR);
    fftw_execute(planG);
    fftw_execute(planB);

    //    -----------------------------------------------
    //
    //    Normalizacja wartosci amplitudy FFT
    //
    //    -----------------------------------------------

    tempCounter = 0;
    for(int i=0; i < squareSize * squareSize; i++)
    {
        realR = outR[tempCounter][0] / (double)(squareSize * squareSize);
        imagR = outR[tempCounter][1] / (double)(squareSize * squareSize);

        realG = outG[tempCounter][0] / (double)(squareSize * squareSize);
        imagG = outG[tempCounter][1] / (double)(squareSize * squareSize);

        realB = outB[tempCounter][0] / (double)(squareSize * squareSize);
        imagB = outB[tempCounter][1] / (double)(squareSize * squareSize);

        magR = sqrt((realR * realR) + (imagR * imagR));
        magG = sqrt((realG * realG) + (imagG * imagG));
        magB = sqrt((realB * realB) + (imagB * imagB));

        if(i > 0)
        {
            magRMax = (magR > magRMax ? magR : magRMax);
            magGMax = (magG > magGMax ? magG : magGMax);
            magBMax = (magB > magBMax ? magB : magBMax);
        }

        tempCounter++;
    }

    //    -----------------------------------------------
    //
    //    Wyznaczenie amplitudy i fazy
    //
    //    -----------------------------------------------

    tempCounter = 0;
    for(int i=0; i < squareSize; i++)
    {
        for(int j=0; j < squareSize; j++)
        {

            realR = outR[tempCounter][0] / (double)(squareSize * squareSize);
            imagR = outR[tempCounter][1] / (double)(squareSize * squareSize);

            realG = outG[tempCounter][0] / (double)(squareSize * squareSize);
            imagG = outG[tempCounter][1] / (double)(squareSize * squareSize);

            realB = outB[tempCounter][0] / (double)(squareSize * squareSize);
            imagB = outB[tempCounter][1] / (double)(squareSize * squareSize);

            magR = sqrt((realR * realR) + (imagR * imagR));
            magG = sqrt((realG * realG) + (imagG * imagG));
            magB = sqrt((realB * realB) + (imagB * imagB));

            //log(mag)
            int param = 255;
//            magR = log(magR*param+1) / log(magRMax+1);
//            magG = log(magG*param+1) / log(magGMax+1);
//            magB = log(magB*param+1) / log(magBMax+1);
            magR = (param*log(magR+1)) / log(1 + magRMax);
            magG = (param*log(magG+1)) / log(1 + magGMax);
            magB = (param*log(magB+1)) / log(1 + magBMax);

            magR = (magR > 255 ? 255 : magR);
            magG = (magG > 255 ? 255 : magG);
            magB = (magB > 255 ? 255 : magB);

            magR = (magR < 0 ? 0 : magR);
            magG = (magG < 0 ? 0 : magG);
            magB = (magB < 0 ? 0 : magB);

            tempColor.setRed((int)magR);
            tempColor.setGreen((int)magG);
            tempColor.setBlue((int)magB);
            myImageMag->setPixel(i, j, tempColor.rgb());

//            magR = (magR / magRMax)*255.0;
//            magG = (magG / magGMax)*255.0;
//            magB = (magB / magBMax)*255.0;

//            tempColor.setRed(magR);
//            tempColor.setGreen(magG);
//            tempColor.setBlue(magB);
//            myImageMagBasic->setPixel(i, j, tempColor.rgb());


            complex<double> cR(realR, imagR);
            complex<double> cG(realG, imagG);
            complex<double> cB(realB, imagB);

            phaseR = arg(cR) + M_PI;
            phaseG = arg(cG) + M_PI;
            phaseB = arg(cB) + M_PI;


            tempColor.setRed((phaseR / (double)(2 * M_PI)) * 255);
            tempColor.setGreen((phaseG / (double)(2 * M_PI)) * 255);
            tempColor.setBlue((phaseB / (double)(2 * M_PI)) * 255);
            myImagePhase->setPixel(i, j, tempColor.rgb());

            tempCounter++;
        }
    }

    //    -----------------------------------------------
    //
    //    Wyśrodkowanie fazy i amplitudy, zwolnienie pamięci.
    //
    //    -----------------------------------------------

    swapQuadrants(squareSize, myImageMag);
    swapQuadrants(squareSize, myImagePhase);

    fftw_destroy_plan(planR);
    fftw_destroy_plan(planG);
    fftw_destroy_plan(planB);
    fftw_free(inR);
    fftw_free(inG);
    fftw_free(inB);
    fftw_free(outR);
    fftw_free(outG);
    fftw_free(outB);

}

void ift(int squareSize, QImage *myImageMag, QImage *myImagePhase, QImage *myImageOut)
{
    fftw_plan planR, planG, planB;
    fftw_complex *inR, *inG, *inB, *outR, *outG, *outB;
    double magR, magB, magG,
            phaseR, phaseB, phaseG,
            redMax = 0.0, greenMax = 0.0, blueMax = 0.0,
            redMin = 0.0, greenMin = 0.0, blueMin = 0.0;
    QColor tempColor, tempColor2;
    int tempCounter = 0,
            red, green, blue;

    swapQuadrants(squareSize, myImageMag);
    swapQuadrants(squareSize, myImagePhase);

    //    -----------------------------------------------
    //
    //    Allokacja pamięci i obliczanie liczby urojonych
    //
    //    -----------------------------------------------
    inR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    inG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    inB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);

    outR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    outG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);
    outB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * squareSize * squareSize);

    planR = fftw_plan_dft_2d(squareSize, squareSize, inR, outR, FFTW_BACKWARD, FFTW_ESTIMATE);
    planG = fftw_plan_dft_2d(squareSize, squareSize, inG, outG, FFTW_BACKWARD, FFTW_ESTIMATE);
    planB = fftw_plan_dft_2d(squareSize, squareSize, inB, outB, FFTW_BACKWARD, FFTW_ESTIMATE);

    for(int i = 0; i < squareSize; i++)
    {
        for(int j = 0; j < squareSize; j++)
        {
            tempColor.setRgb(myImageMag->pixel(i,j));
            tempColor2.setRgb(myImagePhase->pixel(i,j));

            //magR = tempColor.red();
            magR = tempColor.red() / 255.0;
            phaseR = ((tempColor2.red() / 255.0) * 2 * M_PI) - M_PI;
            inR[tempCounter][0] = (magR * cos(phaseR));
            inR[tempCounter][1] = (magR * sin(phaseR));

            magG = tempColor.green() / 255.0;
            phaseG = ((tempColor2.green() / 255.0) * 2 * M_PI) - M_PI;
            inG[tempCounter][0] = (magG * cos(phaseG));
            inG[tempCounter][1] = (magG * sin(phaseG));

            magB = tempColor.blue() / 255.0;
            phaseB = ((tempColor2.blue() / 255.0) * 2 * M_PI) - M_PI;
            inB[tempCounter][0] = (magB * cos(phaseB));
            inB[tempCounter][1] = (magB * sin(phaseB));
            tempCounter++;
        }
    }
    tempCounter = 0;
    fftw_execute(planR);
    fftw_execute(planG);
    fftw_execute(planB);

    for(int i=0; i < squareSize; i++)
    {
        for(int j=0; j < squareSize; j++)
        {
            redMax = (int)(outR[tempCounter][0] > redMax ? outR[tempCounter][0] : redMax);
            greenMax = (int)(outG[tempCounter][0] > greenMax ? outG[tempCounter][0] : greenMax);
            blueMax = (int)(outB[tempCounter][0] > blueMax ? outB[tempCounter][0] : blueMax);

            redMin = (int)(outR[tempCounter][0] < redMin ? outR[tempCounter][0] : redMin);
            greenMin = (int)(outG[tempCounter][0] < greenMin ? outG[tempCounter][0] : greenMin);
            blueMin = (int)(outB[tempCounter][0] < blueMin ? outB[tempCounter][0] : blueMin);

            tempCounter++;
        }
    }

    redMax += abs(redMin);
    greenMax += abs(greenMin);
    blueMax += abs(blueMin);

    tempCounter = 0;
    for(int i = 0; i < squareSize; i++)
    {
        for(int j = 0; j < squareSize; j++)
        {
            red = (int)(outR[tempCounter][0]);
            green = (int)(outG[tempCounter][0]);
            blue = (int)(outB[tempCounter][0]);

            red = ((red + abs(redMin)) / (double)redMax)*255.0;
            green = ((green + abs(greenMin)) / (double)greenMax)*255.0;
            blue = ((blue + abs(blueMin)) / (double)blueMax)*255.0;

            tempColor.setRed(red);
            tempColor.setGreen(green);
            tempColor.setBlue(blue);

            myImageOut->setPixel(i,j,tempColor.rgb());
            tempCounter++;
        }
    }

    //Zwalnianie pamięci
    fftw_destroy_plan(planR);
    fftw_destroy_plan(planG);
    fftw_destroy_plan(planB);
    fftw_free(inR);
    fftw_free(outR);
    fftw_free(inG);
    fftw_free(outG);
    fftw_free(inB);
    fftw_free(outB);
}


void swapQuadrants(int squareSize, QImage * myImage)
{
    int half = floor(squareSize / 2.0);
    QColor tempColor1, tempColor2;
    QImage *myImageTemp = new QImage(600,600,QImage::Format_RGB32);
    myImageTemp->fill(Qt::red);

    for(int i=0; i < half; i++)
    {
        for(int j=0; j< half; j++)
        {
            //Pierwsza cwiartka
            tempColor1.setRgb(myImage->pixel(i,j));
            tempColor2.setRgb(myImage->pixel(half-i-1, half-j-1));

            myImageTemp->setPixel(i,j,tempColor2.rgb());
            myImageTemp->setPixel(half-i-1, half-j-1, tempColor1.rgb());

            //Druga cwiartka
            tempColor1.setRgb(myImage->pixel(i,half + j));
            tempColor2.setRgb(myImage->pixel(half-i-1, squareSize-j-1));

            myImageTemp->setPixel(i,half + j,tempColor2.rgb());
            myImageTemp->setPixel(half-i-1, squareSize-j-1, tempColor1.rgb());

            //Trzecia cwiartka
            tempColor1.setRgb(myImage->pixel(half + i,j));
            tempColor2.setRgb(myImage->pixel(squareSize-i-1, half-j-1));

            myImageTemp->setPixel(half + i,j,tempColor2.rgb());
            myImageTemp->setPixel(squareSize-i-1, half-j-1, tempColor1.rgb());

            //Czwarta cwiartka
            tempColor1.setRgb(myImage->pixel(half + i,half + j));
            tempColor2.setRgb(myImage->pixel(squareSize-i-1, squareSize-j-1));

            myImageTemp->setPixel(half + i,half + j,tempColor2.rgb());
            myImageTemp->setPixel(squareSize-i-1, squareSize-j-1, tempColor1.rgb());
        }
    }
    *myImage = *myImageTemp;
    delete myImageTemp;
}
