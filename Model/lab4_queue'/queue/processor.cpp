#include "processor.h"

void Processor::receiveRequest()
{
    m_qsize++;
    m_totalRequests++;
    if (m_qsize > m_maxQueueSize)
        m_maxQueueSize++;
}

Processor::Processor(RandomGenerator *gen)
{
    m_gen = gen;
    m_qsize = 0;
    m_totalRequests = 0;
    m_maxQueueSize = 0;
    m_test = 0;
}

double Processor::nextProcessingPeriod()
{
    double r = m_gen->expRand();
    if (r < 0)
        r = 0;
    return r;
}

void Processor::Process(double part)
{
    if (m_qsize > 0) {
        m_qsize--;
        if (RandomGenerator::myRand() <= part) {
            m_test++;
            emit sendReques();
        }
    }
}

int Processor::qsize() const
{
    return m_qsize;
}

int Processor::reentered() const
{
    return m_test;
}

int Processor::processedRequests() const
{
    return m_totalRequests;
}

int Processor::maxQueueSize() const
{
    return m_maxQueueSize;
}
