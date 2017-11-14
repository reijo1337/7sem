#ifndef RANDOMGENERATOR_H
#define RANDOMGENERATOR_H


class RandomGenerator
{
public:
    RandomGenerator(double a, double b, double lamda);

    double ravnRand();
    double expRand();

    static double myRand();

private:
    double m_a, m_b, m_lambda;
};

#endif // RANDOMGENERATOR_H
