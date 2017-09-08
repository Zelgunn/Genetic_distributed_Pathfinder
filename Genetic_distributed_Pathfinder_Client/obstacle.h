#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPoint>
#include <QTime>
#include <QRect>

#include "include.h"

class Obstacle
{
public:
    Obstacle();
    Obstacle(int x, int y, int r);
    ~Obstacle();
    void randomizeObstacle(int w, int h);
    bool isPointInside(const QPoint &p) const;

    QPoint origine() const;
    int radius() const;
    QRectF toQRectF() const;

private:
    QPoint m_origine;
    int m_radius;
};

#endif // OBSTACLE_H
