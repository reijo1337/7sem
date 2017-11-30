#include "manager.h"

#include <QObject>

Manager::Manager()
{
    m_clients = new Clients(1000, 8, 22);
    m_bk = new BurgerKing(4);
    QObject::connect(m_clients, SIGNAL(sendRequest()), m_bk, SLOT(recvRequest()));
}

double Manager::time() const
{
    return m_time;
}

void Manager::manage()
{
    m_clients->proc();
    m_bk->proc();
    m_time += 0.01;
    emit sendData(m_bk->queue(),
                  m_bk->cashes()[0]->isFree(),
                  m_bk->cashes()[1]->isFree(),
                  m_bk->cashes()[2]->isFree(),
                  m_bk->cashes()[3]->isFree());

    while (!(m_clients->done() && m_bk->done())) {
        m_clients->proc();
        m_bk->proc();
        m_time += 0.01;
        emit sendData(m_bk->queue(),
                      m_bk->cashes()[0]->isFree(),
                      m_bk->cashes()[1]->isFree(),
                      m_bk->cashes()[2]->isFree(),
                      m_bk->cashes()[3]->isFree());
    }
}
