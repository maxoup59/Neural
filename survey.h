#ifndef SURVEY_H
#define SURVEY_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "poney.h"
#include <QDate>
#include <QVector>
#include "brain.h"
#include "price.h"
class Survey : public QThread
{
    Q_OBJECT
public:
    Survey(QString filename = "test.dev.db");
    ~Survey();
    void run();
    QVector<Price *> getCourseData(QDate pCurrentDate);
     QVector<Brain*> listBrain;
    void initCourseData();
    int nbPoney;
    QSqlDatabase db;
    int expected;
signals:
    void somethingToSay(int,QString);
private slots:
    void onCycleFinished(int);
   // void onNoMoreData(QVector<float>,float);
    void onWantMoreData(int);
    void onSomethingToSay(int,QString);
private:
    QVector<QString> listPriceCurrentDay;
    QVector<QVector<float> > listresult;
    int bestID;
    float bestRatio;
    QVector<int> listNbPoneyByPrice;
    int currentPrice;
    QDate currentDate;
    QString filename;
    int findTheBest();
    QVector<float> listRatio;
};

#endif // SURVEY_H
