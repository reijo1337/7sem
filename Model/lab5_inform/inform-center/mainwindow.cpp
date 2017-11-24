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

    double firstOpMin = ui->m_firstOpMin->value();
    double firstOpMax = ui->m_firstOpMax->value();
    double secondOpMin = ui->m_secondOpMin->value();
    double secondOpMax = ui->m_secondOpMax->value();
    double thirdOpMin = ui->m_thirdOpMin->value();
    double thirdOpMax = ui->m_thirdOpMax->value();

    manager.setOpsTimes(firstOpMin, firstOpMax,
                        secondOpMin, secondOpMax,
                        thirdOpMin, thirdOpMax);

    double firstComp = ui->doubleSpinBox_9->value();
    double secondComp = ui->doubleSpinBox_11->value();

    manager.setCompsTime(firstComp, secondComp);

    manager.manage();

    ui->m_denied->setText(QString::number(manager.deniedReq()));
    ui->m_coef->setText(QString::number(manager.deniedCoef()));
    ui->m_time->setText(QString::number(manager.time()));
}
