#include "operator.h"

#include "base.h"

Operator::Operator(int min, int max)
{
    m_isFree = true;
    m_maxTime = max;
    m_minTime = min;
    m_currentTime = 0;
}

bool Operator::isFree() const
{
    return m_isFree;
}

void Operator::recvRequest()
{
    if (m_isFree) {
        m_isFree = false;
        m_currentTime = randomGen(m_minTime, m_maxTime);
    }
}

void Operator::proc()
{
    if (!m_isFree) {
        m_currentTime -= 0.01;

        if (m_currentTime <= 0) {
            m_isFree = true;
            emit sendRequest();
        }
    }
}

void Operators::recvForFreeOp()
{
    int num = -1;
    for (int i = 0; i < m_ops.length(); i++) {
        if (m_ops[i]->isFree()) {
            num = i;
            break;
        }
    }

    emit sendFreeOp(num);
}

void Operators::recvRequest(int num)
{
    m_ops[num]->recvRequest();
}

Operators::Operators()
{
    m_ops.push_back(new Operator(15, 25));
    m_ops.push_back(new Operator(30, 50));
    m_ops.push_back(new Operator(20, 60));
}

QVector<Operator *> Operators::ops() const
{
    return m_ops;
}

void Operators::proc()
{
    Q_FOREACH(Operator *op, m_ops)
        op->proc();
}

bool Operators::done()
{
    bool ret = true;

    Q_FOREACH(Operator *op, m_ops)
        ret &= op->isFree();

    return ret;
}
