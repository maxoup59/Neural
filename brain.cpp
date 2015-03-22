#include "brain.h"
#include <cstdlib>

Brain::Brain()
{
    for (int i = 0; i< nbPoney ; i ++)
    {
        listPoney.push_back(new Neuron());
    }
}

Brain::~Brain()
{

}

void Brain::run()
{
    while(finDemandee)
    {
        QVector<float> coeff;
        for (int i = 0; i < 7/*NB INPUT*/;i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        for (int i =0;i <nbPoney ; i++)
        {
            listPoney[i]->setCoeff(coeff);
        }
    }
}

