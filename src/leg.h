#ifndef LEG_H
#define LEG_H

#include <QObject>

class Leg : public QObject
{
    Q_OBJECT
public:
    explicit Leg(QObject *parent = 0);
    Leg(Leg *, QObject *parent = 0);
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


signals:

public slots:

};

#endif // LEG_H
