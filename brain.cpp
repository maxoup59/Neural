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
    bool end = true;
    while(end)
    {
        listNeuron.clear();
        //On recrée les neurones quand on a pas le résultat : pas bon
        QVector<float> coeff;
        for (int i = 0; i < 6 ; i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        for (int i = 0; i< dataPoney[currentPrice]->nbOfPoney ; i ++)
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
            listNeuron[i]->setInput(input);
            result.push_back((float)listNeuron[i]->think());
        }
        QStringList first = dataPoney[currentPrice]->arrive.split("-");
        nbTry++;
        qDebug() << QString::number(id) + "nbTry : " + QString::number(nbTry);
        //qDebug() << first[0];
        if(tri(result) == first[0].toInt())
        {
            qDebug() << "Result found by " + QString::number(id);
            nbSuccess++;
            if(currentPrice == dataPoney.length())
            {
                end = true;
                emit wantMoreData(id);
            }
            else
            {
                currentPrice++;
            }
        }
        else
        {

        }
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

