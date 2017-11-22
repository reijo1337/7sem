#ifndef OPERATOR_H
#define OPERATOR_H

#include <QObject>
#include <QVector>

class Operator : public QObject
{
    Q_OBJECT
signals:
    void sendRequest();

public:
    Operator(int min, int max);

    bool isFree() const;

    void recvRequest();
    void proc();

private:
    bool m_isFree;
    double m_minTime;
    double m_maxTime;
    double m_currentTime;
};

class Operators : public QObject
{
    Q_OBJECT
signals:
    void sendFreeOp(int num);

public slots:
    void recvForFreeOp();
    void recvRequest(int num);

public:
    Operators();

    QVector<Operator *> ops() const;

    void proc();

    bool done();

private:
    QVector<Operator *> m_ops;
};

#endif // OPERATOR_H
