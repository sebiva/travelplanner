#include "trip.h"

Trip::Trip(QObject *parent) :
    QObject(parent)
{
    leglist = new QList<Leg *>();
}
Trip::Trip(const Trip &, QObject *parent) :
    QObject(parent)
{
    leglist = new QList<Leg *>();
}
Trip::~Trip() {
    qDebug() << "Destroying trip";
}


void Trip::addleg(Leg *leg) {
    leglist->append(leg);
}

Leg *Trip::getLeg(int index) {
    return leglist->at(index);
}

void Trip::print() {
    qDebug() << "## Printing Trip:";
}

void Trip::calculatetimes() {
    depdelay = Timehelper::delay(depdate, deptime, deprtdate, deprttime);
    arivdelay = Timehelper::delay(arivdate,arivtime,arivrtdate,arivrttime);
    duration = Timehelper::duration(deprtdate,deprttime,arivrtdate,arivrttime);
}
