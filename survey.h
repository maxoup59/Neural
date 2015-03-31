#ifndef SURVEY_H
#define SURVEY_H

#include <QDebug>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include "brain.h"
#include "price.h"
#include "poney.h"



class Survey : public QThread
{
    Q_OBJECT
public:
    Survey(QString filename = "test.dev.db");
    ~Survey();
    float ratioMutation;
signals:
    void newRatioCalculated(int id,float value);
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
    float backPropagation();

    int findTheBest();
    QVector<Price *> getCourseData(QDate pCurrentDate);

    void run();
};

#endif // SURVEY_H
