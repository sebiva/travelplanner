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
    Q_INVOKABLE QString geterrmsg() {return errmsg;}

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
    QString errmsg;
    bool passed;
signals:

public slots:

};

#endif // TRIP_H
