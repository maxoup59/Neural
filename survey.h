#ifndef SURVEY_H
#define SURVEY_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "poney.h"
#include <QDate>
#include <QVector>
#include "brain.h"
class Survey : public QThread
{
    Q_OBJECT
public:
    Survey();
    ~Survey();
    void run();
    QVector<Poney *> getCourseData();
     QVector<Brain*> listBrain;
    void initCourseData();
    int nbPoney;
    QSqlDatabase db;
    int expected;
private slots:
    void onCycleFinished(int);
    void onResultOKAY();
private:
    QVector<QString> listPriceCurrentDay;
    QVector<int> listNbPoneyByPrice;
    int currentPrice;
    QDate currentDate;
};

#endif // SURVEY_H
