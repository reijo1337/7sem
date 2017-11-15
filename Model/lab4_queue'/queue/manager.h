#ifndef MANAGER_H
#define MANAGER_H

#include "generator.h"
#include "processor.h"

class Manager
{
public:
    Manager(Generator *gen, Processor *proc, int req);

    void deltaT(double part, double dt);
    void event(double part);

    int getTotalRequests();
    int getMaxQueueSize();

private:
    Generator *m_gen;
    Processor *m_proc;
    int m_requests;

    double m_genNext;
    double m_prNext;
    double m_part;
};

#endif // MANAGER_H
