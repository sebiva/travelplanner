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
#include "search.h"

Parser *Search::mparser = Vasttrafik::getinstance();

Search::Search(QObject *parent) :
    QObject(parent)
{
    //setbackend("Västtrafik");
}

void Search::setbackend(QString backend) {
    if (backend == "Västtrafik") {
        qDebug() << "Setting backend to Västtrafik";
        mparser = Vasttrafik::getinstance();
    } else if (backend == "SL"){
        qDebug() << "Setting backend to SL";
        mparser = SL::getinstance();
    } else {
        return;
    }
    //Clear the current search
    mparser->cleartrips();

    connect(mparser,SIGNAL(replyready(QString)),this,SLOT(parseready(QString)));
    connect(mparser,SIGNAL(stopsready(QString)),this,SLOT(stopsreceived(QString)));
}

QString Search::getstop(int i) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return NULL;
    }
    return mparser->getstop(i);
}

int Search::getnumstops() {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return -1;
    }
    return mparser->numstops();
}

bool Search::getstops(QString str) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    return mparser->getstops(str.toHtmlEscaped());
}

void Search::stopsreceived(QString err) {
    emit stopsready(err);
}


bool Search::search() {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    emit searching();
    return mparser->getXML(mparser->fromid,mparser->toid,mparser->date,mparser->time);
}

bool Search::search(QString fromid, QString toid, QString date, QString time) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    mparser->fromid = fromid;
    mparser->toid = toid;
    mparser->date = Timehelper::convertdate(date);
    mparser->time = time;
    return search();
}
bool Search::search(QString fromid, QString toid, QString date, QString hour, QString minute) {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return false;
    }
    mparser->fromid = fromid;
    mparser->toid = toid;
    mparser->date = date;
    mparser->time = Timehelper::converttime(hour,minute);
    return search();
}

void Search::cleartrips() {
    if (mparser == NULL) {
        qDebug() << "mparser NULL";
        return;
    }
    mparser->cleartrips();
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
    qDebug() << "Ready";
    emit ready(err);
}

QString Search::getlanguage() {
    qDebug() << getenv("LANG");
    QString LANG = getenv("LANG");
    if (LANG == "sv_SE") {
        return "Svenska";
    }
    return "English";
}
