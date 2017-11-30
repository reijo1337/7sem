#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QTime>

#include "manager.h"

#include "burgerking.h"
#include "clients.h"

void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(5);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::redraw(int queue_size, bool free1, bool free2, bool free3, bool free4)
{
    ui->m_queue->setText("");
    ui->m_first->setVisible(false);
    ui->m_second->setVisible(false);
    ui->m_third->setVisible(false);
    ui->m_fourth->setVisible(false);

    QString queue = "";
    for (int i = 0; i < queue_size; i++)
        queue += "■ ";
    ui->m_queue->setText(queue);

    if (!free1)
        ui->m_first->setVisible(true);
    if (!free2)
        ui->m_second->setVisible(true);
    if (!free3)
        ui->m_third->setVisible(true);
    if (!free4)
        ui->m_fourth->setVisible(true);

    delay();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->m_queue->setText("");
    ui->m_first->setVisible(false);
    ui->m_second->setVisible(false);
    ui->m_third->setVisible(false);
    ui->m_fourth->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Clients *clients = new Clients(300, 2, 8);
    BurgerKing *bk = new BurgerKing(4);
    QObject::connect(clients, SIGNAL(sendRequest()), bk, SLOT(recvRequest()));

    clients->proc();
    bk->proc();

    redraw(bk->queue(),
                  bk->cashes()[0]->isFree(),
                  bk->cashes()[1]->isFree(),
                  bk->cashes()[2]->isFree(),
                  bk->cashes()[3]->isFree());

    while (!(clients->done() && bk->done())) {
        clients->proc();
        bk->proc();

        redraw(bk->queue(),
                      bk->cashes()[0]->isFree(),
                      bk->cashes()[1]->isFree(),
                      bk->cashes()[2]->isFree(),
                      bk->cashes()[3]->isFree());
    }
}
