#include <iostream>

#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QFile>

#include "baseconv.h"
#include "enigma.h"

using namespace std;

int main(int argc, char *argv[])
{
    BaseConv basis;
    Enigma enigma;

    if (argc < 2) {
        cout << "Input string:" << endl;
        QTextStream qtin(stdin);
        QString str = qtin.readLine();

        QString enc = basis.toBase(str);
        cout << "Encoded: " << enc.toStdString() << endl;

        QString enigmed = enigma.cryptString(enc, basis.splitter()[0]);
        cout << "Enigmed: " << enigmed.toStdString() << endl;

        QString deenigmed = enigma.decryptString(enigmed, basis.splitter()[0]);
        cout << "Deenigmed: " << deenigmed.toStdString() << endl;

        QString dec = basis.fromBase(deenigmed);
        cout << "Decoded: " << dec.toStdString() << endl;
    } else {
        QString fileName(argv[1]);
        QStringList fileNameParts = fileName.split(".");
        QString prefix = "";
        if (fileNameParts.length() > 1) {
            prefix = "." + fileNameParts.last();
            fileNameParts.pop_back();
        }

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            return 1;

        QByteArray fileRaw = file.readAll();

        QString str(fileRaw.toBase64());

        QString enc = basis.toBase(str);

        QString enigmed = enigma.cryptString(enc, basis.splitter()[0]);
        QFile enig(fileNameParts.join(".") + "Enigmed" + prefix);
        enig.setPermissions(file.permissions());
        enig.open(QIODevice::WriteOnly);
        enig.write(enigmed.toUtf8());

        QString deenigmed = enigma.decryptString(enigmed, basis.splitter()[0]);
        QFile deenig(fileNameParts.join(".") + "Deenigmed" + prefix);
        deenig.setPermissions(file.permissions());
        deenig.open(QIODevice::WriteOnly);
        deenig.write(deenigmed.toUtf8());

        QString dec = basis.fromBase(deenigmed);
        QByteArray rest = QByteArray::fromBase64(dec.toUtf8());
        QFile restored(fileNameParts.join(".") + "Restored" + prefix);

        restored.setPermissions(file.permissions());

        restored.open(QIODevice::WriteOnly);
        restored.write(rest);
    }
}
