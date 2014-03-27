#ifndef TRIP_H

#include "leg.h"
#include <QList>
#include <QDebug>

#define TRIP_H

#include <QObject>


class Trip : public QObject
{
    Q_OBJECT
public:
    explicit Trip(QObject *parent = 0);
    Trip(const Trip &, QObject *parent = 0);
    void addleg(Leg *leg);
    Leg *getLeg(int index);
    QList<Leg *> *leglist;
    void print();

    Q_INVOKABLE QString getdeptime() {return deptime;}
    Q_INVOKABLE QString getdepdate() {return depdate;}
    Q_INVOKABLE QString getdeprttime() {return deprttime;}
    Q_INVOKABLE QString getdeprtdate() {return deprtdate;}
    Q_INVOKABLE QString getarivtime() {return arivtime;}
    Q_INVOKABLE QString getarivdate() {return arivdate;}
    Q_INVOKABLE QString getarivrttime() {return arivrttime;}
    Q_INVOKABLE QString getarivrtdate() {return arivrtdate;}


    QString deptime;
    QString depdate;
    QString deprttime;
    QString deprtdate;
    QString arivtime;
    QString arivdate;
    QString arivrttime;
    QString arivrtdate;
signals:

public slots:

};

#endif // TRIP_H
