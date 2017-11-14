#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "randomgenerator.h"
#include "base.h"

class Processor : public BaseGenerator, BaseReceiver
{
public slots:
    void receiveRequest();

public:
    Processor(RandomGenerator *gen);

    double nextGenerationPeriod() override {return 0.0;}
    double nextProcessingPeriod() override;

    void Process(double part) override;

    int qsize() const;

    int reentered() const;

    int processedRequests() const;

    int maxQueueSize() const;

private:
    RandomGenerator *m_gen;

    int m_qsize;

    int m_totalRequests;
    int m_maxQueueSize;

    int m_test;
};

#endif // PROCESSOR_H
