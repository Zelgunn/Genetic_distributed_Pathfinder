#ifndef GRILLE_H
#define GRILLE_H

#include <QPoint>
#include <QList>
#include <QTime>
#include <QDebug>

#include "obstacle.h"

class Grille
{
public:
    Grille();
    Grille(int width, int height);
    Grille(QStringList args);
    ~Grille();
    void randomizeGrid();
    void loadGrid();
    bool isPointInsideObstacles(QPoint p);
    QPoint randomizePoint();

    int width() const;
    int height() const;
    int obstaclesCount() const;
    QPoint depart() const;
    QPoint porte() const;
    Obstacle obstacleAt(int i) const;

    void setDepart(QPoint p);
    void setPorte(QPoint p);
    void addObstacle(Obstacle o);

    QString parseGrid();

private:
    int m_width;
    int m_height;
    QPoint m_depart;
    QPoint m_porte;
    QList<Obstacle> m_obstacles;
};

#endif // GRILLE_H
