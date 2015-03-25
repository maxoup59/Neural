#ifndef BRAIN_H
#define BRAIN_H

#include <QThread>
#include <QtGlobal>
#include "neuron.h"
#include "poney.h"
#include <QDate>
#include "price.h"
class Brain : public QThread
{
    Q_OBJECT
public:
    Brain();
    ~Brain();
    void run();
    void setFinDemandee(bool pFin){finDemandee=pFin;}
    void setNbPoney(int pNbPoney){nbPoney=pNbPoney;}
    QVector<QVector<Price*> > dataPoney;
    int expected;
    int id;
    int currentPrice;
    int currentDate;

signals:
    void cycleFinished(int);
    void wantMoreData(int id);
    void somethingToSay(int,QString);
private:
    bool finDemandee;
    int nbPoney;
    QVector<Neuron*> listNeuron;
    int tri(QVector<float>);
    int nbSuccess;
    int nbTry;
    void init();

};

#endif // BRAIN_H
