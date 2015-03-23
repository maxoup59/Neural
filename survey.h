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
    Survey();
    ~Survey();
    void run();
    QVector<Price *> getCourseData(QDate pCurrentDate);
     QVector<Brain*> listBrain;
    void initCourseData();
    int nbPoney;
    QSqlDatabase db;
    int expected;
private slots:
    void onCycleFinished(int);
    void onWantMoreData(int);
private:
    QVector<QString> listPriceCurrentDay;
    QVector<int> listNbPoneyByPrice;
    int currentPrice;
    QDate currentDate;
};

#endif // SURVEY_H
