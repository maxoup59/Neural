#ifndef BRAIN_H
#define BRAIN_H

#include <QThread>
#include "neuron.h"
#include "price.h"
#include <cstdlib>
#include <QDebug>

class Brain : public QThread
{
    Q_OBJECT
public:
    Brain();
    ~Brain();

    void setDataDay(QVector<Price*> pData) { dataPoney = pData;}
    void setCoeff(QVector<float> pCoeff) { coeff = pCoeff;}
    void setID(int pID) {id = pID;}

    QVector<float> getCoeff() {return coeff;}
    float getRatio() {return (float)nbSuccess / (float)nbTry;}
    int getID() {return id;}

    void run();
    float optimalMutationRatio;

private:
    int NB_NEURON;

    int nbSuccess;
    int nbTry;
    int id;

    QVector<float> coeff;
    QVector<Price*> dataPoney;
    QVector<Neuron*> listNeuron;

    int tri(QVector<float>);
    void init();
    QVector<float> findRange(int price);
};

#endif // BRAIN_H
