#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>

class Computer : public QObject
{
    Q_OBJECT
public slots:
    void recvRequest();

public:
    Computer(double time);

    void proc();

    bool done();

    void setTime(double time);

private:
    int m_queue;
    double m_serveTime;
    double m_currentTime;
};

#endif // COMPUTER_H
