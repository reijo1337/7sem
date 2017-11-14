#include "generator.h"

double Generator::nextGenerationPeriod()
{
    double r = m_gen->ravnRand();
    if (r < 0)
        r = 0;
    return r;
}
