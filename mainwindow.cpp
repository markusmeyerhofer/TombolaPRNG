#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <chrono>
using namespace std::chrono;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    min = 1;
    max = 1;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{

    milliseconds ms = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    );


    srand(ms.count());

    max = ui->spinBox->value();

    int value =  arc4random() % (max - min + 1) + min;

    ui->lcdNumber->display(value);

}

