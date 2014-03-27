#include "search.h"

#include "time.h"

Search::Search(QObject *parent) :
    QObject(parent)
{
    mparser = Parser::getinstance();
    mparser->hello();
    connect(mparser,SIGNAL(ready(int)),this,SLOT(parseready(int)));
}

bool Search::search(QString fromid, QString toid, QString date, QString time) {
    return mparser->getXML(fromid,toid,Time::convertdate(date),time);
}
bool Search::search(QString fromid, QString toid, QString date, QString hour, QString minute) {
    return mparser->getXML(fromid,toid,Time::convertdate(date),Time::converttime(hour, minute));
}


QObject * Search::getTrip(int index) {
    return mparser->getTrip(index);
}

QObject * Search::getLeg(int tripindex, int legindex) {
    return mparser->getLeg(tripindex, legindex);
}

void Search::parseready(int err) {
    emit ready(err);
}
