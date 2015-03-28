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

private:
    int NB_BRAIN;
    int bestID;
    int currentDate;

    QString filename;
    QDate endDate;
    QSqlDatabase db;

    QVector<QVector<float> > listresult;
    QVector<QVector<float> > coeff;

    QVector<QVector<Price*> > data;
    QVector<Brain*> listBrain;
    QVector<float> listRatio;

    void initCoeff();
    void initBrain();
    void initData();
    void initBDD();

    void initCourseData();
    void generateNewCoeff(int idBest);

    int findTheBest();
    QVector<Price *> getCourseData(QDate pCurrentDate);

    void run();

};

#endif // SURVEY_H
