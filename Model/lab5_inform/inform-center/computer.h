#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>

class Computer : public QObject
{
    Q_OBJECT
public slots:
    void recvRequest();

public:
    Computer(int time);

    void proc();

    bool done();

private:
    int m_queue;
    int m_serveTime;
    double m_currentTime;
};

#endif // COMPUTER_H
