#include "brain.h"
#include <cstdlib>
#include <QDebug>
Brain::Brain()
{

}

Brain::~Brain()
{

}

void Brain::run()
{
   // while(finDemandee)
    {
        for (int i = 0; i< nbPoney ; i ++)
        {
            listNeuron.push_back(new Neuron());
        }
        QVector<float> coeff;
        for (int i = 0; i < nbPoney/*NB INPUT*/;i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        for (int i = 0 ;i < nbPoney ; i++)
        {
            listNeuron[i]->setCoeff(coeff);
        }
        QVector<float> result;
        for (int i = 0 ; i < nbPoney ; i ++)
        {
            QVector<float> input;
            input.push_back(dataPoney[i]->ratioPoney);
            input.push_back(dataPoney[i]->ratioJockey);
            input.push_back(dataPoney[i]->ratioTrainer);
            input.push_back(dataPoney[i]->sexe);
            input.push_back(dataPoney[i]->age);
            listNeuron[i]->setInput(input);
            result.push_back(listNeuron[i]->think());
        }
        tri(result);
    }
}

void Brain::tri(QVector<float> result)
{
    float top = 0;
    for (int i = 0; i < result.length() ; i++)
    {
        //qDebug() << result [i];
        if (result[i] > top)
        {
            top = result[i];
        }
    }
    int first = result.indexOf(top);
    qDebug() << first;
}

