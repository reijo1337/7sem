#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVector>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->m_matrixTable->setItemDelegate(new TableDelegate());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_makeMatrix_clicked()
{
    int size = ui->m_num->value();
    ui->m_matrixTable->clear();
    ui->m_matrixTable->setRowCount(size);
    ui->m_matrixTable->setColumnCount(size);
}

void MainWindow::on_pushButton_clicked()
{
    int size = ui->m_matrixTable->rowCount();
    if (size == 0)
        return;

    qreal matrix[size][size];

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = 0;

    for (int row = 0; row < size; row++) {
        for (int i = 0; i < size; i++) {
            matrix[row][i] += getVal(row, i);
            matrix[row][row] -= getVal(i, row);
        }
    }


//    QString result = "";
//    for (int i = 0; i < size; i++) {
//        result += "t" + QString::number(i) + " = " + QString::number(res[i]) + "\n";
//    }

//    QMessageBox msg;
//    msg.setText(result);
//    msg.setWindowTitle("Результат");
//    msg.exec();
}

qreal MainWindow::getVal(int row, int col)
{
    QTableWidgetItem *item = ui->m_matrixTable->item(row, col);
    if (!item) {
        return 0.0;
    } else {
        return item->data(Qt::DisplayRole).toDouble();
    }
}
