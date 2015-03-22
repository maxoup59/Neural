#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include "survey.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Survey*test = new Survey();
    test->nbPoney = 14;
    test->start();
    //ui->pushButton->setText(QString::number(coeff));
}
