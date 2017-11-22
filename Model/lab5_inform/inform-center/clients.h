#ifndef CLIENTS_H
#define CLIENTS_H

#include <QObject>
#include <QVector>

class Clients : public QObject
{
    Q_OBJECT
signals:
    void reqForFreeOp();
    void sendRequest(int num);

public slots:
    void recvFreeReq(int num);

public:
    Clients(int count, double min, double max);

    void proc();

    void nextRequest();

    bool done();

    int denied() const;
    double deniedCoef();

private:
    int m_count;        // Total count of clients
    int m_served;       // Served clients
    int m_denied;       // Denied clients

    double m_minTime;
    double m_maxTime;
    double m_currentTime;
};

#endif // CLIENTS_H
