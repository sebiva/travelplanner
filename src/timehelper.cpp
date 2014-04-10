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
#include "timehelper.h"

Timehelper::Timehelper(QObject *parent) :
    QObject(parent)
{
}

QString Timehelper::convertdate(QString date) {
    if(date.length() == 10 && date.indexOf("-") != 0) {
        return date;
    } else if (date.indexOf("/") != -1 && date.split("/").length() == 3) {
        return date.split("/").at(2) + "-" + date.split("/").at(1) + "-" + date.split("/").at(0);
    }
    return "";
}

QString Timehelper::converttime(QString hour, QString minute) {
    return hour + ":" + minute;
}

QString Timehelper::duration(QString depdate, QString deptime, QString arivdate, QString arivtime, bool delay) {
    depdate = convertdate(depdate);
    arivdate = convertdate(arivdate);

    QDate depdate2 = QDate::fromString(depdate, "yyyy-MM-dd");
    QTime deptime2 = QTime::fromString(deptime, "hh:mm");
    QDate arivdate2 = QDate::fromString(arivdate, "yyyy-MM-dd");
    QTime arivtime2 = QTime::fromString(arivtime, "hh:mm");

    bool minus = false;
    QDateTime epoch = QDateTime(QDate(1970,1,1),QTime(0,0),Qt::UTC);
    qint64 dep = QDateTime(depdate2, deptime2).toMSecsSinceEpoch();
    qint64 ariv = QDateTime(arivdate2, arivtime2).toMSecsSinceEpoch();
    minus = dep > ariv;
    if (dep == ariv) {
        if (!delay) {
            return "0min";
        } else {
            return "";
        }

    }
    qint64 diff2 = minus ? (dep - ariv) : (ariv - dep);
    //qDebug() << diff2;
    epoch = epoch.addMSecs(diff2);

    int days = epoch.toString("d").toInt() - 1;
    int hour = epoch.toString("h").toInt();
    int min = epoch.toString("m").toInt();

    //qDebug() << minus << days << "d" << hour << "h" << min << "min";
    //qDebug() << epoch.toString();

    QString answer = "";

    if (min > 0) {
        if (!delay) {
            answer.sprintf("%dmin", min);
        } else {
            answer.sprintf("%d", min);
        }
    }
    if (hour > 0) {
        QString hourstr = "";
        if (!delay) {
            hourstr.sprintf("%dh", hour);
        } else {
            if (min > 0) {
                hourstr.sprintf("%d:", hour);
            } else {
                hourstr.sprintf("%dh", hour);
            }
        }
        answer = hourstr + answer;
    }
    if (days > 0) {
        QString daystr = "";
        daystr.sprintf("%dd", days);
        answer = answer.append(daystr);
    }
    return minus ? "-" + answer : (delay ? "+" + answer : answer);
}

QString Timehelper::delay(QString date, QString time, QString rtdate, QString rttime) {
    return duration(date, time, rtdate, rttime, true);
}

QString Timehelper::getcurrentdatestr() {
    return QDate::currentDate().toString("yyyy-MM-dd");
}
QString Timehelper::getcurrenttimestr() {
    return QTime::currentTime().toString("hh:mm");
}
QDate Timehelper::getcurrentdate() {
    return QDate::currentDate();
}
QTime Timehelper::getcurrenttime() {
    return QTime::currentTime();
}
qint64 Timehelper::daysfromtoday(QString date) {
    QDate qdate(date.mid(0,4).toInt(),date.mid(5,2).toInt(),date.mid(8,2).toInt());
    return QDate::currentDate().daysTo(qdate);
}

bool Timehelper::beforenow(QString date, QString time) {
    QDate qdate(date.mid(0,4).toInt(),date.mid(5,2).toInt(),date.mid(8,2).toInt());
    QTime qtime(time.mid(0,2).toInt(),time.mid(3,2).toInt());
    return QDateTime::currentDateTime().secsTo(QDateTime(qdate,qtime)) < 0;
}
