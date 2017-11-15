#ifndef GENERATOR_H
#define GENERATOR_H

#include <QObject>
#include "randomgenerator.h"

class Generator : public QObject
{
    Q_OBJECT
signals:
    void sendReques();

public:
    Generator(RandomGenerator *gen) : m_gen(gen) {}

    double nextGenerationPeriod();


    void emitRequest() {
        emit sendReques();
    }

private:
    RandomGenerator *m_gen;
};

#endif // GENERATOR_H
