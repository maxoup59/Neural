#ifndef SURVEY_H
#define SURVEY_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "poney.h"
class Survey : public QThread
{
public:
    Survey();
    ~Survey();
    void run();
    QVector<Poney*> getCourseData();
    int nbPoney;
    QSqlDatabase db;
};

#endif // SURVEY_H
