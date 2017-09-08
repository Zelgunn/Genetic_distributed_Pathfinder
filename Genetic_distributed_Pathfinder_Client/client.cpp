#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    m_rpc = new MaiaXmlRpcClient(QUrl("http://localhost:8081/RPC2"), this);
    m_server = Q_NULLPTR;

    QSslConfiguration config = m_rpc->sslConfiguration();
    config.setProtocol(QSsl::AnyProtocol);
    m_rpc->setSslConfiguration(config);

    connect(m_rpc, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
            this, SLOT(handleSslErrors(QNetworkReply *, const QList<QSslError> &)));
    init();
}

Client::~Client()
{

}

void Client::init()
{
    QVariantList args;
    args << QVariant(0);
    m_rpc->call("Server.addConnection", args,
              this, SLOT(setConnectionPort(QVariant &)),
                this, SLOT(connectionError(int, const QString &)));
}

void Client::initServer()
{
    m_server = new MaiaXmlRpcServer(m_connectionPort, this);
    qDebug() << "Registering on port" << m_connectionPort;
    m_server->addMethod("Client.requestGP", this, "requestGeneticProcess");
    m_server->addMethod("Client.updateGrid", this, "updateGrid");
}

void Client::setConnectionPort(QVariant &arg)
{
    QString tmp = arg.toString();
    QStringList argList = tmp.split(',');

    // Port
    m_connectionPort = argList.first().toInt();
    qDebug() << "Connection port:" << m_connectionPort;

    // Grille
    argList.removeFirst();
    m_grid = Grille(argList);

    qDebug() << "Finished to copy new grid";
    initServer();
}

QString Client::updateGrid(QString args)
{
    m_grid = Grille(args.split(','));
    qDebug() << "New grid setup done";

    return QString("Ok");
}

QString Client::requestGeneticProcess(QString args)
{
    QStringList argList = args.split(',');

    int iterationCount = argList.first().toInt();

    argList.removeFirst();

    Village v = Village(argList, &m_grid);
    for(int i=0; i<iterationCount; i++)
        v.proceedNextGen();

    qDebug() << "Rating of my best villager :" << v.rateVillager(0);

    QString res = QString::number(m_connectionPort).append(',');
    res.append(v.parseVillage());

    return res;
}

void Client::handleSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    qDebug() << "SSL Error:" << errors;
    reply->ignoreSslErrors();
}

void Client::connectionError(int error, const QString &message)
{
    qDebug() << "Error :" << error << "-" << message;
}
