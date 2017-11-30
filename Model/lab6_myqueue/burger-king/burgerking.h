#ifndef BURGERKING_H
#define BURGERKING_H

#include <QObject>
#include <QVector>

#include "cashbox.h"

class BurgerKing : public QObject
{
    Q_OBJECT
public:
    BurgerKing(int num);

    void proc();

    bool done();

    int queue() const;

    QVector<Cashbox *> cashes() const;

private:
    QVector<Cashbox *> m_cashes;
    int m_queue;

public slots:
    void recvRequest();
};

#endif // BURGERKING_H
