#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"../../../");
    Survey*test = new Survey(fileName);
    test->start();
}
