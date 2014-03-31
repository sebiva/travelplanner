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
