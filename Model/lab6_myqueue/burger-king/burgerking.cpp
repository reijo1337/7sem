#include "burgerking.h"

BurgerKing::BurgerKing(int num)
{
    for (int i = 0; i < num; i++) {
        m_cashes.push_back(new Cashbox(15, 25));
    }
    m_queue = 0;
}

void BurgerKing::proc()
{
    Q_FOREACH(Cashbox *box, m_cashes) {
        if (box->isFree() && m_queue > 0) {
            m_queue--;
            box->recvNewClient();
        }

        box->proc();
    }
}

bool BurgerKing::done()
{
    bool ret = true;

    Q_FOREACH(Cashbox *op, m_cashes)
        ret &= op->isFree();

    return ret;
}

int BurgerKing::queue() const
{
    return m_queue;
}

QVector<Cashbox *> BurgerKing::cashes() const
{
    return m_cashes;
}

void BurgerKing::recvRequest()
{
    m_queue++;
}
