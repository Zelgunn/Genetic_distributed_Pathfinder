#include "obstacle.h"

Obstacle::Obstacle()
{

}

Obstacle::Obstacle(int x, int y, int r)
{
    m_origine.setX(x);
    m_origine.setY(y);
    m_radius = r;
}

Obstacle::~Obstacle()
{

}

void Obstacle::randomizeObstacle(int w, int h)
{
    int x = qrand()%w, y = qrand()%h;
    m_origine.setX(x);
    m_origine.setY(y);
    int maxRadius = qMax(qMin(qMin(x, w-x), qMin(y, h-y))-1, MIN_RAD+1);
    m_radius = MIN_RAD+qrand()%(maxRadius-MIN_RAD);
}

bool Obstacle::isPointInside(const QPoint &p) const
{
    // (xO-xP)²+(yO-yP)²<r²
    return (((m_origine.x()-p.x())*(m_origine.x()-p.x())+(m_origine.y()-p.y())*(m_origine.y()-p.y())) < (m_radius*m_radius));
}

QPoint Obstacle::origine() const
{
    return m_origine;
}

int Obstacle::radius() const
{
    return m_radius;
}

QRectF Obstacle::toQRectF() const
{
    QRectF res;
    res.setCoords(m_origine.x()-m_radius, m_origine.y()-m_radius, m_origine.x()+m_radius, m_origine.y()+m_radius);
    return res;
}

