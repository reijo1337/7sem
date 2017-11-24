#include "manager.h"

#include <QObject>

Manager::Manager()
{
    m_clients = new Clients(300, 8, 12);
    m_operators = new Operators();
    m_firstComp = new Computer(15);
    m_secondComp = new Computer(30);

    QObject::connect(m_clients, SIGNAL(reqForFreeOp()),
                     m_operators, SLOT(recvForFreeOp()));
    QObject::connect(m_operators, SIGNAL(sendFreeOp(int)),
                     m_clients, SLOT(recvFreeReq(int)));
    QObject::connect(m_clients, SIGNAL(sendRequest(int)),
                     m_operators, SLOT(recvRequest(int)));
    QObject::connect(m_operators->ops()[0], SIGNAL(sendRequest()),
            m_firstComp, SLOT(recvRequest()));
    QObject::connect(m_operators->ops()[1], SIGNAL(sendRequest()),
            m_firstComp, SLOT(recvRequest()));
    QObject::connect(m_operators->ops()[2], SIGNAL(sendRequest()),
            m_secondComp, SLOT(recvRequest()));

    m_time = 0;
}

void Manager::manage()
{
    m_clients->proc();
    m_operators->proc();
    m_firstComp->proc();
    m_secondComp->proc();
    m_time += 0.01;

    while (!(m_clients->done() && m_operators->done() &&
             m_firstComp->done() && m_secondComp->done())) {
        m_clients->proc();
        m_operators->proc();
        m_firstComp->proc();
        m_secondComp->proc();
        m_time += 0.01;
    }
}

double Manager::time() const
{
    return m_time;
}

int Manager::deniedReq() const
{
    return m_clients->denied();
}

double Manager::deniedCoef()
{
    return m_clients->deniedCoef();
}

void Manager::setOpsTimes(double min1, double max1, double min2, double max2, double min3, double max3)
{
    m_operators->setOpsTimes(min1, max1, min2, max2, min3, max3);
}

void Manager::setCompsTime(double comp1, double comp2)
{
    m_firstComp->setTime(comp1);
    m_secondComp->setTime(comp2);
}
