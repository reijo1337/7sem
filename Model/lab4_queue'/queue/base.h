#ifndef BASE_H
#define BASE_H

#include <QObject>

class BaseGenerator : public QObject
{
    Q_OBJECT
signals:
    void sendReques();

public:
    virtual double nextGenerationPeriod() = 0;
    void emitRequest() {
        emit sendReques();
    }
};

class BaseReceiver : public QObject
{
    Q_OBJECT
public slots:
    virtual void receiveRequest() = 0;

public:
    virtual double nextProcessingPeriod() = 0;
    virtual void Process(double part) = 0;
};

#endif // BASE_H
