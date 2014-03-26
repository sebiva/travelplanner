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
signals:

public slots:

};

#endif // TRIP_H
