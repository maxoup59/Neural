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
        connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
        connect(listBrain[i],SIGNAL(wantMoreData(int)),this,SLOT(onWantMoreData(int)));
        listBrain[i]->start();
    }
}

QVector<Price *> Survey::getCourseData(QDate pCurrentDate)
{
    //Get list price of the day
    QVector<Price*> dataOfTheday;
    //QVector<Poney*> data;
    QString query = "SELECT prix,count() FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui' group by prix";
    QSqlQuery getDayPrix;
    if(getDayPrix.exec(query))
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
            QString query = "SELECT CoursesCheval,VictoiresCheval,PlacesCheval,CoursesEntraineur"
                            ",VictoiresEntraineur,PlaceEntraineur,CoursesJockey,VictoiresJockey"
                            ",PlaceJockey,SexeAge,partant,Arrive FROM PoneyDB where jour = '"+pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
                                                                                                                                 " and prix ='" + temp->name +"'";
            QSqlQuery getData;
            if(!getData.exec(query))
            {
                qDebug() << "Error query";
            }
            else
            {
                int nb = 0;
                while(getData.next())
                {
                    data[nb]->ratioPoney = (float)getData.value(1).toInt() / (float)getData.value(0).toInt();
                    data[nb]->ratioTrainer = (float)getData.value(3).toInt() / (float)getData.value(4).toInt();
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
            dataOfTheday.push_back(temp);
        }
    }
    return dataOfTheday;
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
    QDate brainCurrentDate = listBrain[id]->currentDate;
    QVector<Price*> data = getCourseData(brainCurrentDate);
    listBrain[id]->dataPoney = data;
    listBrain[id]->expected = expected;
    listBrain[id]->start();
}

/*void Survey::onResultOKAY()
{
    //getCourseData();
    //getNewData();
    qDebug() << "RESULT FOUND : " + currentDate.toString("yyyy-MM-dd") + currentPrice;
    currentPrice++;
    //
    if(currentPrice < listPriceCurrentDay.length())
    {
        QVector<Poney*> data = getCourseData();
        for (int i = 0 ;i < 5 ; i ++)
        {
            listBrain[i]->setNbPoney(listNbPoneyByPrice[currentPrice]);
            listBrain[i]->dataPoney = data;
            listBrain[i]->expected = expected;
            connect(listBrain[i],SIGNAL(cycleFinished(int)),this,SLOT(onCycleFinished(int)));
            connect(listBrain[i],SIGNAL(resultOKAY()),this,SLOT(onResultOKAY()));
            listBrain[i]->start();
        }
    }
    else
    {
        currentDate = currentDate.addDays(1);
        initCourseData();
        currentPrice = 0;
    }
}*/


