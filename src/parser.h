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

#include <QDateTime>
#include <QDate>
#include <QTime>

#include <QQmlEngine>
#include <QJSEngine>
#include <QtQuick>
#include <qqml.h>

#define PARSER_H

#include <QObject>




class Parser : public QObject
{
    Q_OBJECT
public:

    static Parser *getinstance();
    ~Parser();
    static QString getstop(int i);
    bool getstops(QString backend, QString str);
    int numstops();
    bool getXML(QString backend, QString fromid, QString toid, QString date, QString time);

    Trip * getTrip(int index);
    Leg * getLeg(int tripindex, int legindex);

    int numtrips();
    int numlegs(int tripindex);
    void cleartrips();

    QString removespecials(QString str);

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
    void ready(QString err);
    void stopsparsed(QString err);

public slots:
    void stopsreply(QNetworkReply *reply);
    void XMLready(QNetworkReply *reply);



private:
    void vasttrafikstops(QNetworkReply *reply);
    void parsevasttrafikreply(QNetworkReply *reply);
    QString address;
    QString nameaddress;
    static Parser *mparser;
    static QList<Trip *> *trips;
    static QList<QString> *stops;

    explicit Parser(QObject *parent = 0);
};

#endif // PARSER_H
