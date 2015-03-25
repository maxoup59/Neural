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
    QVector<QVector<Price*> > getCourseData();
    QVector<Brain*> listBrain;
    int nbPoney;
    QSqlDatabase db;
    int expected;
signals:
    void somethingToSay(int,QString);
private slots:
    void onCycleFinished(int);
    void onSomethingToSay(int,QString);
private:
    QVector<QString> listPriceCurrentDay;
    QVector<int> listNbPoneyByPrice;
    int currentPrice;
    QDate currentDate;
};

#endif // SURVEY_H
