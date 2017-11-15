#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "randomgenerator.h"
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

void MainWindow::on_m_doWork_clicked()
{
    double a = ui->m_a->value();
    double b = ui->m_b->value();
    double lambda = ui->m_lambda->value();

    RandomGenerator *rndGen = new RandomGenerator(a, b, lambda);
    Generator *gen = new Generator(rndGen);
    Processor *proc = new Processor(rndGen);
    Manager *manager = new Manager(gen, proc, ui->m_num->value());

    double part = -1;
    if (ui->m_isCicle->isChecked())
        part = ui->m_retP->value();
    if (ui->m_method->currentIndex() == 0) {
        manager->deltaT(part, ui->m_dt->value());
    } else {
        manager->event(part);
    }

    int cnt = manager->getMaxQueueSize();

    ui->m_answer->setText("Оптимальный размер буфера: " + QString::number(cnt));
}
