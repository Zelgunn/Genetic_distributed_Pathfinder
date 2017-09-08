#include "grille.h"

Grille::Grille()
{

}

Grille::Grille(int width, int height)
{
    m_width = width;
    m_height = height;
}

Grille::Grille(QStringList args)
{
    // Grille
    m_width = args.at(0).toInt();
    m_height = args.at(1).toInt();

    // // Depart
    m_depart = QPoint(args.at(2).toInt(), args.at(3).toInt());

    // // Porte
    m_porte = QPoint(args.at(4).toInt(), args.at(5).toInt());

    // // Obstacles
    int obstaclesCount = args.at(6).toInt();
    Obstacle o;
    for(int i=0; i<obstaclesCount; i++)
    {
        o = Obstacle(args.at(i*3+7).toInt(), args.at(i*3+8).toInt(), args.at(i*3+9).toInt());
        m_obstacles.append(o);
    }
}

Grille::~Grille()
{

}

void Grille::randomizeGrid()
{
    qsrand(QTime::currentTime().msec());
    int nbObstacles = 1+qrand()%5;  // Un à Cinq obstacles
    m_obstacles.clear();
    Obstacle tmp;
    for(int i=0; i<nbObstacles; i++)
    {
        tmp.randomizeObstacle(m_width, m_height);
        m_obstacles.append(tmp);
    }
    m_depart = randomizePoint();
    QPoint p(qMax(m_width-m_depart.x(), m_depart.x()), qMax(m_height-m_depart.y(), m_depart.y()));
    int miniDist = p.manhattanLength()*3/4;
    p = QPoint(0,0);
    while(p.manhattanLength()<miniDist)
    {
        m_porte = randomizePoint();
        p = m_depart-m_porte;
    }
}

void Grille::loadGrid()
{

}

bool Grille::isPointInsideObstacles(QPoint p)
{
    Obstacle tmp;
    for(int i=0; i<m_obstacles.size(); i++)
    {
        // rendre isPointInside const
        if(m_obstacles.at(i).isPointInside(p)) return true;
    }
    return false;
}

QPoint Grille::randomizePoint()
{
    QPoint tmp;
    tmp.setX(qrand()%m_width);
    tmp.setY(qrand()%m_height);
    while(isPointInsideObstacles(tmp))
    {
        tmp.setX(qrand()%m_width);
        tmp.setY(qrand()%m_height);
    }
    return tmp;
}

int Grille::width() const
{
    return m_width;
}

int Grille::height() const
{
    return m_height;
}

int Grille::obstaclesCount() const
{
    return m_obstacles.size();
}

QPoint Grille::depart() const
{
    return m_depart;
}

QPoint Grille::porte() const
{
    return m_porte;
}

Obstacle Grille::obstacleAt(int i) const
{
    Q_ASSERT_X(i>=0&&i<m_obstacles.size(), "Grille::obstacleAt", "i : index out of range");
    return m_obstacles.at(i);
}

void Grille::setDepart(QPoint p)
{
    m_depart = p;
}

void Grille::setPorte(QPoint p)
{
    m_porte = p;
}

void Grille::addObstacle(Obstacle o)
{
    m_obstacles.append(o);
}

QString Grille::parseGrid()
{
    int obstaclesCount = m_obstacles.size();
    QString res;
    res.append(QString::number(m_width)); res.append(',');
    res.append(QString::number(m_height)); res.append(',');
    res.append(QString::number(m_depart.x())); res.append(',');
    res.append(QString::number(m_depart.y())); res.append(',');
    res.append(QString::number(m_porte.x())); res.append(',');
    res.append(QString::number(m_porte.y())); res.append(',');
    res.append(QString::number(obstaclesCount)); res.append(',');
    for(int i=0; i<obstaclesCount; i++)
    {
        res.append(QString::number(obstacleAt(i).origine().x())); res.append(',');
        res.append(QString::number(obstacleAt(i).origine().y())); res.append(',');
        res.append(QString::number(obstacleAt(i).radius())); res.append(',');
    }

    return res;
}
