#include "cashbox.h"
#include "base.h"

void Cashbox::recvNewClient()
{
    if (m_isFree) {
        m_isFree = false;
        m_currentTime = ravnRand(m_minTime, m_maxTime);
    }
}

void Cashbox::proc()
{
    if (!m_isFree) {
        m_currentTime -= 0.01;

        if (m_currentTime <= 0) {
            m_isFree = true;
        }
    }
}

Cashbox::Cashbox(int min, int max)
{
    m_isFree = true;
    m_maxTime = max;
    m_minTime = min;
    m_currentTime = 0;
}

bool Cashbox::isFree() const
{
    return m_isFree;
}
