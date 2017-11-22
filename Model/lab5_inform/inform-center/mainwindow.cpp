#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "manager.h"

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
    Manager manager;

    manager.manage();

    ui->m_denied->setText(QString::number(manager.deniedReq()));
    ui->m_coef->setText(QString::number(manager.deniedCoef()));
    ui->m_time->setText(QString::number(manager.time()));
}
