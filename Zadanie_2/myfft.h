#include <QImage>
#include <fftw3.h>

#ifndef MYFFT_H
#define MYFFT_H

typedef std::vector<std::vector<double>> Matrix;
typedef std::vector<double> Vector;

class myfft
{
public:
    void loadImage(QImage myImage);
    void resizeImage();
    void fft();
    QImage ifft();

    QImage lowPassNormal(int radius);
    QImage lowPassSmooth(double param);
    QImage highPassNormal(int radius);
    QImage highPassSmooth(double param);
    QImage bandPassNormal(int inRadius, int outRadius);
    QImage bandPassSmooth(double param);
    QImage bandStopNormal(int inRadius, int outRadius);
    QImage bandStopSmooth(double param);
    void modPhase(int k, int l);
    void noFilter();
    QImage logMagImageF(int logParam);
    QImage phaseImageF();

private:
    Matrix swapQuadrants(Matrix data);
    Matrix copy(Matrix data);

    QImage oryginalImage;
    QImage resizedImage;
    QImage magImage;
    QImage logMagImage;
    QImage phaseImage;
    int sizeImage;
    Matrix magR;
    Matrix magG;
    Matrix magB;
    Matrix magRCopy;
    Matrix magGCopy;
    Matrix magBCopy;
    Matrix phaseR;
    Matrix phaseG;
    Matrix phaseB;
    Matrix phaseRCopy;
    Matrix phaseGCopy;
    Matrix phaseBCopy;
};

#endif // MYFFT_H
