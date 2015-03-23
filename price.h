#ifndef PRICE_H
#define PRICE_H
#include <QVector>
#include "poney.h"
#include <QString>
class Price
{
public:
    Price();
    ~Price();
    QVector<Poney*> listOfPoney;
    QString name;
    int nbOfPoney;
    QString arrive;
};

#endif // PRICE_H
