#include "survey.h"
#include "poney.h"
#include <QDebug>
#include "brain.h"
Survey::Survey()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
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
    QVector<Poney*> data = getCourseData();
    QVector<Brain*> listBrain;
    for (int i = 0 ;i < 5 ; i ++)
    {
        listBrain.push_back(new Brain());
        listBrain[i]->setNbPoney(nbPoney);
        listBrain[i]->dataPoney = data;
        listBrain[i]->start();
    }

}

QVector<Poney*> Survey::getCourseData()
{
    QVector<Poney*> data;
    //Get data from BDD and put in Poney for each Poney
    for(int i = 0 ; i < nbPoney; i++)
    {
        data.push_back(new Poney());
    }
    QString query = "SELECT CoursesCheval,VictoiresCheval,PlacesCheval,CoursesEntraineur"
                    ",VictoiresEntraineur,PlaceEntraineur,CoursesJockey,VictoiresJockey"
                    ",PlaceJockey,SexeAge,partant FROM PoneyDB where jour = '2015-03-02' and PMU = 'oui'"
                    " and Categorie = 'Course B'";
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
            nbPoney = getData.value(10).toInt();
        }
    }
    return data;
}

