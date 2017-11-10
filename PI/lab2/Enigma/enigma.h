#ifndef ENIGMA_H
#define ENIGMA_H

#include <QString>
#include <QStringList>
#include <QVector>

class Enigma
{
public:
    Enigma();

    QString cryptString(QString string, QChar splitter);
    QString decryptString(QString string, QChar splitter);

private:
    void rotate(QVector<int> index);
    QChar crypt(QChar ch, QChar splitter, QVector<int> index);

    QStringList m_rotors;
    QString m_chars;
    QString m_reflector;

    QVector<int> m_rindex;
    QVector<int> m_drindex;
};

#endif // ENIGMA_H
