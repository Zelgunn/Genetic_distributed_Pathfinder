#include "humain.h"

Humain::Humain()
{

}

Humain::~Humain()
{

}

void Humain::randomizeDNA()
{
    //qsrand(QTime::currentTime().msec());
    for(int i=0; i<DNA_LEN; i++)
    {
        m_dna[i] = qrand()%8;
    }
}

Humain Humain::makeChildWith(Humain otherParent)
{
    Humain res;
    //qsrand(QTime::currentTime().msec());
    int sep = qrand()%DNA_LEN;
    for(int i=0; i<DNA_LEN; i++)
    {
        if(i<sep)
            res.setDNAAt(i, m_dna[i]);
        else
            res.setDNAAt(i, otherParent.getDNAAt(i));
    }
    // Mutation
    int mutation;
    for(int i=0; i<DNA_LEN*MUT_PCT/100; i++)
    {
        sep = qrand()%DNA_LEN;
        mutation = ((((qrand()%2)*2)-1) + res.getDNAAt(sep))%8;
        if(mutation==-1) mutation = 7;
        res.setDNAAt(sep, mutation);
    }
    return res;
}

void Humain::setDNAAt(int i, char v)
{
    Q_ASSERT_X(i>=0&&i<DNA_LEN, "Humain::setDNAAt", "i : index out of range");
    Q_ASSERT_X(v>=0&&v<=7, "Humain::getOffset", "Valeur \"v\" incorrecte");
    m_dna[i] = v;
}

char Humain::getDNAAt(int i)
{
    Q_ASSERT_X(i>=0&&i<DNA_LEN, "Humain::setDNAAt", "i : index out of range");
    return m_dna[i];
}

QPoint Humain::getOffset(char v)
{
    Q_ASSERT_X(v>=0&&v<=7, "Humain::getOffset", "Valeur \"v\" incorrecte");
    QPoint res;
    switch(v)
    {
    case 0:
        res = QPoint(1,0);
        break;
    case 1:
        res = QPoint(1,-1);
        break;
    case 2:
        res = QPoint(0,-1);
        break;
    case 3:
        res = QPoint(-1,-1);
        break;
    case 4:
        res = QPoint(-1,0);
        break;
    case 5:
        res = QPoint(-1,1);
        break;
    case 6:
        res = QPoint(0,1);
        break;
    default :   // 7
        res = QPoint(1,1);
        break;
    }
    return res;
}

