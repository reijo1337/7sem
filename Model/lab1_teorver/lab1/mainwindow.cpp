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

double func_2(double a, double b, double x) {
    if (x < a) {
        return 0;
    } else if (a <= x && x < b) {
        return (x - a) / (b - a);
    } else {
        return 1;
    }
}

double plot_2(double a, double b, double x) {
    if (a <= x && x < b) {
        return 1 / (b - a);
    } else {
        return 0;
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

    ui->m_func_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->m_func_2->xAxis->setLabel("X");
    ui->m_func_2->yAxis->setLabel("F(X)");

    ui->m_plot_2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->m_plot_2->xAxis->setLabel("X");
    ui->m_plot_2->yAxis->setLabel("f(X)");
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

    ui->m_func->clearGraphs();
    ui->m_plot->clearGraphs();
    ui->m_func_2->clearGraphs();
    ui->m_plot_2->clearGraphs();


    int n = (ui->m_xn->value() - ui->m_x0->value()) / STEP + 1;
    QVector<qreal> xVal(n), funcVal(n), plotVal(n), funcVal_2(n), plotVal_2(n);

    for (int i = 0; i < n; i++) {
        xVal[i] = ui->m_x0->value() + i * STEP;
        funcVal[i] = func(ui->m_lambda->value(), xVal[i]);
        plotVal[i] = plot(ui->m_lambda->value(), xVal[i]);
        funcVal_2[i] = func_2(ui->m_x0->value(), ui->m_xn->value(), xVal[i]);
        plotVal_2[i] = plot_2(ui->m_x0->value(), ui->m_xn->value(), xVal[i]);
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


    ui->m_func_2->addGraph();
    ui->m_func_2->graph(0)->addData(xVal, funcVal_2);
    ui->m_func_2->xAxis->setRange(ui->m_xn->value(), ui->m_x0->value());
    ui->m_func_2->yAxis->setRange(funcVal_2[0], funcVal_2.last());
    ui->m_func_2->replot();

    ui->m_plot_2->addGraph();
    ui->m_plot_2->graph(0)->addData(xVal, plotVal_2);
    ui->m_plot_2->xAxis->setRange(ui->m_xn->value(), ui->m_x0->value());
    ui->m_plot_2->yAxis->setRange(plotVal_2.first(), plotVal.last());
    ui->m_plot_2->replot();
}
