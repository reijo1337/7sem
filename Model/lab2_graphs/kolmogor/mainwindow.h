#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QItemDelegate>
#include <QLineEdit>


class TableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableDelegate(QObject *parent = Q_NULLPTR) : QItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        Q_UNUSED(option)
        Q_UNUSED(index)
        QLineEdit *editor = new QLineEdit(parent);
        editor->setValidator(new QDoubleValidator(editor));
        return editor;
    }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_m_makeMatrix_clicked();

    void on_pushButton_clicked();

private:
    qreal getVal(int row, int col);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
