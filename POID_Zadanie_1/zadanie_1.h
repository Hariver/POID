#ifndef ZADANIE_1_H
#define ZADANIE_1_H

#include "myphotoedit.h"

#include <QMainWindow>
#include <QImage>

namespace Ui {
class Zadanie_1;
}

class Zadanie_1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Zadanie_1(QWidget *parent = 0);
    ~Zadanie_1();

private slots:
    void on_actionOpen_triggered();

    void on_horizontalSlider_sliderMoved();

    void on_horizontalSlider_2_sliderMoved();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Zadanie_1 *ui;
    QImage myImage;
    QImage myImageCopy;
    void resize_label_pic();
    void resize_label_pic_copy();
};

#endif // ZADANIE_1_H
