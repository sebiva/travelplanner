#include "leg.h"
#include <QDebug>

Leg::Leg(QObject *parent) :
    QObject(parent)
{
}

Leg::Leg(Leg *, QObject *parent) :
    QObject(parent)
{
}


Leg::~Leg() {
    qDebug() << "Deleting leg!";
}

void Leg::calculatetimes() {
    mdepdelay = Timehelper::delay(mdepdate,mdeptime,mdeprtdate,mdeprttime);
    marivdelay = Timehelper::delay(marivdate,marivtime,marivrtdate,marivrttime);
}
