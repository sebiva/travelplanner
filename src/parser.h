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
    ~Parser();
    bool getXML(QString fromid, QString toid, QString date, QString time);

    Trip * getTrip(int index);
    Leg * getLeg(int tripindex, int legindex);

    int numtrips();
    int numlegs(int tripindex);
    void cleartrips();

    QString fromid;
    QString toid;
    QString from;
    QString to;
    QString time;
    QString date;
    QString timeofsearch;
    QString dateofsearch;

signals:
    void ready(QString err);

public slots:
    void XMLready(QNetworkReply *);

private:
    void parsevasttrafikreply(QNetworkReply * reply);
    QString address;
    static Parser *mparser;
    static QList<Trip *> *trips;

    explicit Parser(QObject *parent = 0);
};

#endif // PARSER_H
