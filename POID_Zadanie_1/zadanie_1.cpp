#include "zadanie_1.h"
#include "ui_zadanie_1.h"
#include "myphotoedit.h"
#include "qcustomplot.h"

#include <QFileDialog>
#include <QImage>
#include <QIODevice>



Zadanie_1::Zadanie_1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Zadanie_1)
{
    ui->setupUi(this);
    ui->label_pic->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->label_pic_copy->setStyleSheet("QLabel { background-color : white; color : black; }");

    ui->plot->xAxis->setRange(-1,256);
    ui->plot->yAxis->setRange(0,1.05);
}

Zadanie_1::~Zadanie_1()
{
    delete ui;
}

void Zadanie_1::on_actionOpen_triggered()
{
    //Open the picture
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images(*.png *.xpm *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Images"), "",
                                                    tr("Image Files (*.png *.jpg *.bmp)"));

    if(!fileName.isEmpty())
    {
        myImage.load(fileName);
        this->resize_label_pic();
        if(ui->dockWidget_2->isFloating())
            ui->label_pic->setPixmap(QPixmap::fromImage(myImage));
        else
            ui->label_pic->setPixmap(QPixmap::fromImage(myImageResize));
        plotHistogram(myImage, ui->plot_3);
    }
}

void Zadanie_1::resize_label_pic()
{
    int picHeight, picWidth,
            label_picHeight, label_picWidth;
    picHeight = myImage.height();
    picWidth = myImage.width();
    label_picHeight = ui->label_pic->height();
    label_picWidth = ui->label_pic->width();

    if(picHeight > label_picHeight || picWidth > label_picWidth)
        myImageResize = myImage.scaled(label_picWidth, label_picHeight, Qt::KeepAspectRatio);
}

void Zadanie_1::resize_label_pic_copy()
{
    int picHeight, picWidth,
            label_picHeight, label_picWidth;
    picHeight = myImageCopy.height();
    picWidth = myImageCopy.width();
    label_picHeight = ui->label_pic_copy->height();
    label_picWidth = ui->label_pic_copy->width();

    if(picHeight > label_picHeight || picWidth > label_picWidth)
        myImageCopyResize = myImageCopy.scaled(label_picWidth, label_picHeight, Qt::KeepAspectRatio);
}

void Zadanie_1::on_horizontalSlider_sliderMoved()
{
    myImageCopy = changeBrightness(myImage, ui->horizontalSlider->value());
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_horizontalSlider_2_sliderMoved()
{
    float tempContrast;
    tempContrast = ui->horizontalSlider_2->value() / 100.0;
    myImageCopy = changeContrast(myImage, tempContrast);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_pushButton_clicked()
{
    myImageCopy = negativeImage(myImage);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_pushButton_2_clicked()
{
    int tempSize;
    tempSize = ui->lineEdit->text().toInt();
    myImageCopy = arithmeticMeanFilter(myImage, tempSize);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_pushButton_3_clicked()
{
    int tempSize;
    tempSize = ui->lineEdit->text().toInt();
    myImageCopy = medianFilter(myImage, tempSize);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
}

void Zadanie_1::on_pushButton_4_clicked()
{
    plotHistogram(myImageCopy, ui->plot);
    plotHistogram(myImage, ui->plot_3);
}

void Zadanie_1::on_pushButton_5_clicked()
{
    int tempSize;
    tempSize = ui->lineEdit->text().toInt();
    myImageCopy = lowPassFilter(myImage, tempSize);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_pushButton_6_clicked()
{
    myImageCopy = operatorRobertsV2(myImage);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_pushButton_7_clicked()
{
    int tempGMin;
    tempGMin = ui->lineEdit_2->text().toInt();
    myImageCopy = distributionRayleigh(myImage, tempGMin);
    this->resize_label_pic_copy();
    if(ui->dockWidget_3->isFloating())
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    else
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    plotHistogram(myImageCopy, ui->plot);
}

void Zadanie_1::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    QFile plik(filename);
    plik.open(QIODevice::WriteOnly);
    myImageCopy.save(&plik, "PNG");



}

void Zadanie_1::on_dockWidget_2_topLevelChanged(bool topLevel)
{
    if(topLevel)
    {
        ui->label_pic->setFixedHeight(myImage.height());
        ui->label_pic->setFixedWidth(myImage.width());
        ui->dockWidget_2->setFixedHeight(myImage.height());
        ui->dockWidget_2->setFixedWidth(myImage.width());
        ui->label_pic->setPixmap(QPixmap::fromImage(myImage));
    }
    else if(!topLevel)
    {
        ui->label_pic->setFixedHeight(331);
        ui->label_pic->setFixedWidth(631);
        ui->dockWidget_2->setFixedHeight(381);
        ui->dockWidget_2->setFixedWidth(632);
        ui->label_pic->setPixmap(QPixmap::fromImage(myImageResize));
    }
}

void Zadanie_1::on_dockWidget_3_topLevelChanged(bool topLevel)
{
    if(topLevel)
    {
       ui->label_pic_copy->setFixedHeight(myImageCopy.height());
       ui->label_pic_copy->setFixedWidth(myImageCopy.width());
       ui->dockWidget_3->setFixedHeight(myImageCopy.height());
       ui->dockWidget_3->setFixedWidth(myImageCopy.width());
       ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
    }
    else if(!topLevel)
    {
        ui->label_pic_copy->setFixedHeight(331);
        ui->label_pic_copy->setFixedWidth(631);
        ui->dockWidget_3->setFixedHeight(381);
        ui->dockWidget_3->setFixedWidth(632);
        this->resize_label_pic_copy();
        ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopyResize));
    }
}
