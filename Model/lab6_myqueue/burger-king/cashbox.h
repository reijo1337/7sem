#ifndef CASHBOX_H
#define CASHBOX_H

#include <QObject>

class Cashbox : public QObject
{
    Q_OBJECT
public:
    Cashbox(int min, int max);

    bool isFree() const;

    void recvNewClient();

    void proc();

private:
    bool m_isFree;
    double m_minTime;
    double m_maxTime;
    double m_currentTime;
};

#endif // CASHBOX_H
