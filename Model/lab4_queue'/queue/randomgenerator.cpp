#include "randomgenerator.h"

#include <QTime>
#include "qmath.h"

RandomGenerator::RandomGenerator(double a, double b, double lamda) :
    m_a(a), m_b(b), m_lambda(lamda)
{
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));
}

double RandomGenerator::ravnRand()
{
    return (m_b - m_a) * myRand() + m_a;
}

double RandomGenerator::expRand()
{
    return -1.0 / m_lambda * exp(1 - myRand());
}

double RandomGenerator::myRand()
{
    int accuracy = 17;
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}
