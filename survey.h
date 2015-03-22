#ifndef SURVEY_H
#define SURVEY_H

#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>

class Survey : public QThread
{
public:
    Survey();
    ~Survey();
    void run();
    void getCourseData();
    int nbPoney;
    QSqlDatabase db;
};

#endif // SURVEY_H
