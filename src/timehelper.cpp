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

