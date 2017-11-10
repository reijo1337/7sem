#ifndef BASECONV_H
#define BASECONV_H

#include <QString>

class BaseConv
{
public:
    BaseConv() {
        m_chars = "abcdefghijklmnopqrstuvwxyz";
        m_base = m_chars.length();
        m_splitter = "!";
    }

    QString toBase(QString string);
    QString fromBase(QString string);

    QString splitter() const;

private:
    QString basealphaEncode(int binary);
    QString basealphaDecode(QString charset);

    QString m_chars;
    int m_base;
    QString m_splitter;
};

#endif // BASECONV_H
