#include "manager.h"

#include <QObject>

Manager::Manager(Generator *gen, Processor *proc, int req) :
    m_gen(gen), m_proc(proc), m_requests(req)
{
    QObject::connect(gen, SIGNAL(sendReques()), proc, SLOT(receiveRequest()));
    QObject::connect(proc, SIGNAL(sendReques()), proc, SLOT(receiveRequest()));
}

void Manager::deltaT(double part, double dt)
{
    m_part = part;
    m_genNext = m_gen->nextGenerationPeriod();
    m_prNext = m_genNext + m_proc->nextProcessingPeriod();

    for (double curTime = 0; m_requests > 0; curTime += dt) {
        if (m_genNext <= curTime) {
            m_gen->emitRequest();
            m_genNext += m_gen->nextGenerationPeriod();
            m_requests--;
        }
        if (curTime >= m_prNext) {
            if (m_proc->qsize() > 0)
                m_proc->Process(m_part);
            if (m_proc->qsize() > 0) {
                m_prNext += m_proc->nextProcessingPeriod();
            } else {
                m_prNext = m_genNext + m_proc->nextProcessingPeriod();
            }
        }
    }
}

void Manager::event(double part)
{
    m_part = part;
    m_genNext = m_gen->nextGenerationPeriod();
    m_prNext = m_genNext + m_proc->nextProcessingPeriod();

    while (m_requests > 0) {
        if (m_genNext <= m_prNext) {
            m_gen->emitRequest();
            m_genNext += m_gen->nextGenerationPeriod();
            m_requests--;
        }
        if (m_genNext >= m_prNext) {
            if (m_proc->qsize() > 0)
                m_proc->Process(part);
            if (m_proc->qsize() > 0) {
                m_prNext += m_proc->nextProcessingPeriod();
            } else {
                m_prNext = m_genNext + m_proc->nextProcessingPeriod();
            }
        }
    }
}

int Manager::getTotalRequests()
{
    return m_proc->processedRequests();
}

int Manager::getMaxQueueSize()
{
    return m_proc->maxQueueSize();
}
