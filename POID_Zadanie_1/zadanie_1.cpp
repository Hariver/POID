#include "zadanie_1.h"
#include "ui_zadanie_1.h"
#include "myphotoedit.h"
#include "qcustomplot.h"

#include <QFileDialog>
#include <QImage>



Zadanie_1::Zadanie_1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Zadanie_1)
{
    ui->setupUi(this);
    ui->label_pic->setStyleSheet("QLabel { background-color : white; color : black; }");
    ui->label_pic_copy->setStyleSheet("QLabel { background-color : white; color : black; }");
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
                                                    tr("Open Images"), "*.jpg",
                                                    tr("All Files (*.*);;Image Files (*.png *.jpg *.bmp);;Test (.test)"));

    if(!fileName.isEmpty())
    {
        myImage.load(fileName);
        this->resize_label_pic();
        ui->label_pic->setPixmap(QPixmap::fromImage(myImage));
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
        myImage = myImage.scaled(label_picWidth, label_picHeight, Qt::KeepAspectRatio);
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
        myImageCopy = myImageCopy.scaled(label_picWidth, label_picHeight, Qt::KeepAspectRatio);
}

void Zadanie_1::on_horizontalSlider_sliderMoved()
{
    myImageCopy = changeBrightness(myImage, ui->horizontalSlider->value());
    this->resize_label_pic_copy();
    ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
}

void Zadanie_1::on_horizontalSlider_2_sliderMoved()
{
    float tempContrast;
    tempContrast = ui->horizontalSlider_2->value() / 100.0;
    myImageCopy = changeContrast(myImage, tempContrast);
    this->resize_label_pic_copy();
    ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
}

void Zadanie_1::on_pushButton_clicked()
{
    myImageCopy = negativeImage(myImage);
    this->resize_label_pic_copy();
    ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
}

void Zadanie_1::on_pushButton_2_clicked()
{
    int tempSize;
    tempSize = ui->lineEdit->text().toInt();
    myImageCopy = arithmeticMeanFilter(myImage, tempSize);
    this->resize_label_pic_copy();
    ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
}

void Zadanie_1::on_pushButton_3_clicked()
{
    int tempSize;
    tempSize = ui->lineEdit->text().toInt();
    myImageCopy = medianFilter(myImage, tempSize);
    this->resize_label_pic_copy();
    ui->label_pic_copy->setPixmap(QPixmap::fromImage(myImageCopy));
}

void Zadanie_1::on_pushButton_4_clicked()
{
    plotHistogram(myImageCopy, ui->plot);
}