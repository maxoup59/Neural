#include "survey.h"
#include "poney.h"
#include <QDebug>
#include "brain.h"
#include "price.h"
Survey::Survey()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    currentDate.setDate(2015,02,02);
    currentPrice =0;
}

Survey::~Survey()
{

}

void Survey::run()
{
    db.setDatabaseName("test.dev.db");
    if(!db.open() || !db.isValid())
    {
        qDebug() << "run : can't open DB";
    }
    else
    {
        qDebug() << "BDD OPEN";
    }
    for (int i = 0 ;i < 5 ; i ++)
    {
        listBrain.push_back(new Brain());
        listBrain[i]->id = i;
        listBrain[i]->dataPoney = getCourseData();
        connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
        connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
        connect(listBrain[i],SIGNAL(somethingToSay(int,QString)),this,SLOT(onSomethingToSay(int,QString)));
        listBrain[i]->start();
    }
}

QVector<QVector<Price*> > Survey::getCourseData()
{
    QVector<QVector<Price*> > allDataInDB;
    QDate startDate(2015,01,01);
    QDate endDate(2015,03,10);
    while (startDate != endDate)
    {
        //Get list price of the day
        QVector<Price*> dataOfTheDay;
        //QVector<Poney*> data;
        QString queryGetDayPrice = "SELECT prix,count() FROM PoneyDB where jour = '"+startDate.toString("yyyy-MM-dd")+"' and PMU = 'oui' group by prix";
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
                                       ",NbCoursePropJour FROM PoneyDB where jour = '"+startDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
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
                        QString test = getData.value(21).toString();
                        //data[nb]->DernierePlace = test[0].;
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
        allDataInDB.push_back(dataOfTheDay);
        startDate = startDate.addDays(1);
        qDebug() << startDate.toString("yyyy-MM-dd");
    }
    return allDataInDB;
}

void Survey::onCycleFinished(int value)
{
    qDebug() << "Cycle END";
}



void Survey::onSomethingToSay(int value, QString message)
{
    emit somethingToSay(value,message);
}


