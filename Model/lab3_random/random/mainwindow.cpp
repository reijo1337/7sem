#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "randovizer.h"

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
    Randovizer rnd;
    rnd.generate();

    rnd.fillAlgTbl(ui->m_algRandTbl);
    rnd.fillTblTbl(ui->m_tblRandTbl);
}

void MainWindow::on_pushButton_2_clicked()
{
    QStringList list = ui->m_seq->text().split(" ");
    QVector<int> nums;

    Q_FOREACH(QString num, list) {
        nums.push_back(num.toInt());
    }

    ui->m_entrop->setText(QString::number(
                              Randovizer::findEntrop(nums) * 100,
                              'f', 2) + "%");

}
