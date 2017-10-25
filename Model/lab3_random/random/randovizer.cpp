#include "randovizer.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <cmath>

#include <QDebug>

Randovizer::Randovizer(){
}

void Randovizer::generate()
{
    m_entrop.resize(6);
    genAld09();
    genAld1099();
    genAld100999();
    genTbl();

}

void Randovizer::fillAlgTbl(QTableWidget *tbl)
{
    for (int i = 1; i <= 10; i++) {
        tbl->setItem(i - 1, 0, new QTableWidgetItem(QString::number(m_algRnd09.at(i*100 - 1))));
        tbl->setItem(i - 1, 1, new QTableWidgetItem(QString::number(m_algRnd1099.at(i*100 - 1))));
        tbl->setItem(i - 1, 2, new QTableWidgetItem(QString::number(m_algRnd100999.at(i*100 - 1))));
    }

    tbl->setItem(10, 0, new QTableWidgetItem(QString::number(m_entrop[0]*100, 'f', 2) + "%"));
    tbl->setItem(10, 1, new QTableWidgetItem(QString::number(m_entrop[1]*100, 'f', 2) + "%"));
    tbl->setItem(10, 2, new QTableWidgetItem(QString::number(m_entrop[2]*100, 'f', 2) + "%"));
}

void Randovizer::fillTblTbl(QTableWidget *tbl)
{
    for (int i = 1; i <= 10; i++) {
        tbl->setItem(i - 1, 0, new QTableWidgetItem(QString::number(m_tblRnd09.at(i*100 - 1))));
        tbl->setItem(i - 1, 1, new QTableWidgetItem(QString::number(m_tblRnd1099.at(i*100 - 1))));
        tbl->setItem(i - 1, 2, new QTableWidgetItem(QString::number(m_tblRnd100999.at(i*100 - 1))));
    }

    tbl->setItem(10, 0, new QTableWidgetItem(QString::number(m_entrop[3] * 100, 'f', 2) + "%"));
    tbl->setItem(10, 1, new QTableWidgetItem(QString::number(m_entrop[4] * 100, 'f', 2) + "%"));
    tbl->setItem(10, 2, new QTableWidgetItem(QString::number(m_entrop[5] * 100, 'f', 2) + "%"));
}

void Randovizer::genAld09()
{
    m_algRnd09.clear();
    for (int i = 0; i < 1000; i++) {
        m_algRnd09.push_back(rand() % 10);
        emit valueChanged((m_algRnd09.length() +
                           m_algRnd1099.length() +
                           m_algRnd100999.length() +
                           m_tblRnd09.length() +
                           m_tblRnd1099.length() +
                           m_tblRnd100999.length()) / 6000);
    }

    m_entrop[0] = findEntrop(m_algRnd09);
}

void Randovizer::genAld1099()
{
    m_algRnd1099.clear();
    for (int i = 0; i < 1000; i++) {
        m_algRnd1099.push_back(10 + rand() % 90);
        emit valueChanged((m_algRnd09.length() +
                           m_algRnd1099.length() +
                           m_algRnd100999.length() +
                           m_tblRnd09.length() +
                           m_tblRnd1099.length() +
                           m_tblRnd100999.length()) / 6000);
    }

    m_entrop[1] = findEntrop(m_algRnd1099);
}

void Randovizer::genAld100999()
{
    m_algRnd100999.clear();
    for (int i = 0; i < 1000; i++) {
        m_algRnd100999.push_back(100 + rand() % 900);
        emit valueChanged((m_algRnd09.length() +
                           m_algRnd1099.length() +
                           m_algRnd100999.length() +
                           m_tblRnd09.length() +
                           m_tblRnd1099.length() +
                           m_tblRnd100999.length()) / 6000);
    }

    m_entrop[2] = findEntrop(m_algRnd100999);
}

void Randovizer::genTbl()
{
    m_tblRnd09.clear();
    m_tblRnd1099.clear();
    m_tblRnd100999.clear();

    QFile tblFile(":/digits.txt");
    if (!tblFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox msg;
        msg.setText("Can't open table");
        msg.exec();
        return;
    }

    QTextStream in(&tblFile);
    while (m_tblRnd09.length() < 1000 ||
           m_tblRnd1099.length() < 1000 ||
           m_tblRnd100999.length() < 1000) {
        QString line = in.readLine();
        if (line.isEmpty()){
            tblFile.close();
            tblFile.open(QIODevice::ReadOnly | QIODevice::Text);
            line = in.readLine();
        }
        QStringList nums = line.split(" ");
        for (int i = 1; i < nums.length(); i++) {
            int num = nums.at(i).toInt();

            if (0 <= num && num <= 9 && m_tblRnd09.length() < 1000) {
                m_tblRnd09.push_back(num);
                emit valueChanged((m_algRnd09.length() +
                                   m_algRnd1099.length() +
                                   m_algRnd100999.length() +
                                   m_tblRnd09.length() +
                                   m_tblRnd1099.length() +
                                   m_tblRnd100999.length()) / 6000);
            } else if (10 <= num && num <= 99 && m_tblRnd1099.length() < 1000) {
                m_tblRnd1099.push_back(num);
                emit valueChanged((m_algRnd09.length() +
                                   m_algRnd1099.length() +
                                   m_algRnd100999.length() +
                                   m_tblRnd09.length() +
                                   m_tblRnd1099.length() +
                                   m_tblRnd100999.length()) / 6000);
            } else if (100 <= num && num <= 999 && m_tblRnd100999.length() < 1000) {
                m_tblRnd100999.push_back(num);
                emit valueChanged((m_algRnd09.length() +
                                   m_algRnd1099.length() +
                                   m_algRnd100999.length() +
                                   m_tblRnd09.length() +
                                   m_tblRnd1099.length() +
                                   m_tblRnd100999.length()) / 6000);
            }
        }
        qDebug() << m_tblRnd09.length() << " " << m_tblRnd1099.length() << " " << m_tblRnd100999.length() << "\n";
    }

    tblFile.close();


    m_entrop[3] = findEntrop(m_tblRnd09);
    m_entrop[4] = findEntrop(m_tblRnd1099);
    m_entrop[5] = findEntrop(m_tblRnd100999);
}

qreal Randovizer::findEntrop(QVector<int> seq)
{
    QMap<int, int> ht;

    Q_FOREACH (int el, seq) {
        if (!ht.contains(el))
            ht[el] = 0;
        ht[el] += 1;
    }

    qreal entr = 0;

    Q_FOREACH(int val, ht.values()) {
       qreal p = (double) val / seq.length();
       entr -= p * log(p) / log(seq.length());
    }

    return entr;
}
