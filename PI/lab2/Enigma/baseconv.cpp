#include "baseconv.h"
#include <algorithm>
#include <QStringList>

QString BaseConv::toBase(QString string)
{
    QString res;

    Q_FOREACH (QChar chaR, string) {
        res += basealphaEncode(chaR.unicode()) + m_splitter;
    }

    return res;
}

QString BaseConv::fromBase(QString string)
{
    QString res;
    QStringList charlist = string.split(m_splitter);
    charlist.pop_back();
    Q_FOREACH(QString word, charlist) {
        QString rev;
        int size = word.size();
        rev.resize(size);
        rev.clear();
        for(int i=0; i<size; i++) rev[size-i - 1]=word[i];
        res += basealphaDecode(rev);
    }

    return res;
}

QString BaseConv::basealphaEncode(int binary)
{
    QString encoded;
    while (binary > 0) {
        encoded.push_front(m_chars.at(binary % m_base));
        binary /= m_base;
    }
    return encoded;
}

QString BaseConv::basealphaDecode(QString charset)
{
    int i =1;
    int res = 0;
    Q_FOREACH(QChar chaR, charset) {
        res += m_chars.indexOf(chaR) * i;
        i *= m_base;
    }

    return QString(QChar(res));
}

QString BaseConv::splitter() const
{
    return m_splitter;
}
