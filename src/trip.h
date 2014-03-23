#ifndef TRIP_H

#include "leg.h"
#include <QList>

#define TRIP_H

#include <QObject>


class Trip : public QObject
{
    Q_OBJECT
public:
    explicit Trip(QObject *parent = 0);
    Trip(const Trip &, QObject *parent = 0);
    void addleg(const Leg &leg);

    QList<Leg> *leglist;
signals:

public slots:

};

#endif // TRIP_H
