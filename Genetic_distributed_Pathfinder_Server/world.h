#ifndef WORLD_H
#define WORLD_H

#include "village.h"
#include "grille.h"

class World
{
public:
    World();
    ~World();
    void initWorld(int width, int villageCount=0);

    int getVillageCount() const;

    Grille getGrid() const;
    QList<Village> villages() const;
    Village villageAt(int i) const;
    void createVillage();
    void addVillage(Village v);
    void removeAllVillages();

    void proceedNextGen();
    void proceedNextGen(int genCount);
    void shareBlood();

private:
    QList<Village> m_villages;
    Grille m_worldGrid;
};

#endif // WORLD_H
