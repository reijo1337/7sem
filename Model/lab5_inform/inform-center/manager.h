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

    void setOpsTimes(double min1, double max1,
                     double min2, double max2,
                     double min3, double max3);

    void setCompsTime(double comp1, double comp2);

private:
    Clients *m_clients;
    Operators *m_operators;
    Computer *m_firstComp;
    Computer *m_secondComp;

    double m_time;
};

#endif // MANAGER_H
