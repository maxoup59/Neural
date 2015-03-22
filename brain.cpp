#include "brain.h"
#include <cstdlib>

Brain::Brain()
{
    for (int i = 0; i< nbPoney ; i ++)
    {
        listNeuron.push_back(new Neuron());
    }
}

Brain::~Brain()
{

}

void Brain::run()
{
   // while(finDemandee)
    {
        QVector<float> coeff;

        for (int i = 0; i < 7/*NB INPUT*/;i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        for (int i = 0 ;i < nbPoney ; i++)
        {
            listNeuron[i]->setCoeff(coeff);
        }
        for (int i = 0 ; i < nbPoney ; i ++)
        {
            QVector<float> input;
            input.push_back(dataPoney[i]->ratioPoney);
            input.push_back(dataPoney[i]->ratioJockey);
            input.push_back(dataPoney[i]->ratioTrainer);
            input.push_back(dataPoney[i]->sexe);
            input.push_back(dataPoney[i]->age);
            listNeuron[i]->setInput(input);
        }

    }
}

