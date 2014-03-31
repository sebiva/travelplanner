#include "search.h"

#include "time.h"

Search::Search(QObject *parent) :
    QObject(parent)
{
    mparser = Parser::getinstance();
    connect(mparser,SIGNAL(ready(int)),this,SLOT(parseready(int)));
}
bool Search::search() {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    return mparser->getXML(mparser->fromid,mparser->toid,mparser->date,mparser->time);
}

bool Search::search(QString fromid, QString toid, QString date, QString time) {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    return mparser->getXML(fromid,toid,Time::convertdate(date),time);
}
bool Search::search(QString fromid, QString toid, QString date, QString hour, QString minute) {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    return mparser->getXML(fromid,toid,Time::convertdate(date),Time::converttime(hour, minute));
}


QObject * Search::getTrip(int index) {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return NULL;
    }
    return mparser->getTrip(index);
}

QObject * Search::getLeg(int tripindex, int legindex) {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return NULL;
    }
    return mparser->getLeg(tripindex, legindex);
}

int Search::getnumlegs(int tripindex) {
    qDebug() << ".";
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return -1;
    }
    return mparser->numlegs(tripindex);
}

void Search::parseready(int err) {
    emit ready(err);
}
