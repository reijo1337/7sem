#include "base.h"

double myRand()
{
    int accuracy = 2;
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}

double ravnRand(double a, double b)
{
    return (b - a) * myRand() + a;
}
