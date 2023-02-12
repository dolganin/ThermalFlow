#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool isANumber(const std::string& str){
    char* flag;
    strtol(str.c_str(), &flag, 10);
    return *flag==NULL;
}

std::vector <Tuple> CSVParse(std::ifstream& in){
    std::vector <Tuple> TableCoordinates;
    int progress = 0;
    while(!in.eof()){
        std::string temp;
        getline(in,temp);
        int ind = temp.find(",");
        if (!isANumber(temp.substr(0,ind))||!isANumber(temp.substr(ind))){
            continue;
        }
        Tuple insert(std::stoi(temp.substr(0,ind)),std::stoi(temp.substr(ind)));
        TableCoordinates.push_back(insert);
    }
    return TableCoordinates;

}

void MainWindow::on_enpow_returnPressed()
{
    ui->settings.enpow = std::stod(ui->enpow->text().toStdString());
}


void MainWindow::on_freq_returnPressed()
{
    ui->settings.freq = std::stoi(ui->freq->text().toStdString());
}


void MainWindow::on_noise_returnPressed()
{
    ui->settings.noise = std::stod(ui->noise->text().toStdString());
}


void MainWindow::on_insignal_returnPressed()
{
    ui->settings.insignal = std::stod(ui->insignal->text().toStdString());
}


void MainWindow::on_resist_returnPressed()
{
    ui->settings.resist = std::stod(ui->resist->text().toStdString());
}


void MainWindow::on_channels_valueChanged(int arg1){
    ui->settings.channels = arg1;
}


void MainWindow::on_progressBar_valueChanged(int value)
{
    for(int i=0;i<100;i++){
        ui->progressBar->setValue(i);
    }
}

void MainWindow::on_csvinput_returnPressed()
{
   std::string csv =(ui->csvinput->text()).toStdString();
   std::ifstream fin;
   fin.open(csv);
       if(!fin.is_open()){
           return;
       }

       CSVParse(fin);


}

