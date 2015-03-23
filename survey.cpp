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
    //initCourseData();
    for (int i = 0 ;i < 2 ; i ++)
    {
        listBrain.push_back(new Brain());
        listBrain[i]->id = i;
        listBrain[i]->dataPoney = getCourseData(listBrain[i]->currentDate);
        connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
        connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
        connect(listBrain[i],SIGNAL(somethingToSay(int,QString)),this,SLOT(onSomethingToSay(int,QString)));
        listBrain[i]->start();
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
                            ",PlaceJockey,SexeAge,partant,Arrive,PlacesCheval FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
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
void Survey::initCourseData()
{
    QVector<Poney*> data;
    QString query = "SELECT prix,count() FROM PoneyDB where jour = '"+currentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui' group by prix";
    QSqlQuery getDayPrix;
    if(getDayPrix.exec(query))
    {
        while(getDayPrix.next())
        {
            listPriceCurrentDay.push_back(getDayPrix.value(0).toString());
            listNbPoneyByPrice.push_back(getDayPrix.value(1).toString().toInt());
        }
    }
}


void Survey::onCycleFinished(int value)
{
    qDebug() << "Cycle END";
}

void Survey::onWantMoreData(int id)
{
    qDebug() << "Brain nb : " +QString::number(id) + "Want more data";
    QDate brainCurrentDate = listBrain[id]->currentDate;
    QVector<Price*> data = getCourseData(brainCurrentDate);
    listBrain[id]->dataPoney = data;
    listBrain[id]->expected = expected;
    listBrain[id]->start();
}

void Survey::onSomethingToSay(int value, QString message)
{
    emit somethingToSay(value,message);
}


