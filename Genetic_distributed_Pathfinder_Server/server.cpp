#include "server.h"

Server::Server(World *world, QObject *parent) : QObject(parent)
{
    m_world = world;
    m_server = new MaiaXmlRpcServer(SVR_PRT, this);
    m_server->addMethod("Server.addConnection", this, "addConnection");
    m_nextPort = SVR_PRT+1;
}

Server::~Server()
{

}

void Server::processOnClients(int iterationCount)
{
    MaiaXmlRpcClient* client;
    QVariantList args;
    QString arg;
    Village v;
    Humain h;
    for(int i=0; i<m_connections.size(); i++)
    {
        args.clear();
        arg = QString::number(iterationCount).append(',');
        v = m_world->villageAt(i);
        QString s = v.parseVillage();
        arg.append(s);

        args << QVariant(arg);

        client = m_clients.at(i);
        client->call("Client.requestGP", args,
                     this, SLOT(requestGP(QVariant &)),
                     this, SLOT(connectionError(int, const QString &)));
    }
    m_expectedConnectionsCount = m_connections.count();
    m_world->removeAllVillages();
}

void Server::updateClientsGrid()
{
    MaiaXmlRpcClient* client;
    QString res = m_world->getGrid().parseGrid();
    QVariantList args;
    args << QVariant(res);

    for(int i=0; i<m_connections.size(); i++)
    {
        client = m_clients.at(i);
        client->call("Client.updateGrid", args,
                     this, SLOT(emptySlot()),
                     this, SLOT(emptySlot()));
    }
}

void Server::removeDisconnectedClients()
{
    QList<int> clientsToRemove;
    for(int i=0; i<m_connections.size(); i++)
    {
        if(!m_remainingConnections.contains(m_connections.at(i)))
        {
            clientsToRemove.append(i);
        }
    }

    for(int i=0; i<clientsToRemove.size(); i++)
    {
        m_clients.removeAt(i);
    }
}

int Server::connectionCount()
{
    return m_connections.size();
}

QString Server::addConnection(int x)
{
    Q_UNUSED(x);

    m_connections.append(m_nextPort);
    Grille grid = m_world->getGrid();
    QString res = QString::number(m_nextPort); res.append(',');
    res.append(grid.parseGrid());

    QString tmp = "http://localhost:";
    tmp.append(QString::number(m_nextPort));
    tmp.append("/RPC2");
    // "http://localhost:8081/RPC2"
    MaiaXmlRpcClient* newClient = new MaiaXmlRpcClient(QUrl(tmp), this);

    QSslConfiguration config = newClient->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    newClient->setSslConfiguration(config);

    connect(newClient, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
    m_clients.append(newClient);

    m_nextPort++;
    emit connectionCountChanged(m_connections.size());

    return res;
}

void Server::requestGP(QVariant &arg)
{
    QStringList s = arg.toString().split(',');
    int port = s.first().toInt();
    m_remainingConnections.append(port);
    m_expectedConnectionsCount--;

    s.removeFirst();

    Grille grid = m_world->getGrid();
    Village v(s, &grid);
    qDebug() << "Rating from last village :" << v.rateVillager(0);
    m_world->addVillage(v);

    qDebug() << "Still" << m_expectedConnectionsCount << "villages to wait.";
    if(m_expectedConnectionsCount==0)
    {
        removeDisconnectedClients();
        emit allClientsAreDone();
    }
}

void Server::emptySlot()
{

}

void Server::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors();
}

void Server::connectionError(int error, const QString &message)
{
    qDebug() << "Error :" << error << "-" << message;
    m_expectedConnectionsCount--;
    if(m_expectedConnectionsCount==0)
    {
        removeDisconnectedClients();
        emit allClientsAreDone();
    }
}
