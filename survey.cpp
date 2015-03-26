#include "survey.h"
#include "poney.h"
#include <QDebug>
#include "brain.h"
#include "price.h"
Survey::Survey(QString pFilename)
{
    filename = pFilename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    currentDate.setDate(2015,02,02);
    currentPrice =0;
    bestID = -1;
    bestRatio = 0;
}

Survey::~Survey()
{

}

void Survey::run()
{
    db.setDatabaseName(filename);
    if(!db.open() || !db.isValid())
    {
        qDebug() << "run : can't open DB";
    }
    else
    {
        qDebug() << "BDD OPEN";
    }

    for (int i = 0 ;i < 2 ; i ++)
    {
        QVector<float> coeff;
        for (int i = 0; i < 25 ; i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            coeff.push_back(frandom);
        }
        listBrain.push_back(new Brain());
        listBrain[i]->id = i;
        listBrain[i]->dataPoney = getCourseData(listBrain[i]->currentDate);
        listBrain[i]->coeff = coeff;
        connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
        connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
        connect(listBrain[i],SIGNAL(somethingToSay(int,QString)),this,SLOT(onSomethingToSay(int,QString)));
        listBrain[i]->run();
        float ratio = (float)listBrain[i]->nbSuccess / (float)listBrain[i]->nbTry;
        listRatio.push_back(ratio);
        listresult.push_back(listBrain[i]->coeff);
    }
    while(true)
    {
        int id = findTheBest();
        qDebug() << "The best is " + QString::number(id);
        for (int i = 0 ;i < 5 ; i ++)
        {
            QVector<float> coeff;
            for (int i = 0; i < 25 ; i++)
            {
                float frandom = ((float)rand() / ((float)RAND_MAX*10));
                coeff.push_back(listresult[id][i]+frandom);
            }
            listBrain[i] = (new Brain());
            listBrain[i]->id = i;
            listBrain[i]->dataPoney = getCourseData(listBrain[i]->currentDate);
            connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
            connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
            connect(listBrain[i],SIGNAL(somethingToSay(int,QString)),this,SLOT(onSomethingToSay(int,QString)));
            listBrain[i]->run();
            float ratio = (float)listBrain[i]->nbSuccess / (float)listBrain[i]->nbTry;
            listRatio.push_back(ratio);
            listresult.push_back(listBrain[i]->coeff);
        }
    }
}

QVector<Price *> Survey::getCourseData(QDate pCurrentDate)
{
    //Get list price of the day
    QVector<Price*> dataOfTheDay;
    //QVector<Poney*> data;
    QString queryGetDayPrice = "SELECT prix,count() FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui' group by prix";
    QSqlQuery getDayPrix;
    if(getDayPrix.exec(queryGetDayPrice))
    {
        while(getDayPrix.next())
        {
            Price *temp = new Price();
            temp->name = getDayPrix.value(0).toString();
            temp->nbOfPoney = getDayPrix.value(1).toString().toInt();
            QString queryGetData = "SELECT CoursesCheval,VictoiresCheval,PlacesCheval,CoursesEntraineur"
                                   ",VictoiresEntraineur,PlaceEntraineur,CoursesJockey,VictoiresJockey"
                                   ",PlaceJockey,SexeAge,partant,Arrive,CoteProb,CoteDirect"
                                   ",Recence,MontesduJockeyJour,CourueJockeyJour,VictoireJocKeyJour"
                                   ",MonteEntraineurJour,CourueEntraineurJour,VictoireEntraineurJour"
                                   ", DernierePlace,DerniereCote"
                                   ",NbCoursePropJour FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
                                                                                                                       " and prix ='" + temp->name +"'";
            //qDebug() << queryGetData;
            QSqlQuery getData;
            if(!getData.exec(queryGetData))
            {
                qDebug() << "Error query";
            }
            else
            {
                QVector<Poney *> data;
                for(int i = 0 ; i < temp->nbOfPoney; i++)
                {
                    data.push_back(new Poney());
                }
                int nb = 0;
                while(getData.next())
                {
                    float ratioPoney = (getData.value(1).toFloat()+getData.value(12).toFloat()) / (getData.value(0).toFloat());
                    float ratioTrainer = getData.value(4).toFloat() / getData.value(3).toFloat();
                    float ratioJockey = getData.value(7).toFloat() / getData.value(6).toFloat();
                    data[nb]->CoteDirect = getData.value(13).toFloat();
                    data[nb]->CoteProb = getData.value(12).toFloat();
                    data[nb]->CourueEntraineurJour = getData.value(19).toFloat();
                    data[nb]->CourueJockeyJour = getData.value(16).toFloat();
                    data[nb]->DerniereCote = getData.value(22).toFloat();
                    data[nb]->DernierePlace = getData.value(21).toFloat();
                    data[nb]->MonteEntraineurJour = getData.value(18).toFloat();
                    data[nb]->MontesduJockeyJour = getData.value(15).toFloat();
                    data[nb]->NbCoursePropJour = getData.value(23).toFloat();
                    data[nb]->VictoireEntraineurJour = getData.value(20).toFloat();
                    data[nb]->VictoireJocKeyJour = getData.value(17).toFloat();
                    data[nb]->Recence = getData.value(14).toFloat();
                    data[nb]->ratioPoney = ratioPoney;
                    data[nb]->ratioTrainer = ratioTrainer;
                    data[nb]->ratioJockey = ratioJockey;

                    if(getData.value(9).toString()[0] == 'M')
                    {
                        data[nb]->sexe = 1;
                    }
                    else
                    {
                        data[nb]->sexe = 0;
                    }
                    data[nb]->age = QString(getData.value(9).toString()[1]).toInt();
                    temp->arrive = getData.value(11).toString();
                    nb++;
                }
                temp->listOfPoney = data;
            }

            dataOfTheDay.push_back(temp);
        }
    }
    return dataOfTheDay;
}


void Survey::onCycleFinished(int value)
{
    qDebug() << "Cycle END";
}

/*void Survey::onNoMoreData(QVector<float> coeff, float ratio , int id)
{
    listresult.push_back(coeff);
    if(ratio > bestRatio)
    {
        bestRatio = ratio;
        bestID = id;
    }
    if(listresult.length() == 5)
    {

    }
}*/

void Survey::onWantMoreData(int id)
{
    qDebug() << "Brain nb : " +QString::number(id) + "Want more data";
    QDate brainCurrentDate = listBrain[id]->currentDate;
    qDebug() << brainCurrentDate.toString("yyyy-MM-dd");
    QVector<Price*> data = getCourseData(brainCurrentDate);
    listBrain[id]->dataPoney = data;
    listBrain[id]->expected = expected;
    listBrain[id]->run();
}

void Survey::onSomethingToSay(int value, QString message)
{
    emit somethingToSay(value,message);
}

int Survey::findTheBest()
{
    float best = 0;
    int id = -1;
    for(int i = 0 ;i < listRatio.length() ;i++)
    {
        if(listRatio[i] >= best)
        {
            best = listRatio[i];
            id = i;
        }
        qDebug() << "Ratio brain n :" + QString::number(i) + "is " + QString::number(listRatio[i]);
    }

    return id;
}


