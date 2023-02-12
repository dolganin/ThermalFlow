#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

//class Parameters {
//public:
//int channels = 10;
//double resist = 2.5;
//double noise = 4/10000000;
//int freq = 5000;
//double insignal = 15/1000;
//double enpow = 3.5;
//};
