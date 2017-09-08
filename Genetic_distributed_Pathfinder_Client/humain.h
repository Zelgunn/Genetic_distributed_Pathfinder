#ifndef HUMAIN_H
#define HUMAIN_H

#include <QDebug>
#include <QTime>
#include <QPoint>
#include "include.h"

/*
 * Un humain se dirige dans les directions suivantes en fonction de son ADN :
 *      3   2   1
 *      4   x   0
 *      5   6   7
 * On accède à la modification d'index via la fonction statique
 */

class Humain
{
public:
    Humain();
    ~Humain();
    void randomizeDNA();
    Humain makeChildWith(Humain otherParent);
    void setDNAAt(int i, char v);
    char getDNAAt(int i);
    static QPoint getOffset(char v);

private:
    char m_dna[DNA_LEN];
};

#endif // HUMAIN_H
