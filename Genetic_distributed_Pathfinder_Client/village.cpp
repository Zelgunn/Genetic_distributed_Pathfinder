#include "village.h"

Village::Village()
{

}

Village::Village(QStringList s, Grille *grid)
{
    Humain h;
    for(int i=0; i<VIL_CNT; i++)
    {
        for(int j=0; j<DNA_LEN; j++)
        {
            char c = s.at(i*DNA_LEN+j).at(0).toLatin1();
            h.setDNAAt(j, c-48);
        }
        m_villagers.append(h);
    }
    m_grid = grid;
}

Village::~Village()
{

}

void Village::initVillage(Grille *grid)
{
    m_grid = grid;
    m_villagers.clear();
    for(int i=0; i<VIL_CNT; i++)
    {
        Humain newVillager;
        newVillager.randomizeDNA();
        m_villagers.append(newVillager);
    }
}

QList<Humain> Village::villagers() const
{
    return m_villagers;
}

Humain Village::villagerAt(int i) const
{
    Q_ASSERT_X(i>=0&&i<VIL_CNT, "Village::villagerAt", "i : index out of range");
    return m_villagers.at(i);
}

int Village::rateVillager(int i) const
{
    Q_ASSERT_X(i>=0&&i<VIL_CNT, "Village::rateVillager", "i : index out of range");

    Humain villager = m_villagers.at(i);
    int w = m_grid->width(), h = m_grid->height(), res;
    QPoint pos = m_grid->depart();

    bool hasCollided = false, hasReachEnd = false;
    int j = 0;
    while(j<DNA_LEN&&!hasCollided&&!hasReachEnd)
    {
       pos += Humain::getOffset(villager.getDNAAt(j));
       hasCollided = pos.x()<0||pos.x()>=w||pos.y()<0||pos.y()>=h||m_grid->isPointInsideObstacles(pos);
       hasReachEnd = pos == m_grid->porte();
       j++;
    }
    pos -= m_grid->porte();
    res = pos.manhattanLength();
    if(hasCollided)
        res = res*5 + 200;
    res += j;
    return res;
}

void Village::proceedNextGen()
{
    // 1) Rate every villagers
    QList<QPoint> rates;
    QPoint rate;
    int minimum = 9999;
    for(int i=0; i<VIL_CNT; i++)
    {
        rate.setX(rateVillager(i));
        rate.setY(i);
        rates.append(rate);
        minimum = qMin(rate.x(), minimum);
    }

    QList<Humain> nextGen;
    int idxMin;
    while(!rates.isEmpty())
    {
        minimum = 9999;
        for(int i=0; i<rates.size(); i++)
        {
            rate = rates.at(i);
            if(minimum>rate.x())
            {
                idxMin = i;
                minimum = rate.x();
            }
        }
        nextGen.append(m_villagers.at(rates.at(idxMin).y()));
        rates.removeAt(idxMin);
    }
    // 2) Remove worst villagers (50%)
    for(int i=0; i<VIL_CNT/2; i++)
    {
        nextGen.pop_back();
    }

    // 3) Make new babies
    Humain child, parent1, parent2;
    for(int i=0; i<VIL_CNT/4; i++)
    {
        parent1 = nextGen.at(i);
        parent2 = nextGen.at(VIL_CNT/2 - i -1);
        child = parent1.makeChildWith(parent2);
        nextGen.append(child);
        child = parent1.makeChildWith(parent2);
        nextGen.append(child);
    }
    m_villagers = nextGen;
}

void Village::exchangeVillagers(Village *target)
{
    Humain villager = target->takeVillager();
    giveVillagerTo(target);
    m_villagers.append(villager);
}

Humain Village::takeVillager(int i)
{
    Q_ASSERT_X(i<VIL_CNT, "Village::takeVillager", "i : must be under VIL_CNT");

    if(i<0)
        i = qrand()%VIL_CNT;

    Humain villager = m_villagers.at(i);
    m_villagers.removeAt(i);

    return villager;
}

void Village::giveVillagerTo(Village *receiver, int i)
{
    Q_ASSERT_X(i<VIL_CNT, "Village::giveVillagerTo", "i : must be under VIL_CNT");

    if(i<0)
        i = qrand()%VIL_CNT;

    Humain villager = m_villagers.at(i);
    m_villagers.removeAt(i);

    receiver->addVillager(villager);
}

void Village::addVillager(Humain villager)
{
    m_villagers.append(villager);
}

void Village::setGrid(Grille *grid)
{
    m_grid = grid;
}

QString Village::parseVillage()
{
    Humain h;
    QString res;
    for(int i=0; i<VIL_CNT; i++)
    {
        h = villagerAt(i);
        for(int j=0; j<DNA_LEN; j++)
        {
            res.append(QString::number(h.getDNAAt(j)));
            res.append(',');
        }
    }
    return res;
}

