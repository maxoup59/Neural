#include "brain.h"
#include <cstdlib>
#include <QDebug>
Brain::Brain()
{
    currentDate.setDate(2015,01,01);
    currentPrice = 0;
    nbTry = 0;
    nbSuccess = 0;
}

Brain::~Brain()
{

}

void Brain::run()
{
    bool go = true;
    init();
    while(go)
    {
        for (int i = 0; i< dataPoney[currentPrice]->nbOfPoney ; i ++)
        {
            listNeuron[i]->setCoeff(coeff);
        }
        QVector<float> result;
        for (int i = 0 ; i < dataPoney[currentPrice]->nbOfPoney ; i ++)
        {
            QVector<float> input;
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->ratioPoney);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->ratioJockey);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->ratioTrainer);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->sexe);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->age);

            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CoteDirect);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CoteProb);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CourueEntraineurJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CourueJockeyJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->DerniereCote);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->DernierePlace);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->MonteEntraineurJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->MontesduJockeyJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->NbCoursePropJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->Recence);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->VictoireEntraineurJour);
            input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->VictoireJocKeyJour);

            listNeuron[i]->setInput(input);
            result.push_back((float)listNeuron[i]->think());
        }
        QStringList first = dataPoney[currentPrice]->arrive.split("-");
        // qDebug() << dataPoney[currentPrice]->arrive;
        nbTry++;
        if(nbTry%100 == 0)
        {
            qDebug() << "ID : " + QString::number(id) + "  " + QString::number(nbSuccess) + "/" + QString::number(nbTry);
            //emit somethingToSay(id,QString::number(nbSuccess) + "/" + QString::number(nbTry));
        }
        int firstBrain = tri(result);
        //qDebug() << QString::number(firstBrain) +" "+ first[0];
        if(firstBrain == first[0].toInt())
        {
            nbSuccess++;
        }
        if(currentPrice  == dataPoney.length() -1)
        {
            if(currentDate.toString("yyyy-MM-dd") == "2015-03-09")
            {
                go = false;
                qDebug() << "no more data";
                float ratio = (float ) nbSuccess/(float) nbTry;
                //emit noMoreData(listNeuron[0]->getCoeff(),ratio);
            }
            else
            {
                go = true;
                currentPrice = 0;
                //qDebug() << currentDate.toString("yyyy-MM-dd");
                currentDate = currentDate.addDays(1);
                //qDebug() << currentDate.toString("yyyy-MM-dd");
                emit wantMoreData(id);
            }
        }
        else
        {
            currentPrice++;
        }
    }
}
void Brain::init()
{
        currentPrice = 0;
    QVector<float> coeff;
    for (int i = 0; i < 25 ; i++)
    {
        float frandom = ((float)rand() / (float)RAND_MAX);
        coeff.push_back(frandom);
    }
    for (int i = 0; i< 50 ; i ++)
    {
        Neuron * temp = new Neuron();
        temp->setCoeff(coeff);
        listNeuron.push_back(temp);
    }
    QVector<float> result;
    for (int i = 0 ; i < dataPoney[currentPrice]->nbOfPoney ; i ++)
    {
        QVector<float> input;
        input.push_back(dataPoney[currentPrice]->listOfPoney[i]->ratioPoney);
        input.push_back(dataPoney[currentPrice]->listOfPoney[i]->ratioJockey);
        input.push_back(dataPoney[currentPrice]->listOfPoney[i]->ratioTrainer);
        input.push_back(dataPoney[currentPrice]->listOfPoney[i]->sexe);
        input.push_back(dataPoney[currentPrice]->listOfPoney[i]->age);

        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CoteDirect);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CoteProb);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CourueEntraineurJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->CourueJockeyJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->DerniereCote);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->DernierePlace);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->MonteEntraineurJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->MontesduJockeyJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->NbCoursePropJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->Recence);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->VictoireEntraineurJour);
        input.push_back((float)dataPoney[currentPrice]->listOfPoney[i]->VictoireJocKeyJour);

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



