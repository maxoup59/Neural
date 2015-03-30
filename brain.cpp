#include "brain.h"

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
    optimalMutationRatio = 0;
    for (int price = 0 ;price < dataPoney.length() ; price++)
    {
        for (int poneyID = 0; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
        {
            listNeuron[poneyID]->setCoeff(coeff);
        }
        QVector<float> maximum = findRange(price);
        for (int poneyID = 0 ; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
        {
            QVector<float> input;
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioPoney);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioJockey);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->ratioTrainer);
            input.push_back(dataPoney[price]->listOfPoney[poneyID]->sexe);
            float test = (float)dataPoney[price]->listOfPoney[poneyID]->age/(float)maximum[0];
            input.push_back(test);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CoteDirect/maximum[1]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CoteProb/maximum[2]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CourueEntraineurJour/maximum[3]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->CourueJockeyJour/maximum[4]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->DerniereCote/maximum[5]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->DernierePlace/maximum[6]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->MonteEntraineurJour/maximum[7]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->MontesduJockeyJour/maximum[8]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->NbCoursePropJour/maximum[9]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->Recence/maximum[10]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->VictoireEntraineurJour/maximum[11]);
            input.push_back((float)dataPoney[price]->listOfPoney[poneyID]->VictoireJocKeyJour/maximum[12]);
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
        else
        {
            int nbPoney = dataPoney[price]->nbOfPoney;
            for (int i = 0 ;i < bestPoneyReal.length(); i++)
            {
                if(bestPoneyReal[i].toInt() == bestPoneySimulation)
                {
                    optimalMutationRatio += (float)i/((float)nbPoney-1);
                }
            }
        }
        nbTry++;
    }
    optimalMutationRatio /= dataPoney.length();
    optimalMutationRatio = 1 - optimalMutationRatio;
}

void Brain::init()
{
    nbTry = 0;
    nbSuccess = 0;
    listNeuron.clear();
    for (int neuron = 0 ; neuron < NB_NEURON ; neuron++)
    {
        Neuron* temp = new Neuron();
        temp->setCoeff(coeff);
        listNeuron.push_back(temp);
    }
}

QVector<float> Brain::findRange(int price)
{
    QVector<float> maximum;
    for (int i = 0 ;i < 13 ; i++)
    {
        maximum.push_back(0);
    }
    for (int poneyID = 0; poneyID < dataPoney[price]->nbOfPoney ; poneyID ++)
    {
        if (dataPoney[price]->listOfPoney[poneyID]->age > maximum[0])
        {
            maximum[0] = dataPoney[price]->listOfPoney[poneyID]->age;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->CoteDirect > maximum[1])
        {
            maximum[1] = dataPoney[price]->listOfPoney[poneyID]->CoteDirect;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->CoteProb > maximum[2])
        {
            maximum[2] = dataPoney[price]->listOfPoney[poneyID]->CoteProb;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->CourueEntraineurJour > maximum[3])
        {
            maximum[3] = dataPoney[price]->listOfPoney[poneyID]->CourueEntraineurJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->CourueJockeyJour > maximum[4])
        {
            maximum[4] = dataPoney[price]->listOfPoney[poneyID]->CourueJockeyJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->DerniereCote > maximum[5])
        {
            maximum[5] = dataPoney[price]->listOfPoney[poneyID]->DerniereCote;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->DernierePlace > maximum[6])
        {
            maximum[6] = dataPoney[price]->listOfPoney[poneyID]->DernierePlace;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->MonteEntraineurJour > maximum[7])
        {
            maximum[7] = dataPoney[price]->listOfPoney[poneyID]->MonteEntraineurJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->MontesduJockeyJour > maximum[8])
        {
            maximum[8] = dataPoney[price]->listOfPoney[poneyID]->MontesduJockeyJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->NbCoursePropJour > maximum[9])
        {
            maximum[9] = dataPoney[price]->listOfPoney[poneyID]->NbCoursePropJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->Recence > maximum[10])
        {
            maximum[10] = dataPoney[price]->listOfPoney[poneyID]->Recence;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->VictoireEntraineurJour > maximum[11])
        {
            maximum[11] = dataPoney[price]->listOfPoney[poneyID]->VictoireEntraineurJour;
        }
        else if(dataPoney[price]->listOfPoney[poneyID]->VictoireJocKeyJour > maximum[12])
        {
            maximum[12] = dataPoney[price]->listOfPoney[poneyID]->VictoireJocKeyJour;
        }
    }
    for (int i = 0 ; i < maximum.length() ; i++)
    {
        if(maximum[i] == 0)
        {
            maximum[i] = 1;
        }
    }
    return maximum;
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



