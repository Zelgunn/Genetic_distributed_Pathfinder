#include "world.h"

World::World()
{

}

World::~World()
{

}

void World::initWorld(int width, int villageCount)
{
    Q_ASSERT_X(villageCount>=0, "World::initWorld", "villageCount : must be positive");
    Q_ASSERT_X(width>0, "World::initWorld", "width : must be positive");
    Q_ASSERT_X(width<DNA_LEN, "World::initWorld", "width : must be under DNA_LEN");

    int height = DNA_LEN - width;
    m_worldGrid = Grille(width, height);
    m_worldGrid.randomizeGrid();

    m_villages.clear();
    for(int i=0; i<villageCount; i++)
    {
        createVillage();
    }
}

int World::getVillageCount() const
{
    return m_villages.size();
}

Grille World::getGrid() const
{
    return m_worldGrid;
}

QList<Village> World::villages() const
{
    return m_villages;
}

Village World::villageAt(int i) const
{
    Q_ASSERT_X(i>=0&&i<m_villages.size(), "World::villageAt", "i : index out of range");
    return m_villages.at(i);
}

void World::createVillage()
{
    Village newVillage;
    newVillage.initVillage(&m_worldGrid);
    m_villages.append(newVillage);
}

void World::addVillage(Village v)
{
    v.setGrid(&m_worldGrid);
    m_villages.append(v);
}

void World::removeAllVillages()
{
    m_villages.clear();
}

void World::proceedNextGen()
{
    Village v;
    QList<Village> nextGen;
    for(int i=0; i<m_villages.size(); i++)
    {
        v = m_villages.at(i);
        v.proceedNextGen();
        nextGen.append(v);
    }
    m_villages = nextGen;
}

void World::proceedNextGen(int genCount)
{
    for(int i=0; i<genCount; i++)
        proceedNextGen();

    shareBlood();
}

void World::shareBlood()
{
    QList<Village*> nextVillages;
    Village* tmp;

    for(int i=0; i<m_villages.size(); i++)
    {
        tmp = new Village;
        *tmp = m_villages.at(i);
        nextVillages.append(tmp);
    }

    for(int i=0; i<nextVillages.size(); i++)
        for(int j=i+1; j<nextVillages.size(); j++)
            nextVillages.at(i)->exchangeVillagers(nextVillages.at(j));

    m_villages.clear();
    for(int i=0; i<nextVillages.size(); i++)
        m_villages.append(*(nextVillages.at(i)));
}

