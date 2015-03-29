#include "brain.h"
#include <cstdlib>
#include <QDebug>
Brain::Brain()
{
    nbTry = 0;
    nbSuccess = 0;
    NB_NEURON = 50;
}

Brain::~Brain()
{

}


void Brain::run()
{
    init();
    for (int price = 0 ;price < dataPoney.length() ; price++)
    {
        for (int poneyID = 0; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
        {
            listNeuron[poneyID]->setCoeff(coeff);
        }
        for (int poneyID = 0 ; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
        {
            QVector<float> input;
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioPoney);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioJockey);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioTrainer);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->sexe);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->age);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CoteDirect);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CoteProb);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CourueEntraineurJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CourueJockeyJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->DerniereCote);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->DernierePlace);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->MonteEntraineurJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->MontesduJockeyJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->NbCoursePropJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->Recence);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->VictoireEntraineurJour);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->VictoireJocKeyJour);
            listNeuron[poneyID]->setInput(input);
        }
        QVector<float> result;
        for (int poneyID = 0 ; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
        {
            result.push_back((float)listNeuron[poneyID]->think());
        }
        QStringList bestPoneyReal = dataPoney[price]->arrive.split("-");
        int bestPoneySimulation = tri(result);
        if (bestPoneySimulation == bestPoneyReal[0].toInt())
        {
            nbSuccess++;
        }
        nbTry++;
    }
}

void Brain::init()
{
    listNeuron.clear();
    for (int neuron = 0 ; neuron < NB_NEURON ; neuron++)
    {
        Neuron* temp = new Neuron();
        temp->setCoeff(coeff);
        listNeuron.push_back(temp);
    }
}

int Brain::tri(QVector<float> result)
{
    float top = 0;
    for (int i = 0; i < result.length() ; i++)
    {
        if (result[i] > top)
        {
            top = result[i];
        }
    }
    int first = result.indexOf(top);
    return first;
}



