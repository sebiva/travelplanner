#include "leg.h"

Leg::Leg(QObject *parent) :
    QObject(parent)
{
}

Leg::Leg(Leg *, QObject *parent) :
    QObject(parent)
{
}