#include "enigma.h"

#include <QVector>

Enigma::Enigma() {
    m_chars = "abcdefghijklmnopqrstuvwxyz";

    m_rotors.append("ekmflgdqvzntowyhxuspaibrcj");  // rotor 1
    m_rotors.append("ajdksiruxblhwtmcqgznpyfvoe");  // rotor 2
    m_rotors.append("bdfhjlcprtxvznyeiwgakmusqo");  // rotor 3

    m_reflector = "yruhqsldpxngokmiebfzcwvjat";

    m_rindex = QVector<int>(3, 0);
    m_drindex = QVector<int>(3, 0);
}

QString Enigma::cryptString(QString string, QChar splitter)
{
    QString res;
    Q_FOREACH(QChar ch, string) {
        res += crypt(ch, splitter, m_rindex);
    }
    return res;
}

QString Enigma::decryptString(QString string, QChar splitter)
{
    QString res;
    Q_FOREACH(QChar ch, string) {
        res += crypt(ch, splitter, m_drindex);
    }
    return res;
}

void Enigma::rotate(QVector<int> index)
{
    index[2]++;
    if (index[2] == 26) {
        index[2] = 0;
        index[1]++;
        if (index[1] == 26) {
            index[1] = 0;
            index[0]++;
            if (index[0] == 26)
                index[0] = 0;
        }
    }
}

QChar Enigma::crypt(QChar ch, QChar splitter, QVector<int> index)
{
    if (ch == splitter)
        return splitter;

    rotate(index);
    QVector<int> in;
    QVector<QChar> out;

    // TyDa
    in.append((m_chars.indexOf(ch) + index[2]) % m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    out.append(m_rotors[0][in.last()]);

    in.append((m_chars.indexOf(out.last()) + index[1] - index[2]) % m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    out.append(m_rotors[1][in.last()]);

    in.append((m_chars.indexOf(out.last()) + index[0] - index[1]) % m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    out.append(m_rotors[2][in.last()]);

    in.append((m_chars.indexOf(out.last()) - index[0]) % m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    out.append(m_reflector[in.last()]);

    //CI0Da
    QChar chr;
    in.append((m_chars.indexOf(out.last()) + index[0])% m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    chr = m_chars[in.last()];
    out.append(m_rotors[2].indexOf(chr));

    in.append((out.last().unicode() + index[1] - index[0])% m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    chr = m_chars[in.last()];
    out.append(m_rotors[1].indexOf(chr));

    in.append((out.last().unicode() + index[2] - index[1])% m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    chr = m_chars[in.last()];
    out.append(m_rotors[0].indexOf(chr));

    in.append((out.last().unicode() - index[2]) % m_chars.length());
    if (in.last() < 0)
        in.last() += 26;
    chr = m_chars[in.last()];
    out.append(m_chars.indexOf(chr));

    return m_chars[out.last().unicode()];
}
