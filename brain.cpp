#include "brain.h"
#include <cstdlib>
#include <QDebug>
Brain::Brain()
{
    currentDate = 0;
    currentPrice = 0;
    nbTry = 0;
    nbSuccess = 0;
}

Brain::~Brain()
{

}

void Brain::run()
{

    init();

    for (int day = 0 ; day < dataPoney.length() ; day++)
    {
        QVector<float> coeff;
        for (int i = 0; i < dataPoney[day][currentPrice]->nbOfPoney ; i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        for (int i = 0; i< dataPoney[day][currentPrice]->nbOfPoney ; i ++)
        {
            listNeuron[i]->setCoeff(coeff);
        }
        QVector<float> result;
        for (int i = 0 ; i < dataPoney[day][currentPrice]->nbOfPoney ; i ++)
        {
            QVector<float> input;
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->ratioPoney);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->ratioJockey);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->ratioTrainer);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->sexe);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->age);

            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->CoteDirect);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->CoteProb);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->CourueEntraineurJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->CourueJockeyJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->DerniereCote);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->DernierePlace);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->MonteEntraineurJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->MontesduJockeyJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->NbCoursePropJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->Recence);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->VictoireEntraineurJour);
            input.push_back((float)dataPoney[day][currentPrice]->listOfPoney[i]->VictoireJocKeyJour);

            listNeuron[i]->setInput(input);
            result.push_back((float)listNeuron[i]->think());
        }
        QStringList first = dataPoney[day][currentPrice]->arrive.split("-");
        nbTry++;
        if(nbTry%100 == 0)
        {
            qDebug() << "ID : " + QString::number(id) + "  " + QString::number(nbSuccess) + "/" + QString::number(nbTry);
        }
        int firstBrain = tri(result);
        if(firstBrain == first[0].toInt())
        {
            nbSuccess++;
        }
        if(currentPrice  == dataPoney.length() -1)
        {
            currentPrice = 0;
            currentDate++;
        }
        else
        {
            currentPrice++;
        }
    }
}
void Brain::init()
{
    QVector<float> coeff;
    for (int i = 0; i < 25 ; i++)
    {
        float frandom = ((float)rand() / (float)RAND_MAX);
        coeff.push_back(frandom);
    }
    for (int i = 0; i< 30 ; i ++)
    {
        Neuron * temp = new Neuron();
        temp->setCoeff(coeff);
        listNeuron.push_back(temp);
    }
    QVector<float> result;
    for (int i = 0 ; i < dataPoney[0][currentPrice]->nbOfPoney ; i ++)
    {
        QVector<float> input;
        input.push_back(dataPoney[0][currentPrice]->listOfPoney[i]->ratioPoney);
        input.push_back(dataPoney[0][currentPrice]->listOfPoney[i]->ratioJockey);
        input.push_back(dataPoney[0][currentPrice]->listOfPoney[i]->ratioTrainer);
        input.push_back(dataPoney[0][currentPrice]->listOfPoney[i]->sexe);
        input.push_back(dataPoney[0][currentPrice]->listOfPoney[i]->age);

        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->CoteDirect);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->CoteProb);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->CourueEntraineurJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->CourueJockeyJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->DerniereCote);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->DernierePlace);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->MonteEntraineurJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->MontesduJockeyJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->NbCoursePropJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->Recence);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->VictoireEntraineurJour);
        input.push_back((float)dataPoney[0][currentPrice]->listOfPoney[i]->VictoireJocKeyJour);

        listNeuron[i]->setInput(input);
        result.push_back((float)listNeuron[i]->think());
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



