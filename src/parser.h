#ifndef PARSER_H
#include <QNetworkAccessManager>
#include <QUrl>
#include <QHttpPart>
#include <QTemporaryFile>
#include <QNetworkReply>

#include <QXmlStreamReader>

#include <QtDebug>
#include <QDebug>

#include <QList>
#include <QStringList>
#include "trip.h"
#include "leg.h"



#include <QQmlEngine>
#include <QJSEngine>
#include <QtQuick>
#include <qqml.h>

#define PARSER_H

#include <QObject>




class Parser : public QObject
{
    Q_OBJECT
public:

    //static QObject *qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine);
    static Parser *getinstance();
    bool getXML(QString fromid, QString toid, QString date, QString time);

    Trip * getTrip(int index);
    Leg * getLeg(int tripindex, int legindex);

    int numtrips();
    int numlegs(int tripindex);

    QString fromid;
    QString toid;
    QString from;
    QString to;
    QString time;
    QString date;
    QString timeofsearch;
    QString dateofsearch;

signals:
    void ready(int err);

public slots:
    void XMLready(QNetworkReply *);

private:
    void parsevasttrafikreply(QNetworkReply * reply);
    QList<Trip *> *trips;
    QString address;
    static Parser *mparser;

    explicit Parser(QObject *parent = 0);
};

#endif // PARSER_H
