#ifndef SURVEY_H
#define SURVEY_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "poney.h"

class Survey : public QThread
{
    Q_OBJECT
public:
    Survey();
    ~Survey();
    void run();
    QVector<Poney*> getCourseData();
    int nbPoney;
    QSqlDatabase db;
    int expected;
private slots:
    void onCycleFinished(int);
    void onResultOKAY();
};

#endif // SURVEY_H
