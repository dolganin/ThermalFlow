/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Parameters {
public:
int channels = 10;
    double resist = 2.5;
    double noise = 4/10000000;
    int freq = 5000;
    double insignal = 15/1000;
    double enpow = 3.5;
};

class Ui_MainWindow
{
public:
    Parameters settings;
    QWidget *centralwidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QFrame *line;
    QLineEdit *enpow;
    QLineEdit *freq;
    QLineEdit *noise;
    QLineEdit *insignal;
    QLabel *label_6;
    QLineEdit *resist;
    QFrame *line_2;
    QLabel *label_7;
    QLineEdit *csvinput;
    QFrame *line_3;
    QFrame *line_4;
    QSpinBox *channels;
    QProgressBar *progressBar;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QMenu *menuThermaFlow;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1223, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(1020, 10, 49, 16));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(1010, 30, 81, 31));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(1010, 60, 81, 31));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(1010, 80, 91, 51));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(1010, 120, 91, 41));
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(0, 610, 1221, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        enpow = new QLineEdit(centralwidget);
        enpow->setObjectName("enpow");
        enpow->setGeometry(QRect(1110, 40, 101, 22));
        freq = new QLineEdit(centralwidget);
        freq->setObjectName("freq");
        freq->setGeometry(QRect(1110, 70, 101, 22));
        noise = new QLineEdit(centralwidget);
        noise->setObjectName("noise");
        noise->setGeometry(QRect(1110, 100, 101, 22));
        insignal = new QLineEdit(centralwidget);
        insignal->setObjectName("insignal");
        insignal->setGeometry(QRect(1110, 130, 101, 22));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(1010, 150, 91, 41));
        resist = new QLineEdit(centralwidget);
        resist->setObjectName("resist");
        resist->setGeometry(QRect(1110, 160, 101, 22));
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(1090, 0, 20, 191));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(0, 630, 111, 16));
        csvinput = new QLineEdit(centralwidget);
        csvinput->setObjectName("csvinput");
        csvinput->setGeometry(QRect(110, 630, 113, 22));
        line_3 = new QFrame(centralwidget);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(990, 0, 20, 191));
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(centralwidget);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(1000, 180, 221, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        channels = new QSpinBox(centralwidget);
        channels->setObjectName("channels");
        channels->setGeometry(QRect(1140, 10, 42, 22));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(230, 630, 118, 23));
        progressBar->setAutoFillBackground(false);
        progressBar->setValue(24);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setGeometry(QRect(0, 0, 961, 621));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1223, 22));
        menuThermaFlow = new QMenu(menubar);
        menuThermaFlow->setObjectName("menuThermaFlow");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuThermaFlow->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\232\320\260\320\275\320\260\320\273\320\276\320\262", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\232\320\276\321\215\321\204\321\204\320\270\321\206\320\270\320\265\320\275\321\202\n"
" \321\203\321\201\320\270\320\273\320\265\320\275\320\270\321\217", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\320\276\321\201\320\260 \321\207\320\260\321\201\321\202\320\276\321\202,\n"
"\320\223\321\206", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 \321\210\321\203\320\274\320\276\320\262", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264\320\275\320\276\320\271 \321\201\320\270\320\263\320\275\320\260\320\273", nullptr));
        enpow->setPlaceholderText(QCoreApplication::translate("MainWindow", "200", nullptr));
        freq->setPlaceholderText(QCoreApplication::translate("MainWindow", "5000", nullptr));
        noise->setPlaceholderText(QCoreApplication::translate("MainWindow", "0.4", nullptr));
        insignal->setPlaceholderText(QCoreApplication::translate("MainWindow", "15", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\277\321\200\320\276\321\202\320\270\320\262\320\273\320\265\320\275\320\270\320\265\n"
"\320\264\320\260\321\202\321\207\320\270\320\272\320\276\320\262", nullptr));
        resist->setPlaceholderText(QCoreApplication::translate("MainWindow", "2.5", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264\320\275\320\276\320\271 CSV-\321\204\320\260\320\271\320\273", nullptr));
        csvinput->setPlaceholderText(QCoreApplication::translate("MainWindow", "input.csv", nullptr));
        channels->setSpecialValueText(QCoreApplication::translate("MainWindow", "10", nullptr));
        menuThermaFlow->setTitle(QCoreApplication::translate("MainWindow", "ThermaFlow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
