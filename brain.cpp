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
    init();
    while(end)
    {
        QVector<float> coeff;
        for (int i = 0; i < 6 ; i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
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
            listNeuron[i]->setInput(input);
            result.push_back((float)listNeuron[i]->think());
        }
        QStringList first = dataPoney[currentPrice]->arrive.split("-");
        nbTry++;
        if(nbTry%10000 == 0)
        {
            //qDebug() << QString::number(nbSuccess) + "/" + QString::number(nbTry);
            emit somethingToSay(id,QString::number(nbSuccess) + "/" + QString::number(nbTry));
        }
        if(tri(result) == first[0].toInt())
        {
            nbSuccess++;
        }
        if(currentPrice  == dataPoney.length() -1)
        {
            if(currentDate.toString("yyyy-MM-dd") == "2015-03-09")
            {
               end = true;
               qDebug() << "no more data";
            }
            end = true;
            emit wantMoreData(id);
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
    for (int i = 0; i < 6 ; i++)
    {
        float frandom = ((float)rand() / (float)RAND_MAX);
        coeff.push_back(frandom);
    }
    for (int i = 0; i< 20 ; i ++)
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
}
int Brain::tri(QVector<float> result)
{
    float top = 0;
    for (int i = 0; i < result.length() ; i++)
    {
        //qDebug() << result[i];
        if (result[i] > top)
        {
            top = result[i];
        }
    }
    int first = result.indexOf(top);
    //qDebug() << QString::number(first);
    return first;
}



