#include "trip.h"

Trip::Trip(QObject *parent) :
    QObject(parent)
{
    leglist = new QList<Leg>();
}
Trip::Trip(const Trip &, QObject *parent) :
    QObject(parent)
{
    leglist = new QList<Leg>();
}


void Trip::addleg(const Leg &leg) {
    leglist->append(leg);
}
