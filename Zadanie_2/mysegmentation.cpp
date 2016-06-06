#include "mysegmentation.h"

MySegmentation::MySegmentation()
{

}


void MySegmentation::loadImage(QImage myImage)
{
    oryginalImage = myImage;
    copyImage = myImage;
    QColor tempColor;

    //Wyzerowanie maski i kanały RGB.
    mask.resize(myImage.height(),Vector(myImage.width(),0));
    bMask.resize(myImage.height(),Vector(myImage.width(),0));
    bRGMask.resize(myImage.height(),Vector(myImage.width(),0));
    redChannel.resize(myImage.height(),Vector(myImage.width(),0));
    greenChannel.resize(myImage.height(),Vector(myImage.width(),0));
    blueChannel.resize(myImage.height(),Vector(myImage.width(),0));

    for(int i=0; i < myImage.width(); i++)
    {
        for(int j=0; j < myImage.height(); j++)
        {
            tempColor = myImage.pixelColor(i,j);
            redChannel[i][j] = tempColor.red();
            greenChannel[i][j] = tempColor.green();
            blueChannel[i][j] = tempColor.blue();
        }
    }
}

QImage MySegmentation::basicRegionGrowing(int x, int y, int offset)
{
    bool isGrowing=true;
    QImage tempImage = copyImage;
    QColor tempColor;
    int counter = 0,
            tempRed, tempGreen, tempBlue;

    basicMask(x,y,offset);
    bRGMask[x][y] = maskCounter;

    while(isGrowing)
    {
        isGrowing = false;
        for(int i=1; i<oryginalImage.width()-1; i++)
        {
            for(int j=1; j<oryginalImage.height()-1; j++)
            {
                if(bRGMask[i-1][j] == maskCounter || bRGMask[i+1][j] == maskCounter || bRGMask[i][j-1] == maskCounter || bRGMask[i][j+1] == maskCounter)
                {
                    if(bMask[i][j] == maskCounter && bRGMask[i][j] != maskCounter)
                    {
                        bRGMask[i][j] = maskCounter;
                        isGrowing = true;
                    }
                }
            }
        }


    counter++;
    }

    tempRed = rand() % 256;
    tempGreen = rand() % 256;
    tempBlue = rand() % 256;


    //Zapisanie maski jako zdjecie
    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
            if(bRGMask[i][j] == maskCounter)
            {
                tempColor.setBlue(tempBlue);
                tempColor.setRed(tempRed);
                tempColor.setGreen(tempGreen);
                tempImage.setPixelColor(i,j,tempColor);
            }
            else
            {
                tempColor.setBlue(bRGMask[i][j]);
                tempColor.setRed(bRGMask[i][j]);
                tempColor.setGreen(bRGMask[i][j]);
                tempImage.setPixelColor(i,j,tempColor);
            }
        }
    }

    return tempImage;
}

void MySegmentation::basicMask(int x, int y, int offset)
{
    double tempOffset;
    QImage tempImage = copyImage;
    QColor tempColor;
    int chosenRed, chosenGreen, chosenBlue;

    maskCounter++;

    //
    //Inicjalizacja wartości wybranych pikseli
    //
    chosenRed = redChannel[x][y];
    chosenGreen = greenChannel[x][y];
    chosenBlue = blueChannel[x][y];

    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
            tempOffset = sqrt((chosenRed-redChannel[i][j])*(chosenRed-redChannel[i][j])+(chosenGreen-greenChannel[i][j])*(chosenGreen-greenChannel[i][j])+(chosenBlue-blueChannel[i][j])+(chosenBlue-blueChannel[i][j]));
            if(tempOffset <= offset)
                bMask[i][j] = maskCounter;
        }
    }

    //Zapisanie maski jako zdjecie
    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
            tempColor.setBlue(bMask[i][j]);
            tempColor.setRed(bMask[i][j]);
            tempColor.setGreen(bMask[i][j]);
            tempImage.setPixelColor(i,j,tempColor);
        }
    }

    tempImage.save("/home/adampelc/Documents/DebugTemp/basicMask.png");
}

void MySegmentation::regionGrowing(int x, int y, int offset)
{
    copyImage = oryginalImage;

    //
    //Inicjalizacja zmiennych
    //
    bool isNewRegionCreated = false, isRegionChanged = false, isMoreThanOne = false;
    int regionCounter = 1,
            chosenRed, chosenGreen, chosenBlue,
            regionBeginX, regionBeginY;
    double tempOffset;
    myBoder segBoder;

    //
    //Inicjalizacja wartości wybranych pikseli
    //
    chosenRed = redChannel[x][y];
    chosenGreen = greenChannel[x][y];
    chosenBlue = blueChannel[x][y];

    do
    {
        isNewRegionCreated = false;
        //
        //Wyszukanie pikselu spełniającego kryterium nowego regionu
        //
        for(int i=1; i<oryginalImage.width()-1; i++)
        {
            for(int j=1; j<oryginalImage.height()-1; j++)
            {
                tempOffset = sqrt((chosenRed-redChannel[i][j])*(chosenRed-redChannel[i][j])+(chosenGreen-greenChannel[i][j])*(chosenGreen-greenChannel[i][j])+(chosenBlue-blueChannel[i][j])+(chosenBlue-blueChannel[i][j]));
                if(tempOffset <= offset && mask[i][j] == 0)
                {
                    mask[i][j] = regionCounter;
                    isNewRegionCreated = true;
                    regionBeginX = i;
                    regionBeginY = j;
                    break;
                }
            }
            if(isNewRegionCreated)
                break;
        }

        //Nie znaleziono nowego regionu spełniającego kryterium
        if(!isNewRegionCreated)
            break;

        segBoder.leftBoder = 0;
        segBoder.rightBoder = 0;
        segBoder.topBoder = 0;
        segBoder.bottomBoder = 0;

        do
        {
            isRegionChanged = false;
            //
            //Rozszerzenie pola tworzenia obszaru
            //
            if(regionBeginX - segBoder.leftBoder > 1)
                segBoder.leftBoder++;
            if(regionBeginX + segBoder.rightBoder < oryginalImage.width()-2)
                segBoder.rightBoder++;
            if(regionBeginY - segBoder.topBoder > 1)
                segBoder.topBoder++;
            if(regionBeginY + segBoder.bottomBoder < oryginalImage.height()-2)
                segBoder.bottomBoder++;

            //
            //Poszukiwanie nowego sąsiedztwa
            //
            for(int i=(regionBeginX-segBoder.leftBoder); i<=(regionBeginX+segBoder.rightBoder); i++)
            {
                for(int j=(regionBeginY-segBoder.topBoder); j<=(regionBeginY+segBoder.bottomBoder); j++)
                {
                    if(mask[i][j] == 0)
                    {
                        if(mask[i-1][j] == regionCounter || mask[i+1][j] == regionCounter || mask[i][j-1] == regionCounter || mask[i][j+1] == regionCounter)
                        {
                            tempOffset = sqrt((chosenRed-redChannel[i][j])*(chosenRed-redChannel[i][j])+(chosenGreen-greenChannel[i][j])*(chosenGreen-greenChannel[i][j])+(chosenBlue-blueChannel[i][j])+(chosenBlue-blueChannel[i][j]));
                            if(tempOffset <= offset)
                            {
                                mask[i][j] = regionCounter;
                                isRegionChanged = true;
                                isMoreThanOne = true;
                            }
                        }
                        else
                            mask[i][j] = -1;
                    }
                }
            }
        }while(isRegionChanged);

        if(!isMoreThanOne)
            mask[regionBeginX][regionBeginY] = -2;
        else
            regionCounter++;

        for(int i=0; i<oryginalImage.width(); i++)
        {
            for(int j=0; j<oryginalImage.height(); j++)
            {
                if(mask[i][j] == -1)
                    mask[i][j] = 0;
            }
        }
    }while(isNewRegionCreated);

    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
            if(mask[i][j] == -2)
                mask[i][j] = 0;
        }
    }
}

void MySegmentation::saveToFile()
{
    QString filename = "/home/adampelc/Downloads/maska.txt";
    QFile file( filename );
    if( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        for(int i=0; i<oryginalImage.width(); i++)
        {
            for(int j=0; j<oryginalImage.height(); j++)
            {
                stream << mask[i][j] << "\t";
            }
            stream << endl;
        }
    }
}

QImage MySegmentation::reset()
{
    copyImage = oryginalImage;

    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
                bMask[i][j] = 0;
                bRGMask[i][j] = 0;
        }
    }

    return oryginalImage;
}

QImage MySegmentation::imageWithMask()
{
    QColor tempColor;


    int tempRed = rand() % 256;
    int tempGreen = rand() % 256;
    int tempBlue = rand() % 256;

    for(int i=0; i<oryginalImage.width(); i++)
    {
        for(int j=0; j<oryginalImage.height(); j++)
        {
            if(bRGMask[i][j] == maskCounter)
            {
                tempColor.setBlue(tempBlue);
                tempColor.setRed(tempRed);
                tempColor.setGreen(tempGreen);
                copyImage.setPixelColor(i,j,tempColor);
            }

        }
    }



    return copyImage;
}
