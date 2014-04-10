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
#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QStringList>

class Timehelper : public QObject
{
    Q_OBJECT
public:
    explicit Timehelper(QObject *parent = 0);

    Q_INVOKABLE static QString convertdate(QString date);
    Q_INVOKABLE static QString converttime(QString hour, QString minute);
    Q_INVOKABLE static QString duration(QString depdate, QString deptime, QString arivdate, QString arivtime, bool delay = false);
    Q_INVOKABLE static QString delay(QString date, QString time, QString rtdate, QString rttime);
    Q_INVOKABLE static QString getcurrentdatestr();
    Q_INVOKABLE static QString getcurrenttimestr();
    Q_INVOKABLE static QDate getcurrentdate();
    Q_INVOKABLE static QTime getcurrenttime();
    Q_INVOKABLE static qint64 daysfromtoday(QString date);
    Q_INVOKABLE static bool beforenow(QString date, QString time);
signals:

public slots:

};

#endif // TIMEHELPER_H
