#include <QImage>
#include <QTextStream>
#include <QFile>
#include <cstdlib>

#ifndef MYSEGMENTATION_H
#define MYSEGMENTATION_H

typedef std::vector<std::vector<double>> Matrix;
typedef std::vector<double> Vector;

struct myBoder
{
    int leftBoder;
    int rightBoder;
    int topBoder;
    int bottomBoder;
};

class MySegmentation
{
public:
    MySegmentation();
    void loadImage(QImage myImage);
    QImage basicRegionGrowing(int x, int y, int offset);
    QImage imageWithMask();
    void regionGrowing(int x, int y, int offset); // gdzie x,y -> kordynaty piksela
    void saveToFile();
    QImage reset();

private:
    void basicMask(int x, int y, int offset);
    int maskCounter = 0;
    QImage oryginalImage;
    QImage copyImage;
    Matrix mask;
    Matrix bMask;
    Matrix bRGMask;
    Matrix redChannel;
    Matrix greenChannel;
    Matrix blueChannel;
};

#endif // MYSEGMENTATION_H
