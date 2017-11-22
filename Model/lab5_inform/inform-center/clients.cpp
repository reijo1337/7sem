#include "clients.h"

#include "base.h"

void Clients::recvFreeReq(int num)
{
    if (num < 0) {
        m_denied++;
    } else {
        m_served++;
        emit sendRequest(num);
    }
}

Clients::Clients(int count, double min, double max)
{
    Q_ASSERT_X(count > 0, "setting parameters in clients", "num of clients must be >0");
    m_count = count;
    m_denied = 0;
    m_served = 0;
    m_minTime = min;
    m_maxTime = max;
    m_currentTime = 0;
}

void Clients::proc()
{
    if (m_currentTime <= 0) {
        m_currentTime = randomGen(m_minTime, m_maxTime);
        nextRequest();
    } else {
        m_currentTime -= 0.01;
    }
}

void Clients::nextRequest()
{
    if (m_count > 0) {
        m_count--;
        emit reqForFreeOp();
    }
}

bool Clients::done()
{
    return (m_count == 0) && (m_currentTime <= 0);
}

int Clients::denied() const
{
    return m_denied;
}

double Clients::deniedCoef()
{
    return (double) m_denied / (m_denied + m_served);
}
