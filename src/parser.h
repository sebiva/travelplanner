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
#ifndef PARSER_H
#include <QNetworkAccessManager>
#include <QUrl>
#include <QHttpPart>
#include <QTemporaryFile>
#include <QNetworkReply>

#include <QXmlStreamReader>

#include <QtDebug>
#include <QDebug>

#include <QList>
#include <QStringList>
#include "trip.h"
#include "leg.h"
//#include "vasttrafik.h"

#include <QDateTime>
#include <QDate>
#include <QTime>

#include <QQmlEngine>
#include <QJSEngine>
#include <QtQuick>
#include <qqml.h>

#define PARSER_H

#include <QObject>

class Vasttrafik;

class Parser : public QObject
{
    Q_OBJECT
public:

    ~Parser();
    static QString getstop(int i);
    virtual bool getstops(QString str) = 0;
    virtual bool getXML(QString fromid, QString toid, QString date, QString time) = 0;

    int numstops();

    Trip * getTrip(int index);
    Leg * getLeg(int tripindex, int legindex);

    int numtrips();
    int numlegs(int tripindex);
    void cleartrips();

    bool skiptoendof(QXmlStreamReader *reader, QString str);
    bool skiptostartof(QXmlStreamReader *reader, QString str);

    QString removespecials(QString str);
    QString removespecials_sl_skane(QString str);

    QString fromid;
    QString toid;
    QString from;
    QString to;
    QString time;
    QString date;
    QString timeofsearch;
    QString dateofsearch;
    QString backend;

signals:
    void replyready(QString err);
    void stopsready(QString err);

public slots:

protected:

    static QList<Trip *> *trips;
    static QList<QString> *stops;
    static Parser *mparser;

    explicit Parser(QObject *parent = 0);

private:

};

#endif // PARSER_H
