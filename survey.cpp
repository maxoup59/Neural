#include "survey.h"
#include "poney.h"
#include <QDebug>
#include "brain.h"
#include "price.h"
Survey::Survey()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    currentDate.setDate(2015,01,01);
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
    for (int i = 0 ;i < 5 ; i ++)
    {
        listBrain.push_back(new Brain());
        listBrain[i]->id = i;
        listBrain[i]->dataPoney = getCourseData(listBrain[i]->currentDate);
        connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
        connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
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
            QVector<Poney *> data;
            for(int i = 0 ; i < temp->nbOfPoney; i++)
            {
                data.push_back(new Poney());
            }
            QString queryGetData = "SELECT CoursesCheval,VictoiresCheval,PlacesCheval,CoursesEntraineur"
                            ",VictoiresEntraineur,PlaceEntraineur,CoursesJockey,VictoiresJockey"
                            ",PlaceJockey,SexeAge,partant,Arrive FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
                                                                                                                                 " and prix ='" + temp->name +"'";
            //qDebug() << queryGetData;
            QSqlQuery getData;
            if(!getData.exec(queryGetData))
            {
                qDebug() << "Error query";
            }
            else
            {
                int nb = 0;
                while(getData.next())
                {
                    data[nb]->ratioPoney = (float)getData.value(1).toInt() / (float)getData.value(0).toInt();
                    data[nb]->ratioTrainer = (float)getData.value(4).toInt() / (float)getData.value(3).toInt();
                    data[nb]->ratioJockey = (float)getData.value(7).toInt() / (float)getData.value(6).toInt();
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
                }
            }
            temp->listOfPoney = data;
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


