#include "search.h"

Search::Search(QObject *parent) :
    QObject(parent)
{
    mparser = Parser::getinstance();
    mparser->hello();
    connect(mparser,SIGNAL(ready(int)),this,SLOT(parseready(int)));
}


bool Search::getXML(QString fromid, QString toid, QString date, QString time) {
    return mparser->getXML(fromid,toid,date,time);
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
