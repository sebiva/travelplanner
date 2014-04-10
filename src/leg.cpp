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
