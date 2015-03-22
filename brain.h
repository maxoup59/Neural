#ifndef BRAIN_H
#define BRAIN_H

#include <QThread>
#include <QtGlobal>
#include "neuron.h"
#include "poney.h"
class Brain : public QThread
{
public:
    Brain();
    ~Brain();
    void run();
    void setFinDemandee(bool pFin){finDemandee=pFin;}
    void setNbPoney(int pNbPoney){nbPoney=pNbPoney;}
    QVector<Poney*> dataPoney;
private:
    bool finDemandee;
    int nbPoney;
    QVector<Neuron*> listNeuron;

};

#endif // BRAIN_H
