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
#ifndef LEG_H
#define LEG_H

#include <QObject>
#include "timehelper.h"

class Leg : public QObject
{
    Q_OBJECT
public:
    explicit Leg(QObject *parent = 0);
    Leg(Leg *, QObject *parent = 0);
    ~Leg();
    void calculatetimes();

    QString mline;
    QString mdir;
    QString mfgcolour;
    QString mbgcolour;
    QString mfrom;
    QString mfromid;
    QString mfromtrack;
    QString mdeptime;
    QString mdeprttime;
    QString mdepdate;
    QString mdeprtdate;
    QString mto;
    QString mtoid;
    QString mtotrack;
    QString marivtime;
    QString marivrttime;
    QString marivdate;
    QString marivrtdate;
    QString mdepdelay;
    QString marivdelay;

    QString merrstatus;

    Q_PROPERTY (QString line READ line)
    Q_PROPERTY (QString dir READ dir)
    Q_PROPERTY (QString fgcolour READ fgcolour)
    Q_PROPERTY (QString bgcolour READ bgcolour)
    Q_PROPERTY (QString from READ from)
    Q_PROPERTY (QString fromid READ fromid)
    Q_PROPERTY (QString fromtrack READ fromtrack)
    Q_PROPERTY (QString deptime READ deptime)
    Q_PROPERTY (QString deprttime READ deprttime)
    Q_PROPERTY (QString depdate READ depdate)
    Q_PROPERTY (QString deprtdate READ deprtdate)
    Q_PROPERTY (QString to READ to)
    Q_PROPERTY (QString toid READ toid)
    Q_PROPERTY (QString totrack READ totrack)
    Q_PROPERTY (QString arivtime READ arivtime)
    Q_PROPERTY (QString arivrttime READ arivrttime)
    Q_PROPERTY (QString arivdate READ arivdate)
    Q_PROPERTY (QString arivrtdate READ arivrtdate)
    Q_PROPERTY (QString depdelay READ depdelay)
    Q_PROPERTY (QString arivdelay READ arivdelay)

    Q_PROPERTY (QString errstatus READ errstatus)

    QString line() {return mline;}
    QString dir() {return mdir;}
    QString fgcolour() {return mfgcolour;}
    QString bgcolour() {return mbgcolour;}
    QString from() {return mfrom;}
    QString fromid() {return mfromid;}
    QString fromtrack() {return mfromtrack;}
    QString deptime() {return mdeptime;}
    QString deprttime() {return mdeprttime;}
    QString depdate() {return mdepdate;}
    QString deprtdate() {return mdeprtdate;}
    QString to() {return mto;}
    QString toid() {return mtoid;}
    QString totrack() {return mtotrack;}
    QString arivtime() {return marivtime;}
    QString arivrttime() {return marivrttime;}
    QString arivdate() {return marivdate;}
    QString arivrtdate() {return marivrtdate;}
    QString depdelay() {return mdepdelay;}
    QString arivdelay() {return marivdelay;}

    QString errstatus() {return merrstatus;}


signals:

public slots:

};

#endif // LEG_H
