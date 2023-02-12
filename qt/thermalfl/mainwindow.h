#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
#include <iostream>
#include <stdio.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Tuple{
public:
    int xcoord;
    int ycoord;
    Tuple(){};
    Tuple(unsigned int x, unsigned y){
        this->xcoord=x;
        this->ycoord=y;
    }

};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_csvinput_returnPressed();

    void on_enpow_returnPressed();

    void on_freq_returnPressed();

    void on_noise_returnPressed();

    void on_insignal_returnPressed();

    void on_resist_returnPressed();

    void on_channels_valueChanged(int arg1);

    void on_progressBar_valueChanged(int value);

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
