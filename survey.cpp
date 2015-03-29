#include "survey.h"


Survey::Survey(QString pFilename)
{
    filename = pFilename;
    db = QSqlDatabase::addDatabase("QSQLITE");
    endDate.setDate(2015,01,02);
    bestID = -1;
    NB_BRAIN = 5;
}

Survey::~Survey()
{

}

void Survey::run()
{
    initBDD();
    initCoeff();
    initData();
    initBrain();
    bool go = true;
    while(go)
    {
        listRatio.clear();
        for (currentDate = 0 ; currentDate < data.length() ; currentDate++)
        {
            for(int brain = 0 ; brain < NB_BRAIN ; brain++)
            {
                listBrain[brain]->setDataDay(data[currentDate]);
                listBrain[brain]->setCoeff(coeff[brain]);
                listBrain[brain]->run();
            }
        }
        for(int brain = 0 ; brain < NB_BRAIN ; brain++)
        {
            float ratio = listBrain[brain]->getRatio();
            emit newRatioCalculated(brain,ratio);
            listRatio.push_back(ratio);
        }
        int idBest = findTheBest();
        for (int brain = 0 ; brain < NB_BRAIN ; brain++)
        {
            generateNewCoeff(idBest);
        }
    }
}


QVector<Price *> Survey::getCourseData(QDate pCurrentDate)
{
    QVector<Price*> dataOfTheDay;
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
                                   ",NbCoursePropJour FROM PoneyDB where jour = '"
                    + pCurrentDate.toString("yyyy-MM-dd")+"' and PMU = 'oui'"
                    + " and prix ='"
                    + temp->name
                    +"'";
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
        qDebug() << "Ratio brain n :" + QString::number(i) + " is " + QString::number(listRatio[i]);
    }
    return id;
}

void Survey::initCoeff()
{
    coeff.clear();
    for (int brain = 0 ; brain < NB_BRAIN ; brain++)
    {
        QVector<float> temp;
        for (int i = 0; i < 25 ; i++)
        {
            float frandom = ((float)rand() / (float)RAND_MAX);
            temp.push_back(frandom);
        }
        coeff.push_back(temp);
    }
}

void Survey::initBrain()
{
    listBrain.clear();
    for (int brain = 0; brain < NB_BRAIN ; brain ++)
    {
        listBrain.push_back(new Brain());
        listBrain[brain]->setID(brain);
    }
}

void Survey::initData()
{
    data.clear();
    for (QDate qCurrentDate(2015,01,01) ; qCurrentDate.toString("yyyy-MM-dd") != endDate.toString("yyyy-MM-dd") ; qCurrentDate  = qCurrentDate.addDays(1))
    {
        data.push_back(getCourseData(qCurrentDate));
        qDebug() << "Data from " + qCurrentDate.toString("yyyy-MM-dd") + " loaded";
    }
}

void Survey::initBDD()
{
    db.setDatabaseName(filename);
    if(!db.open() || !db.isValid())
    {
        qDebug() << "Error when opening DB";
    }
    else
    {
        qDebug() << "DB loaded";
    }

}

void Survey::generateNewCoeff(int idBest)
{
    for (int brain = 0 ; brain < NB_BRAIN ; brain++)
    {
        listBrain[brain]->setCoeff(listBrain[idBest]->getCoeff());
        for (int i = 0; i < 25 ; i++)
        {
            float ratio = (rand()%10)*10+1;
            float ratioMutation = 0.2;
            if(rand() < RAND_MAX * ratio)
            {
                float r = rand() * 2 - RAND_MAX;
                r *= ratioMutation;
                r /=  RAND_MAX;
                coeff[brain][i] += r;
                if(coeff[brain][i] > 1.0f)
                    coeff[brain][i] = 1.0f;
                if(coeff[brain][i] < -1.0f)
                    coeff[brain][i] = -1.0f;
            }
        }
    }
}




