#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_image = Q_NULLPTR;
    m_worldWidth = 100;
    m_world.initWorld(m_worldWidth);
    m_s = new Server(&m_world);
    connect(m_s, SIGNAL(connectionCountChanged(int)), this, SLOT(setVillageCount(int)));
    connect(m_s, SIGNAL(allClientsAreDone()), this, SLOT(updateWorld()));

    setWindowTitle("Genetic, distributed Pathfinder (by Durand de Gevigney Valentin)");
    setWindowState(Qt::WindowMaximized);

    initUI();
    showGridOnUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // Status Bar
    QString s = "Villages count : 0";
    m_villageCount.setText(s);
    m_villageCount.setFrameShape(QFrame::Box);
    ui->statusBar->addPermanentWidget(&m_villageCount);

    s = "Obstacles count : ";
    s.append(QString::number(m_world.getGrid().obstaclesCount()));
    m_obstaclesCount.setText(s);
    m_obstaclesCount.setFrameShape(QFrame::Box);
    ui->statusBar->addPermanentWidget(&m_obstaclesCount);

    // Grid view
    if(ui->graphicsView->scene())
    {
        ui->graphicsView->scene()->clear();
        m_image = Q_NULLPTR;
    }
    else
        ui->graphicsView->setScene(new QGraphicsScene);
}

void MainWindow::showGridOnUI()
{
    Grille grid = m_world.getGrid();
    int w = grid.width(), h = grid.height();

    QImage image(w*5, h*5, QImage::Format_ARGB32);

    QPainter painter(&image);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::black);
    QRectF obs;
    for(int i=0; i<grid.obstaclesCount(); i++)
    {
        obs = grid.obstacleAt(i).toQRectF();
        obs.setCoords(5*obs.x(), 5*obs.y(), 5*(obs.x()+obs.width()), 5*(obs.y()+obs.height()));
        painter.drawEllipse(obs);
    }

    painter.drawRect(0,0,w*5-1,h*5-1);

    QPoint d=grid.depart(), p=grid.porte();
    painter.setBrush(Qt::blue);
    painter.drawRect(d.x()*5, d.y()*5, 5, 5);
    painter.setBrush(Qt::green);
    painter.drawRect(p.x()*5, p.y()*5, 5, 5);

    m_baseImage = image;
    m_image = ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::drawVillagersWay(Humain villager)
{
    QPoint pos = m_world.getGrid().depart();
    int w = m_world.getGrid().width(), h = m_world.getGrid().height();
    QImage image = m_baseImage;
    QPainter painter(&image);
    painter.setBrush(Qt::red);

    for(int i=0; i<DNA_LEN; i++)
    {
        pos += Humain::getOffset(villager.getDNAAt(i));
        if(pos.x()<0||pos.x()>=w||pos.y()<0||pos.y()>=h)
            break;
        if(pos==m_world.getGrid().porte())
            break;
        painter.drawRect(pos.x()*5, pos.y()*5, 5, 5);
    }
    m_image->setPixmap(QPixmap::fromImage(image));
}

World *MainWindow::world()
{
    return &m_world;
}

void MainWindow::setUIVillageCount(int count)
{
    QString s = "Villages count : ";
    s.append(QString::number(count));
    m_villageCount.setText(s);
}

void MainWindow::setVillageCount(int count)
{
    setUIVillageCount(count);
    m_world.createVillage();
}

void MainWindow::updateWorld()
{
    setUIVillageCount(m_s->connectionCount());
    int minimum = 9999, mindx = 0, tmp;
    qDebug() << "Ratings :";
    for(int i=0; i<m_world.getVillageCount(); i++)
    {
        tmp = m_world.villageAt(i).rateVillager(0);
        qDebug() << tmp;
        if(minimum>tmp)
        {
            minimum = tmp;
            mindx = i;
        }
    }
    drawVillagersWay(m_world.villageAt(mindx).villagerAt(0));
    ui->actionContinuer_la_g_n_ration->setEnabled(true);
}

void MainWindow::on_actionReg_n_rer_triggered()
{
    m_world.initWorld(m_worldWidth, m_world.getVillageCount());
    initUI();
    showGridOnUI();
    m_s->updateClientsGrid();
}

void MainWindow::on_actionContinuer_la_g_n_ration_triggered()
{
    if(m_s->connectionCount()==0)return;
    ui->actionContinuer_la_g_n_ration->setEnabled(false);
    m_s->processOnClients(ui->spinBox->value());
}
