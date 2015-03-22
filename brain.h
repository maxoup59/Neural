#ifndef BRAIN_H
#define BRAIN_H

#include <QThread>
#include <QtGlobal>
#include "neuron.h"
#include "poney.h"

class Brain : public QThread
{
    Q_OBJECT
public:
    Brain();
    ~Brain();
    void run();
    void setFinDemandee(bool pFin){finDemandee=pFin;}
    void setNbPoney(int pNbPoney){nbPoney=pNbPoney;}
    QVector<Poney*> dataPoney;
    int expected;
signals:
    void cycleFinished(int);
    void resultOKAY();
private:
    bool finDemandee;
    int nbPoney;
    QVector<Neuron*> listNeuron;
    int tri(QVector<float>);

};

#endif // BRAIN_H
