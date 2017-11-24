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

void Operator::setMinTime(double minTime)
{
    m_minTime = minTime;
}

void Operator::setMaxTime(double maxTime)
{
    m_maxTime = maxTime;
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

void Operators::setOpsTimes(double min1, double max1, double min2, double max2, double min3, double max3)
{
    m_ops[0]->setMinTime(min1);
    m_ops[0]->setMaxTime(max1);
    m_ops[1]->setMinTime(min2);
    m_ops[1]->setMaxTime(max2);
    m_ops[2]->setMinTime(min3);
    m_ops[2]->setMaxTime(max3);
}
