#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <myfft.h>
#include <QLabel>
#include <mysegmentation.h>
#include <my_qlabel.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_radioButton_lowPass_toggled(bool checked);

    void on_radioButton_highPass_toggled(bool checked);

    void on_radioButton_bandPass_toggled(bool checked);

    void on_radioButton_bandStop_toggled(bool checked);

    void on_checkBox_smooth_toggled(bool checked);

    void on_radioButton_toggled(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void Mouse_current_pos();

    void Mouse_Pressed();

    void Mouse_left();


    void on_pushButton_4_clicked();

    void on_actionSave_FFT_triggered();

    void on_actionSave_Seg_triggered();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QImage myImage;
    QImage magImage;
    QImage phaseImage;
    QImage maskImage;
    QImage outputImage;
    QImage maskSegImage,
            segImage;
    myfft newFFT;
    MySegmentation newSeg;
    QLabel *label_segMaskImag = new QLabel;
    my_qlabel *label_segImag = new my_qlabel;
    QLabel *label_orygImag = new QLabel;
    QLabel *label_magImag = new QLabel;
    QLabel *label_phaseImag = new QLabel;
    QLabel *label_maskImag = new QLabel;
    QLabel *label_outImag = new QLabel;
};

#endif // MAINWINDOW_H
