#ifndef RANDOVIZER_H
#define RANDOVIZER_H

#include <QVector>
#include <QTableWidget>

class Randovizer : public QObject
{
    Q_OBJECT
signals:
    void valueChanged(int newVal);

public:
    Randovizer();
    void generate();
    void fillAlgTbl(QTableWidget *tbl);
    void fillTblTbl(QTableWidget *tbl);

    static qreal findEntrop(QVector<int> seq);

private:
    void genAld09();
    void genAld1099();
    void genAld100999();

    void genTbl();

private:
    QVector<int> m_algRnd09;
    QVector<int> m_algRnd1099;
    QVector<int> m_algRnd100999;

    QVector<int> m_tblRnd09;
    QVector<int> m_tblRnd1099;
    QVector<int> m_tblRnd100999;

    QVector<qreal> m_entrop;
};

#endif // RANDOVIZER_H
