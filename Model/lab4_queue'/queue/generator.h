#ifndef GENERATOR_H
#define GENERATOR_H

#include "base.h"
#include "randomgenerator.h"

class Generator : public BaseGenerator
{
public:
    Generator(RandomGenerator *gen) : m_gen(gen) {}

    double nextGenerationPeriod() override;

private:
    RandomGenerator *m_gen;
};

#endif // GENERATOR_H
