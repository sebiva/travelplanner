#ifndef LEG_H
#define LEG_H

#include <QObject>

class Leg : public QObject
{
    Q_OBJECT
public:
    explicit Leg(QObject *parent = 0);
    Leg(const Leg &, QObject *parent = 0);
    QString line;
    QString dir;
    QString fgColour;
    QString bgColour;
    QString from;
    QString fromid;
    QString fromtrack;
    QString deptime;
    QString deprttime;
    QString depdate;
    QString deprtdate;
    QString to;
    QString toid;
    QString totrack;
    QString arivtime;
    QString arivrttime;
    QString arivdate;
    QString arivrtdate;

signals:

public slots:

};

#endif // LEG_H
