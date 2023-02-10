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
return *flag==0;
}

std::vector <Tuple> CSVParse(std::ifstream& in){
    std::vector <Tuple> TableCoordinates;
    while(!in.eof()){
        std::string temp;
        getline(in,temp);
        int ind = temp.find(" ");
        if (!isANumber(temp.substr(0,ind))||!isANumber(temp.substr(ind))){
            continue;
        }
        Tuple insert(std::stoi(temp.substr(0,ind)),std::stoi(temp.substr(ind)));
        TableCoordinates.push_back(insert);
    }
    return TableCoordinates;
}

void MainWindow::on_lineEdit_editingFinished()
{

}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{

}




void MainWindow::on_lineEdit_7_textEdited(const QString &arg1)
{
    std::ifstream fin (arg1.toLocal8Bit());
    if(!fin.is_open()){
        return;
    }
    CSVParse(fin);

}

