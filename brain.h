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
    QVector<Price*> dataPoney;
    int expected;
    int id;
    int currentPrice;
    QDate currentDate;
    QVector<float> coeff;
    int nbSuccess;
    int nbTry;
signals:
    void cycleFinished(int);
    void wantMoreData(int id);
    void somethingToSay(int,QString);
private:
    bool finDemandee;
    int nbPoney;
    QVector<Neuron*> listNeuron;
    int tri(QVector<float>);

    void init();

};

#endif // BRAIN_H
