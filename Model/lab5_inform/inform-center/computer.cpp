#include "computer.h"
#include "base.h"

void Computer::recvRequest()
{
    m_queue++;
}

Computer::Computer(int time)
{
    m_serveTime = time;
    m_currentTime = 0;
    m_queue = 0;
}

void Computer::proc()
{
    if (m_currentTime > 0) {
        m_currentTime -= 0.01;
    }
    if (m_currentTime <= 0 && m_queue > 0) {
        m_currentTime = m_serveTime;
        m_queue--;
    }
}

bool Computer::done()
{
    return (m_queue == 0) && (m_currentTime <= 0);
}
