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
#include "parser.h"

Parser *Parser::mparser = 0;
QList<Trip *> *Parser::trips = new QList<Trip *>();
QList<QString> *Parser::stops = new QList<QString>();

Parser::Parser(QObject *parent) :
    QObject(parent)
{

}

Parser::~Parser() {
    qDebug() << "Deleting parser!";
    cleartrips();
    delete trips;
    delete stops;
}

QString Parser::getstop(int i) {
    if ((stops == NULL) || i >= stops->length()) {
        qDebug() << "Stops NULL, or index out of bounds";
        return NULL;
    }
    return stops->at(i);
}

int Parser::numtrips() {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return -1;
    }
    return trips->length();
}

int Parser::numlegs(int tripindex) {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return -1;
    }

    if (tripindex >= trips->length()) {
        return -1;
    } else {
        return trips->at(tripindex)->leglist->length();
    }
}

Trip * Parser::getTrip(int index) {


    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return NULL;
    }
    if (index >= trips->length()) {
        return NULL;
    }
    return trips->at(index);
}

Leg * Parser::getLeg(int tripindex, int legindex) {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return NULL;
    }
    if (tripindex >= trips->length() || legindex >= trips->at(tripindex)->leglist->length()) {
        return NULL;
    }
    return getTrip(tripindex)->leglist->at(legindex);
}

void Parser::cleartrips() {
    for (int i = 0; i < trips->length(); i++) {
        delete trips->at(i);

    }
    trips->clear();
}


int Parser::numstops() {
    return stops->length();
}


QString Parser::removespecials(QString str) {
    QString res = str;

    for(int i = 0; i < res.length();i++) {
        QString letter = res.mid(i,1);
        if (letter=="å" || letter=="Å") {
            res = res.left(i) + "%C5" + res.right(res.length()-i-1);
        } else if (letter=="ä" || letter=="Ä") {
            res = res.left(i) + "%C4" + res.right(res.length()-i-1);
        } else if (letter=="ö" || letter=="Ö") {
            res = res.left(i) + "%D6" + res.right(res.length()-i-1);
        }
    }
    return res;
}

