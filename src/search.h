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
#ifndef SEARCH_H
#define SEARCH_H

#include <QtDebug>

#include <QList>
#include <QStringList>
#include "trip.h"
#include "leg.h"
#include "parser.h"
#include "timehelper.h"
#include "vasttrafik.h"
#include "sl.h"

#include <QQmlEngine>
#include <QJSEngine>
#include <QtQuick>
#include <qqml.h>

#include <QObject>




class Search : public QObject
{
    Q_OBJECT
public:

    explicit Search(QObject *parent = 0);

    Q_INVOKABLE QString getstop(int i);
    Q_INVOKABLE bool getstops(QString str);
    Q_INVOKABLE bool search();
    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString time);
    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString hour, QString minute);

    Q_INVOKABLE QObject * getTrip(int index);
    Q_INVOKABLE QObject * getLeg(int tripindex, int legindex);
    Q_INVOKABLE int getnumlegs(int tripindex);
    Q_INVOKABLE int getnumstops();

    Q_INVOKABLE void cleartrips();

    Q_INVOKABLE QString getfromid() {
        return mparser->fromid;
    }
    Q_INVOKABLE QString gettoid() {
        return mparser->toid;
    }
    Q_INVOKABLE QString getfrom() {
        return mparser->from;
    }
    Q_INVOKABLE QString getto() {
        return mparser->to;
    }
    Q_INVOKABLE QString getdate() {
        return mparser->date;
    }
    Q_INVOKABLE QString gettime() {
        return mparser->time;
    }
    Q_INVOKABLE QString getdateofsearch() {
        return mparser->dateofsearch;
    }
    Q_INVOKABLE QString gettimeofsearch() {
        return mparser->timeofsearch;
    }

    Q_INVOKABLE void setfromid(QString fromid) {
        mparser->fromid = fromid;
    }
    Q_INVOKABLE void settoid(QString toid) {
        mparser->toid = toid;
    }
    Q_INVOKABLE void setfrom(QString from) {
        mparser->from = from;
    }
    Q_INVOKABLE void setto(QString to) {
        mparser->to = to;
    }
    Q_INVOKABLE void setdate(QString date) {
        mparser->date = date;
    }
    Q_INVOKABLE void settime(QString time) {
        mparser->time = time;
    }
    Q_INVOKABLE void setdateofsearch(QString dateofsearch) {
        mparser->dateofsearch = dateofsearch;
    }
    Q_INVOKABLE void settimeofsearch(QString timeofsearch) {
        mparser->timeofsearch = timeofsearch;
    }

    Q_INVOKABLE void setbackend(QString backend);
    Q_INVOKABLE QString getlanguage();
signals:
    void ready(QString err);
    void searching();
    void stopsready(QString err);

public slots:
    void parseready(QString err);
    void stopsreceived(QString err);

private:
    static Parser * mparser;

};
#endif // SEARCH_H
