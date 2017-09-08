#include <QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    qsrand(QTime::currentTime().msec());
    QCoreApplication a(argc, argv);

    Client c;

    return a.exec();
}
