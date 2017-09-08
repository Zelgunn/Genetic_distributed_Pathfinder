#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include "maiaXmlRpcClient.h"
#include "maiaXmlRpcServer.h"
#include "village.h"

class Client : QObject
{
    Q_OBJECT
public:
    Client(QObject *parent = 0);
    ~Client();
    void init();
    void initServer();

private slots:
    void setConnectionPort(QVariant &);
    QString updateGrid(QString args);

    QString requestGeneticProcess(QString args);

    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void connectionError(int error, const QString &message);

private:
    MaiaXmlRpcClient *m_rpc;
    int m_connectionPort;
    Grille m_grid;
    MaiaXmlRpcServer *m_server;
};

#endif // CLIENT_H
