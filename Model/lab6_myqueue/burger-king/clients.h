#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>

class Clients : public QObject
{
    Q_OBJECT
signals:
    void sendRequest();

public:
    Clients(int count, double min, double max);

    void proc();

    bool done();

private:
    void nextRequest();

private:
    int m_count;

    double m_minTime;
    double m_currentTime;
    double m_maxTime;
};

#endif // CLIENTS_H
