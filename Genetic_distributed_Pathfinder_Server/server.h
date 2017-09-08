#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "maiaXmlRpcServer.h"
#include "maiaXmlRpcClient.h"
#include "world.h"
#include "include.h"
#define SVR_PRT 8081

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(World* world, QObject *parent = 0);
    ~Server();
    void processOnClients(int iterationCount);
    void updateClientsGrid();
    void removeDisconnectedClients();
    int connectionCount();

private slots:
    QString addConnection(int x);
    void requestGP(QVariant &arg);
    void emptySlot();

    void handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void connectionError(int error, const QString &message);

signals:
    void connectionCountChanged(int);
    void allClientsAreDone();

private:
    World *m_world;
    MaiaXmlRpcServer *m_server;
    QList<int> m_connections;
    QList<int> m_remainingConnections;
    int m_expectedConnectionsCount;
    QList<MaiaXmlRpcClient*> m_clients;
    int m_nextPort;
};

#endif // SERVER_H
