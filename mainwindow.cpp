#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdlib>
#include "survey.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    label.push_back(ui->label);
    label.push_back(ui->label_2);
    label.push_back(ui->label_3);
    label.push_back(ui->label_4);
    label.push_back(ui->label_5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Survey*test = new Survey();
    connect(test,SIGNAL(somethingToSay(int,QString)),this,SLOT(onSomethingToSay(int,QString)));
    test->nbPoney = 14;
    test->start();
}

void MainWindow::onSomethingToSay(int value,QString message)
{
    label[value]->setText(message);
}
