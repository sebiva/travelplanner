#include "search.h"

#include "time.h"

Search::Search(QObject *parent) :
    QObject(parent)
{
    mparser = Parser::getinstance();
    connect(mparser,SIGNAL(ready(QString)),this,SLOT(parseready(QString)));
}
bool Search::search() {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    return mparser->getXML(mparser->fromid,mparser->toid,mparser->date,mparser->time);
}

bool Search::search(QString fromid, QString toid, QString date, QString time) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    mparser->fromid = fromid;
    mparser->toid = toid;
    mparser->date = date;
    mparser->time = time;
    return mparser->getXML(fromid,toid,Time::convertdate(date),time);
}
bool Search::search(QString fromid, QString toid, QString date, QString hour, QString minute) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    mparser->fromid = fromid;
    mparser->toid = toid;
    mparser->date = date;
    mparser->time = Time::converttime(hour,minute);
    return mparser->getXML(fromid,toid,Time::convertdate(date),Time::converttime(hour, minute));
}


QObject * Search::getTrip(int index) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return NULL;
    }
    return mparser->getTrip(index);
}

QObject * Search::getLeg(int tripindex, int legindex) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return NULL;
    }
    return mparser->getLeg(tripindex, legindex);
}

int Search::getnumlegs(int tripindex) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return -1;
    }
    return mparser->numlegs(tripindex);
}

void Search::parseready(QString err) {
    emit ready(err);
}
