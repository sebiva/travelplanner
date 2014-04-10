/*
    Copyright (C) 2014 Sebastian Ivarsson
    Contact: Sebastian Ivarsson <sebiva@student.chalmers.se>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
    if (!valid || passed) {
        duration = errmsg;
    } else {
        duration = Timehelper::duration(deprtdate,deprttime,arivrtdate,arivrttime);
    }
}
