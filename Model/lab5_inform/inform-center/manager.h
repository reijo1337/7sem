#ifndef MANAGER_H
#define MANAGER_H

#include "clients.h"
#include "operator.h"
#include "computer.h"

class Manager
{
public:
    Manager();

    void manage();

    double time() const;

    int deniedReq() const;

    double deniedCoef();

private:
    Clients *m_clients;
    Operators *m_operators;
    Computer *m_firstComp;
    Computer *m_secondComp;

    double m_time;
};

#endif // MANAGER_H
