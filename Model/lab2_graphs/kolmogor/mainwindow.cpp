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
    qreal res[size], tb[size];

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size+1; j++)
            matrix[i][j] = 0;

    for (int row = 0; row < size; row++) {
        for (int i = 0; i < size; i++) {
            matrix[row][i] += getVal(row, i);
            matrix[row][row] -= getVal(i, row);
        }
        tb[row] = 0;
    }

    for (int i = 0; i < size; i++) {
        matrix[0][i] = 1;
    }
    tb[0] = 1;

    for (int i = 0; i < size - 1; i++) {
        // меняем текущую строку с той, у которой диагональный
        // элемент ненулевой и наибольший
        int imax = i;
        for (int j = i; j < size; j++) {
            if ((matrix[j][i] > matrix[imax][i]) && (matrix[j][i] != 0))
                imax = j;
        }
        if (imax != i) {
            double tmp;
            for (int t = 0; t < size; t++) {
                tmp = matrix[imax][t];
                matrix[imax][t] = matrix[i][t];
                matrix[i][t] = tmp;
            }
            tmp = tb[imax];
            tb[imax] = tb[i];
            tb[i] = tmp;
        }

        // теперь прямой ход по данной строке
        for (int k = i + 1; k < size; k++) {
            if (matrix[i][i] == 0)
                return;
            double divider = matrix[k][i] / matrix[i][i];
            for (int l = i; l < size; l++) {
                matrix[k][l] = matrix[k][l] - matrix[i][l] * divider;
            }
            tb[k] = tb[k] - tb[i] * divider;
        }
    }

    // все, прямой ход сделан, теперь идем обратно
    for (int i = size - 1; i >= 0; i--) {
        for (int j = i + 1; j < size; j++)
            tb[i] -= res[j] * matrix[i][j];
        if (matrix[i][i] == 0)
            return;
        res[i] = tb[i] / matrix[i][i];
    }

    QString result = "";
    for (int i = 0; i < size; i++) {
        result += "p(" + QString::number(i+1) + ") = " + QString::number(res[i]) + "\n";
    }

    QMessageBox msg;
    msg.setText(result);
    msg.setWindowTitle("Результат");
    msg.exec();
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
