#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include "burgerking.h"
#include "clients.h"

class Manager : public QObject
{
    Q_OBJECT

signals:
    void sendData(int queue_size, bool free1, bool free2, bool free3, bool free4);

public:
    Manager();

    double time() const;

    void manage();

private:
    BurgerKing *m_bk;
    Clients *m_clients;

    double m_time;
};

#endif // MANAGER_H
