#ifndef VILLAGE_H
#define VILLAGE_H

#include "humain.h"
#include "grille.h"
#include "include.h"

class Village
{
public:
    Village();
    Village(QStringList s, Grille* grid);
    ~Village();
    void initVillage(Grille* grid);

    QList<Humain> villagers() const;
    Humain villagerAt(int i) const;

    int rateVillager(int i) const;
    void proceedNextGen();

    void exchangeVillagers(Village *target);
    Humain takeVillager(int i=-1);
    void giveVillagerTo(Village *receiver, int i=-1);
    void addVillager(Humain villager);
    void setGrid(Grille *grid);

    QString parseVillage();

private:
    QList<Humain> m_villagers;
    Grille *m_grid;
};

#endif // VILLAGE_H
