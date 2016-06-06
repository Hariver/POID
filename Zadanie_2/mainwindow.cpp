#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QIODevice>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QHBoxLayout>
#include <QDebug>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList<int> heights;
    heights.push_back(400);
    heights.push_back(350);
    ui->splitter->setSizes(heights);
    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);


    ui->pushButton->setDisabled(true);
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit->setDisabled(true);
    ui->lineEdit_3->setDisabled(true);
    ui->lineEdit_4->setDisabled(true);
    ui->pushButton_2->setDisabled(true);



}

MainWindow::~MainWindow()
{
    delete label_orygImag;
    delete label_magImag;
    delete label_phaseImag;
    delete label_maskImag;
    delete label_outImag;
    delete label_segImag;
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
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
            //delete label_orygImag;
            myImage.load(fileName);

            newSeg.loadImage(myImage);
            delete label_segImag;
            label_segImag = new my_qlabel;
            label_segImag->setPixmap(QPixmap::fromImage(myImage));
            label_segImag->setAlignment(Qt::AlignLeft);
            label_segImag->setAlignment(Qt::AlignTop);
            ui->scrollArea_segm->setWidget(label_segImag);

            connect(label_segImag, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_current_pos()));
            connect(label_segImag, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
            connect(label_segImag, SIGNAL(Mouse_Left()), this, SLOT(Mouse_left()));

            newFFT.loadImage(myImage);
            newFFT.resizeImage();
            newFFT.fft();
            delete label_orygImag;
            label_orygImag = new QLabel;
            label_orygImag->setPixmap(QPixmap::fromImage(myImage));
            ui->scrollArea_orygImag->setWidget(label_orygImag);
            ui->pushButton->setDisabled(false);
            ui->lineEdit_3->setDisabled(false);
            ui->lineEdit_4->setDisabled(false);
            ui->pushButton_2->setDisabled(false);
        }

}

void MainWindow::on_radioButton_lowPass_toggled(bool checked)
{
    if(!checked)
        return;

    bool isSmooth = ui->checkBox_smooth->isChecked();

    ui->lineEdit->setDisabled(false);

    if(!isSmooth)
    {
        ui->label->setText("Promień:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
    else
    {
        ui->label->setText("Parametr:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
}

void MainWindow::on_radioButton_highPass_toggled(bool checked)
{
    if(!checked)
        return;

    bool isSmooth = ui->checkBox_smooth->isChecked();
    ui->lineEdit->setDisabled(false);

    if(!isSmooth)
    {
        ui->label->setText("Promień:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
    else
    {
        ui->label->setText("Parametr:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
}

void MainWindow::on_radioButton_bandPass_toggled(bool checked)
{
    if(!checked)
        return;

    bool isSmooth = ui->checkBox_smooth->isChecked();

    ui->lineEdit->setDisabled(false);

    if(!isSmooth)
    {
        ui->label->setText("Promień wewnętrzny:");
        ui->label_2->setText("Promień zewnętrzny:");
        ui->lineEdit_2->setDisabled(false);
    }
    else
    {
        ui->label->setText("Parametr:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
}

void MainWindow::on_radioButton_bandStop_toggled(bool checked)
{
    if(!checked)
        return;

    bool isSmooth = ui->checkBox_smooth->isChecked();

    ui->lineEdit->setDisabled(false);

    if(!isSmooth)
    {
        ui->label->setText("Promień wewnętrzny:");
        ui->label_2->setText("Promień zewnętrzny:");
        ui->lineEdit_2->setDisabled(false);
    }
    else
    {
        ui->label->setText("Parametr:");
        ui->label_2->setText("");
        ui->lineEdit_2->setDisabled(true);
    }
}

void MainWindow::on_checkBox_smooth_toggled(bool checked)
{

    if(ui->radioButton_lowPass->isChecked())
    {
        ui->lineEdit->setDisabled(false);
        if(!checked)
        {
            ui->label->setText("Promień:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
        else
        {
            ui->label->setText("Parametr:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
    }
    else if(ui->radioButton_highPass->isChecked())
    {
        ui->lineEdit->setDisabled(false);
        if(!checked)
        {
            ui->label->setText("Promień:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
        else
        {
            ui->label->setText("Parametr:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
    }
    else if(ui->radioButton_bandPass->isChecked())
    {
        ui->lineEdit->setDisabled(false);
        if(!checked)
        {
            ui->label->setText("Promień wewnętrzny:");
            ui->label_2->setText("Promień zewnętrzny:");
            ui->lineEdit_2->setDisabled(false);
        }
        else
        {
            ui->label->setText("Parametr:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
    }
    else if(ui->radioButton_bandStop->isChecked())
    {
        ui->lineEdit->setDisabled(false);
        if(!checked)
        {
            ui->label->setText("Promień wewnętrzny:");
            ui->label_2->setText("Promień zewnętrzny:");
            ui->lineEdit_2->setDisabled(false);
        }
        else
        {
            ui->label->setText("Parametr:");
            ui->label_2->setText("");
            ui->lineEdit_2->setDisabled(true);
        }
    }
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    if(!checked)
        return;

    ui->label->setText("");
    ui->label_2->setText("");
    ui->lineEdit_2->setDisabled(true);
    ui->lineEdit->setDisabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    if(!ui->checkBox_smooth->isChecked())
    {
        if(ui->radioButton_lowPass->isChecked())
            maskImage = newFFT.lowPassNormal(ui->lineEdit->text().toInt());
        else if(ui->radioButton_highPass->isChecked())
            maskImage = newFFT.highPassNormal(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandPass->isChecked())
            maskImage = newFFT.bandPassNormal(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
        else if(ui->radioButton_bandStop->isChecked())
            maskImage = newFFT.bandStopNormal(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
        else
            newFFT.noFilter();
    }
    else if(ui->checkBox_smooth->isChecked())
    {
        if(ui->radioButton_lowPass->isChecked())
            maskImage = newFFT.lowPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_highPass->isChecked())
            maskImage = newFFT.highPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandPass->isChecked())
            maskImage = newFFT.bandPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandStop->isChecked())
            maskImage = newFFT.bandStopSmooth(ui->lineEdit->text().toDouble());
        else
            newFFT.noFilter();
    }

    magImage = newFFT.logMagImageF(10000);
    phaseImage = newFFT.phaseImageF();
    outputImage = newFFT.ifft();

    label_magImag->setPixmap(QPixmap::fromImage(magImage));
    ui->scrollArea_magImag->setWidget(label_magImag);

    label_phaseImag->setPixmap(QPixmap::fromImage(phaseImage));
    ui->scrollArea_phaseImag->setWidget(label_phaseImag);

    label_maskImag->setPixmap(QPixmap::fromImage(maskImage));
    ui->scrollArea_maskImag->setWidget(label_maskImag);

    label_outImag->setPixmap(QPixmap::fromImage(outputImage));
    ui->scrollArea_outImag->setWidget(label_outImag);
}

void MainWindow::on_pushButton_2_clicked()
{
    int tempX, tempY;

    tempX = ui->lineEdit_3->text().toInt();
    tempY = ui->lineEdit_4->text().toInt();

    if(!ui->checkBox_smooth->isChecked())
    {
        if(ui->radioButton_lowPass->isChecked())
            maskImage = newFFT.lowPassNormal(ui->lineEdit->text().toInt());
        else if(ui->radioButton_highPass->isChecked())
            maskImage = newFFT.highPassNormal(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandPass->isChecked())
            maskImage = newFFT.bandPassNormal(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
        else if(ui->radioButton_bandStop->isChecked())
            maskImage = newFFT.bandStopNormal(ui->lineEdit->text().toInt(), ui->lineEdit_2->text().toInt());
        else
            newFFT.noFilter();
    }
    else if(ui->checkBox_smooth->isChecked())
    {
        if(ui->radioButton_lowPass->isChecked())
            maskImage = newFFT.lowPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_highPass->isChecked())
            maskImage = newFFT.highPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandPass->isChecked())
            maskImage = newFFT.bandPassSmooth(ui->lineEdit->text().toDouble());
        else if(ui->radioButton_bandStop->isChecked())
            maskImage = newFFT.bandStopSmooth(ui->lineEdit->text().toDouble());
        else
            newFFT.noFilter();
    }

    newFFT.modPhase(tempX, tempY);

    magImage = newFFT.logMagImageF(10000);
    phaseImage = newFFT.phaseImageF();
    outputImage = newFFT.ifft();

    label_magImag->setPixmap(QPixmap::fromImage(magImage));
    ui->scrollArea_magImag->setWidget(label_magImag);

    label_phaseImag->setPixmap(QPixmap::fromImage(phaseImage));
    ui->scrollArea_phaseImag->setWidget(label_phaseImag);

    label_maskImag->setPixmap(QPixmap::fromImage(maskImage));
    ui->scrollArea_maskImag->setWidget(label_maskImag);

    label_outImag->setPixmap(QPixmap::fromImage(outputImage));
    ui->scrollArea_outImag->setWidget(label_outImag);
}

void MainWindow::Mouse_current_pos()
{
    ui->label_mousePos->setText(QString("X = %1 ; Y = %2").arg(label_segImag->x)
                                .arg(label_segImag->y));
}

void MainWindow::Mouse_Pressed()
{

}

void MainWindow::Mouse_left()
{

}

void MainWindow::on_pushButton_4_clicked()
{
    int x = label_segImag->x,
            y = label_segImag->y;

    //newSeg.reset();

    maskSegImage = newSeg.basicRegionGrowing(x,y, ui->lineEdit_8->text().toInt());
    label_segMaskImag->setPixmap(QPixmap::fromImage(maskSegImage));
    ui->scrollArea_segm_mask->setWidget(label_segMaskImag);

    segImage = newSeg.imageWithMask();
    label_segImag->setPixmap(QPixmap::fromImage(segImage));
    ui->scrollArea_segm->setWidget(label_segImag);
}

void MainWindow::on_actionSave_FFT_triggered()
{
    myImage.save("/home/adampelc/Documents/do_sprawozdania/FFT/oryginal.png");
    magImage.save("/home/adampelc/Documents/do_sprawozdania/FFT/amplituda.png");;
    phaseImage.save("/home/adampelc/Documents/do_sprawozdania/FFT/faza.png");
    maskImage.save("/home/adampelc/Documents/do_sprawozdania/FFT/maska.png");
    outputImage.save("/home/adampelc/Documents/do_sprawozdania/FFT/wyjsciowy.png");
}

void MainWindow::on_actionSave_Seg_triggered()
{
    myImage.save("/home/adampelc/Documents/do_sprawozdania/Segmentacja/oryginal.png");
    maskSegImage.save("/home/adampelc/Documents/do_sprawozdania/Segmentacja/zmieniony.png");
    segImage.save("/home/adampelc/Documents/do_sprawozdania/Segmentacja/maska.png");
}

void MainWindow::on_pushButton_3_clicked()
{
    segImage = newSeg.reset();
    label_segImag->setPixmap(QPixmap::fromImage(segImage));
    ui->scrollArea_segm->setWidget(label_segImag);
}
