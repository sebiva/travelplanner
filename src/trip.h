#ifndef TRIP_H

#include "leg.h"
#include <QList>
#include <QDebug>
#include "timehelper.h"

#define TRIP_H

#include <QObject>


class Trip : public QObject
{
    Q_OBJECT
public:
    explicit Trip(QObject *parent = 0);
    Trip(const Trip &, QObject *parent = 0);
    ~Trip();
    void calculatetimes();

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
    Q_INVOKABLE QString getdepdelay() {return depdelay;}
    Q_INVOKABLE QString getarivdelay() {return arivdelay;}
    Q_INVOKABLE QString getduration() {return duration;}

    QString deptime;
    QString depdate;
    QString deprttime;
    QString deprtdate;
    QString arivtime;
    QString arivdate;
    QString arivrttime;
    QString arivrtdate;

    QString depdelay;
    QString arivdelay;
    QString duration;
    bool valid;
    bool passed;
signals:

public slots:

};

#endif // TRIP_H
