#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QVector>
#include <cmath>

#define STEP 0.01

double func(double lambda, double x) {
    if (x < 0) {
        return 0;
    } else {
        return 1 - exp(-lambda * x);
    }
}

double plot(double lambda, double x) {
    if (x < 0) {
        return 0;
    } else {
        return lambda * exp(-lambda * x);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->m_func->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->m_func->xAxis->setLabel("X");
    ui->m_func->yAxis->setLabel("F(X)");

    ui->m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->m_plot->xAxis->setLabel("X");
    ui->m_plot->yAxis->setLabel("f(X)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_doWork_clicked()
{
    if (ui->m_x0->value() > ui->m_xn->value()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Некорректный ввод!");
        messageBox.setFixedSize(500,200);

        return;
    }

    int n = (ui->m_xn->value() - ui->m_x0->value()) / STEP + 1;
    QVector<qreal> xVal(n), funcVal(n), plotVal(n);

    for (int i = 0; i < n; i++) {
        xVal[i] = ui->m_x0->value() + i * STEP;
        funcVal[i] = func(ui->m_lambda->value(), xVal[i]);
        plotVal[i] = plot(ui->m_lambda->value(), xVal[i]);
    }

    ui->m_func->addGraph();
    ui->m_func->graph(0)->addData(xVal, funcVal);
    ui->m_func->xAxis->setRange(ui->m_xn->value(), ui->m_x0->value());
    ui->m_func->yAxis->setRange(funcVal[0], funcVal.last());
    ui->m_func->replot();

    ui->m_plot->addGraph();
    ui->m_plot->graph(0)->addData(xVal, plotVal);
    ui->m_plot->xAxis->setRange(ui->m_xn->value(), ui->m_x0->value());
    ui->m_plot->yAxis->setRange(plotVal.last(), plotVal.first());
    ui->m_plot->replot();
}
