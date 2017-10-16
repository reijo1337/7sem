#include <QCoreApplication>
#include <iostream>

#include <QFile>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    system("wmic path win32_physicalmedia get SerialNumber >> key");

    QFile keyFile("iam1337pocan"), key("key");
    if (!key.open(QIODevice::ReadOnly | QIODevice::Text))
        return a.exec();
    if (!keyFile.open((QIODevice::ReadWrite | QIODevice::Text)))
        return a.exec();

    QByteArray hddKey = key.readAll();
    QByteArray verification = keyFile.readAll();

    if (verification.isEmpty()) {
        keyFile.write(hddKey);
        cout << "Registration complete!" << endl;
    } else if (verification == hddKey) {
        cout << "Nice! You are 1337 pocan!" << endl;
    } else {
        cout << "C'mon maaaan! You are not true 1337 pocan!\n" << endl;
    }

    keyFile.close();
    key.close();
    system("del key");

    return a.exec();
}
