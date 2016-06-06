#include <fftw3.h>
#include <complex>
#include <math.h>
#include <QImage>
#include "myfft.h"

using namespace std;

void myfft::loadImage(QImage myImage)
{
    oryginalImage = myImage;
    //Debug--------
    //oryginalImage.save("/home/adampelc/Documents/myfft_imgs/oryginalImage.png");
    //-------------
}

void myfft::resizeImage()
{
    QColor tempColor;

    sizeImage = ( oryginalImage.height() > oryginalImage.width() ? oryginalImage.height() : oryginalImage.width() );

    QImage tempImage(sizeImage, sizeImage, QImage::Format_RGB32);
    tempImage.fill(Qt::black);

    for(int i=0; i < oryginalImage.height(); i++)
    {
        for(int j=0; j < oryginalImage.width(); j++)
        {
            tempColor = oryginalImage.pixelColor(i,j);
            tempImage.setPixelColor(i, j, tempColor);
        }
    }

    resizedImage = tempImage;
    logMagImage = resizedImage;
    //Debug--------
    //resizedImage.save("/home/adampelc/Documents/myfft_imgs/resizedImage.png");
    //-------------
}

void myfft::fft()
{

    fftw_plan planR, planG, planB;
    fftw_complex *inR, *inG, *inB, *outR, *outG, *outB;
    QColor tempColor;
    int tempCounter = 0;
    double realR, realG, realB,
            imagR, imagG, imagB;

    //    -----------------------------------------------
    //
    //    Alokacja wektorów i wykonanie FFT.
    //
    //    -----------------------------------------------

    inR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    inG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    inB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);

    outR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    outG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    outB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);

    planR = fftw_plan_dft_2d(sizeImage, sizeImage, inR, outR, FFTW_FORWARD, FFTW_ESTIMATE);
    planG = fftw_plan_dft_2d(sizeImage, sizeImage, inG, outG, FFTW_FORWARD, FFTW_ESTIMATE);
    planB = fftw_plan_dft_2d(sizeImage, sizeImage, inB, outB, FFTW_FORWARD, FFTW_ESTIMATE);

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            tempColor.setRgb(resizedImage.pixel(i, j));
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
    //    Stworzenie tablicy 2D dla amplitudy i fazy.
    //
    //    -----------------------------------------------

    magR.resize(sizeImage, vector<double>(sizeImage, 0));
    magG.resize(sizeImage, vector<double>(sizeImage, 0));
    magB.resize(sizeImage, vector<double>(sizeImage, 0));
    magRCopy.resize(sizeImage, vector<double>(sizeImage, 0));
    magGCopy.resize(sizeImage, vector<double>(sizeImage, 0));
    magBCopy.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseR.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseG.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseB.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseRCopy.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseGCopy.resize(sizeImage, vector<double>(sizeImage, 0));
    phaseBCopy.resize(sizeImage, vector<double>(sizeImage, 0));

    tempCounter = 0;
    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            realR = outR[tempCounter][0] / (double)(sizeImage * sizeImage);
            imagR = outR[tempCounter][1] / (double)(sizeImage * sizeImage);

            realG = outG[tempCounter][0] / (double)(sizeImage * sizeImage);
            imagG = outG[tempCounter][1] / (double)(sizeImage * sizeImage);

            realB = outB[tempCounter][0] / (double)(sizeImage * sizeImage);
            imagB = outB[tempCounter][1] / (double)(sizeImage * sizeImage);

            magR[i][j] = sqrt((realR * realR) + (imagR * imagR));
            magG[i][j] = sqrt((realG * realG) + (imagG * imagG));
            magB[i][j] = sqrt((realB * realB) + (imagB * imagB));

            complex<double> cR(realR, imagR);
            complex<double> cG(realG, imagG);
            complex<double> cB(realB, imagB);

            phaseR[i][j] = arg(cR) + M_PI;
            phaseG[i][j] = arg(cG) + M_PI;
            phaseB[i][j] = arg(cB) + M_PI;

            tempCounter++;
        }
    }

    magR = swapQuadrants(magR);
    magG = swapQuadrants(magG);
    magB = swapQuadrants(magB);

    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    phaseRCopy = phaseR;
    phaseGCopy = phaseG;
    phaseBCopy = phaseB;

}

QImage myfft::ifft()
{
    QImage ifftImage;
    QColor tempColor;
    fftw_plan planR, planG, planB;
    fftw_complex *inR, *inG, *inB, *outR, *outG, *outB;
    int tempCounter = 0,
            red, green, blue;

    magRCopy = swapQuadrants(magRCopy);
    magGCopy = swapQuadrants(magGCopy);
    magBCopy = swapQuadrants(magBCopy);

    ifftImage = resizedImage;

    inR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    inG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    inB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);

    outR = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    outG = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);
    outB = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * sizeImage * sizeImage);

    planR = fftw_plan_dft_2d(sizeImage, sizeImage, inR, outR, FFTW_BACKWARD, FFTW_ESTIMATE);
    planG = fftw_plan_dft_2d(sizeImage, sizeImage, inG, outG, FFTW_BACKWARD, FFTW_ESTIMATE);
    planB = fftw_plan_dft_2d(sizeImage, sizeImage, inB, outB, FFTW_BACKWARD, FFTW_ESTIMATE);

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            inR[tempCounter][0] = (magRCopy[i][j] * cos(phaseRCopy[i][j]));
            inR[tempCounter][1] = (magRCopy[i][j] * sin(phaseRCopy[i][j]));

            inG[tempCounter][0] = (magGCopy[i][j] * cos(phaseGCopy[i][j]));
            inG[tempCounter][1] = (magGCopy[i][j] * sin(phaseGCopy[i][j]));

            inB[tempCounter][0] = (magBCopy[i][j] * cos(phaseBCopy[i][j]));
            inB[tempCounter][1] = (magBCopy[i][j] * sin(phaseBCopy[i][j]));
            tempCounter ++;
        }
    }

    fftw_execute(planR);
    fftw_execute(planG);
    fftw_execute(planB);

    tempCounter = 0;
    for(int i = 0; i < sizeImage; i++)
    {
        for(int j = 0; j < sizeImage; j++)
        {
            red = (int)(outR[tempCounter][0]);
            green = (int)(outG[tempCounter][0]);
            blue = (int)(outB[tempCounter][0]);

            red = abs(red);
            green = abs(green);
            blue = abs(blue);

            red = (red > 255 ? 255 : red);
            green = (green > 255 ? 255 : green);
            blue = (blue > 255 ? 255 : blue);

            tempColor.setRed(red);
            tempColor.setGreen(green);
            tempColor.setBlue(blue);

            ifftImage.setPixel(i,j,tempColor.rgb());
            tempCounter++;
        }
    }

    //Debug-------
    //ifftImage.save("/home/adampelc/Documents/myfft_imgs/ifftImage.png");
    //------------
    return ifftImage;
}

QImage myfft::logMagImageF(int logParam)
{
    QColor tempColor;
//    double magRMax = 0, magGMax = 0, magBMax = 0,
    double temp;

//    //Maksymalna wartość amplitudy.
//    for(int i=0; i < sizeImage; i++)
//    {
//        for(int j=0; j < sizeImage; j++)
//        {
//            magRMax = (magR[i][j] > magRMax ? magR[i][j] : magRMax);
//            magGMax = (magG[i][j] > magGMax ? magG[i][j] : magGMax);
//            magBMax = (magB[i][j] > magBMax ? magB[i][j] : magBMax);
//        }
//    }

    //Przejęcie konfiguracji obrazu matki.

//    magRCopy = swapQuadrants(magR);
//    magGCopy = swapQuadrants(magG);
//    magBCopy = swapQuadrants(magB);

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            temp = 255*(log(logParam*magRCopy[i][j]+1) / log(logParam + 1));
            temp = (temp > 255 ? 255 : temp);
            tempColor.setRed((int)temp);

            temp = 255*(log(logParam*magGCopy[i][j]+1) / log(logParam + 1));
            temp = (temp > 255 ? 255 : temp);
            tempColor.setGreen((int)temp);

            temp = 255*(log(logParam*magBCopy[i][j]+1) / log(logParam + 1));
            temp = (temp > 255 ? 255 : temp);
            tempColor.setBlue((int)temp);

            logMagImage.setPixel(i, j, tempColor.rgb());
        }
    }

    //logMagImage.save("/home/adampelc/Documents/myfft_imgs/logMagImage.png");

    return logMagImage;
}

//QImage zwracany jest dziwny, ale działa.
QImage myfft::phaseImageF()
{
    QColor tempColor;
    double tempRMax = 0, tempGMax = 0, tempBMax = 0;

    phaseRCopy = swapQuadrants(phaseRCopy);
    phaseGCopy = swapQuadrants(phaseGCopy);
    phaseBCopy = swapQuadrants(phaseBCopy);

    phaseImage = resizedImage;

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            tempRMax = (tempRMax > phaseRCopy[i][j] ? tempRMax : phaseRCopy[i][j]);
            tempGMax = (tempGMax > phaseGCopy[i][j] ? tempGMax : phaseGCopy[i][j]);
            tempBMax = (tempBMax > phaseBCopy[i][j] ? tempBMax : phaseBCopy[i][j]);
        }
    }

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            tempColor.setRed(abs((phaseRCopy[i][j] / tempRMax) * 255));
            tempColor.setGreen(abs((phaseGCopy[i][j] / tempGMax) * 255));
            tempColor.setBlue(abs((phaseBCopy[i][j] / tempBMax) * 255));
            phaseImage.setPixel(i, j, tempColor.rgb());
        }
    }

   //phaseImage.save("/home/adampelc/Documents/myfft_imgs/phaseImage.png");

    phaseRCopy = swapQuadrants(phaseRCopy);
    phaseGCopy = swapQuadrants(phaseGCopy);
    phaseBCopy = swapQuadrants(phaseBCopy);
    return phaseImage;
}

Matrix myfft::swapQuadrants(Matrix data)
{
    int half = floor(sizeImage / 2.0);
    Matrix tempData(
                sizeImage,
                std::vector<double>(sizeImage));

    for(int i=0; i < half; i++)
    {
        for(int j=0; j < half; j++)
        {
            //Pierwsza cwiartka
            tempData[half-i-1][half-j-1] = data[i][j];
            tempData[i][j] = data[half-i-1][half-j-1];

            //Druga cwiartka
            tempData[half-i-1][sizeImage-j-1] = data[i][half + j];
            tempData[i][half + j] = data[half-i-1][sizeImage-j-1];

            //Trzecia cwiartka
            tempData[sizeImage-i-1][half-j-1] = data[half + i][j];
            tempData[half + i][j] = data[sizeImage-i-1][half-j-1];

            //Czwarta cwiartka
            tempData[sizeImage-i-1][sizeImage-j-1] = data[half + i][half + j];
            tempData[half + i][half + j] = data[sizeImage-i-1][sizeImage-j-1];
        }
    }
    return tempData;
}

Matrix myfft::copy(Matrix data)
{
    return data;
}

QImage myfft::highPassNormal(int radius)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                2*radius+1,
                std::vector<double>(2*radius+1,0.0));
    QImage tempImage = resizedImage;
    tempImage.fill(Qt::white);
    QColor tempColor;

    //Tworzenie maski - dolno-przepustowy
    for(int i=(-radius); i <= radius; i++)
    {
        for(int j=(-radius); j <= radius; j++)
        {
            if(i*i + j*j <= radius*radius)
                tempFilter[i+radius][j+radius] = 0.0;
            else
                tempFilter[i+radius][j+radius] = 1.0;
        }
    }

    for(int i=(sizeImage/2)-radius; i <=(sizeImage/2)+radius; i++)
    {
        for(int j=(sizeImage/2)-radius; j <=(sizeImage/2)+radius; j++)
        {
            magRCopy[i][j] *= tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)];
            magGCopy[i][j] *= tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)];
            magBCopy[i][j] *= tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)];
            tempColor.setRed(tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)] * 255);
            tempColor.setGreen(tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)] * 255);
            tempColor.setBlue(tempFilter[i+radius-(sizeImage/2)][j+radius-(sizeImage/2)] * 255);
            tempImage.setPixel(i, j, tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/highPassNormalImage.png");
    return tempImage;
}

QImage myfft::lowPassNormal(int radius)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));

    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
            if(i*i + j*j <= radius*radius)
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 1.0;
            else
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 0.0;
            tempColor.setRed(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setGreen(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setBlue(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/lowPassNormalImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }
    return tempImage;
}

QImage myfft::bandPassNormal(int inRadius, int outRadius)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
            if((i*i + j*j <= outRadius*outRadius) && (i*i + j*j >= inRadius*inRadius))
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 1.0;
            else
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 0.0;
            tempColor.setRed(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setGreen(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setBlue(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/bandPassNormalImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }
    return tempImage;
}

QImage myfft::bandStopNormal(int inRadius, int outRadius)
{

    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
            if((i*i + j*j <= outRadius*outRadius) && (i*i + j*j >= inRadius*inRadius))
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 0.0;
            else
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = 1.0;
            tempColor.setRed(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setGreen(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempColor.setBlue(tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] * 255);
            tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/bandStopNormalImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }
    return tempImage;
}

QImage myfft::highPassSmooth(double param)
{

    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));

    double temp;
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
                temp = -exp(-(i*i+j*j)/(2*param*param)) + 1;
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = temp;
                tempColor.setRed(temp * 255);
                tempColor.setGreen(temp * 255);
                tempColor.setBlue(temp * 255);
                tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/highPassSmoothImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }

    return tempImage;
}

QImage myfft::lowPassSmooth(double param)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));

    double temp;
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
                temp = (-1.0 / exp((i*i+j*j)/(2*param*param))) + 1;
                temp = 1 - temp;
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = temp;
                tempColor.setRed(temp * 255);
                tempColor.setGreen(temp * 255);
                tempColor.setBlue(temp * 255);
                tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/lowPassSmoothImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }

    return tempImage;
}

QImage myfft::bandPassSmooth(double param)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));

    double temp;
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
                temp = -exp(-2*abs((i*i+j*j)/(2*param*param))) + exp(-abs((i*i+j*j)/(2*param*param)));
                temp *= 4;
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = temp;
                tempColor.setRed(temp * 255);
                tempColor.setGreen(temp * 255);
                tempColor.setBlue(temp * 255);
                tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/bandPassSmoothImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }

    return tempImage;
}

QImage myfft::bandStopSmooth(double param)
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;

    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage,0.0));

    double temp;
    QImage tempImage = resizedImage;
    QColor tempColor;

    //Tworzenie maski - gorno-przepustowy
    for(int i=-(sizeImage/2); i < sizeImage/2; i++)
    {
        for(int j=-(sizeImage/2); j < sizeImage/2; j++)
        {
                temp = -exp(-2*abs((i*i+j*j)/(2*param*param))) + exp(-abs((i*i+j*j)/(2*param*param)));
                temp = -temp*4+1;
                tempFilter[i+(sizeImage/2)][j+(sizeImage/2)] = temp;
                tempColor.setRed(temp * 255);
                tempColor.setGreen(temp * 255);
                tempColor.setBlue(temp * 255);
                tempImage.setPixel(i+(sizeImage/2), j+(sizeImage/2), tempColor.rgb());
        }
    }

    //tempImage.save("/home/adampelc/Documents/myfft_imgs/bandStopSmoothImage.png");

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {
            magRCopy[i][j] *= tempFilter[i][j];
            magGCopy[i][j] *= tempFilter[i][j];
            magBCopy[i][j] *= tempFilter[i][j];
        }
    }

    return tempImage;
}

void myfft::modPhase(int k, int l)
{
    Matrix tempFilter(
                sizeImage,
                std::vector<double>(sizeImage, 0));

    phaseRCopy = phaseR;
    phaseGCopy = phaseG;
    phaseBCopy = phaseB;

    phaseRCopy = swapQuadrants(phaseRCopy);
    phaseGCopy = swapQuadrants(phaseGCopy);
    phaseBCopy = swapQuadrants(phaseBCopy);

    for(int i=0; i < sizeImage; i++)
    {
        for(int j=0; j < sizeImage; j++)
        {

            tempFilter[i][j] = (-i*k*2*M_PI)/sizeImage + (-j*l*2*M_PI)/sizeImage + (k + l)*M_PI;
            phaseRCopy[i][j] += tempFilter[i][j];
            phaseGCopy[i][j] += tempFilter[i][j];
            phaseBCopy[i][j] += tempFilter[i][j];
        }
    }

    phaseRCopy = swapQuadrants(phaseRCopy);
    phaseGCopy = swapQuadrants(phaseGCopy);
    phaseBCopy = swapQuadrants(phaseBCopy);
}

void myfft::noFilter()
{
    magRCopy = magR;
    magGCopy = magG;
    magBCopy = magB;
}
