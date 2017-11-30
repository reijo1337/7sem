#include "clients.h"
#include "base.h"

Clients::Clients(int count, double min, double max)
{
    Q_ASSERT_X(count > 0, "setting parameters in clients", "num of clients must be >0");
    m_count = count;
    m_minTime = min;
    m_maxTime = max;
    m_currentTime = 0;
}

void Clients::proc()
{
    if (m_currentTime <= 0) {
        m_currentTime = ravnRand(m_minTime, m_maxTime);
        nextRequest();
    } else {
        m_currentTime -= 0.01;
    }
}

void Clients::nextRequest()
{
    if (m_count > 0) {
        m_count--;
        emit sendRequest();
    }
}

bool Clients::done()
{
    return (m_count == 0) && (m_currentTime <= 0);
}
