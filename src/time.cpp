#include "time.h"

#include <QStringList>

Time::Time(QObject *parent) :
    QObject(parent)
{
}


QString Time::convertdate(QString date) {
    if(date.length() == 10 && date.indexOf("-") == 2 && date.indexOf("-", 3) == 4 && date.indexOf("-", 5) == 6) {
        return date;
    } else if (date.indexOf("/") != -1 && date.split("/").length() == 3) {
        return date.split("/").at(2) + "-" + date.split("/").at(1) + "-" + date.split("/").at(0);
    }
    return "";
}

QString Time::converttime(QString hour, QString minute) {
    return hour + ":" + minute;
}
