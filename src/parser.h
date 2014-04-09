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

#include <QDateTime>
#include <QDate>
#include <QTime>

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
    static QString getstop(int i);
    bool getstops(QString str);
    int numstops();
    bool getXML(QString fromid, QString toid, QString date, QString time);

    Trip * getTrip(int index);
    Leg * getLeg(int tripindex, int legindex);

    int numtrips();
    int numlegs(int tripindex);
    void cleartrips();

    QString removespecials(QString str);

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
    void stopsparsed(QString err);

public slots:
    void stopsreply(QNetworkReply *reply);
    void XMLready(QNetworkReply *reply);



private:
    void vasttrafikstops(QNetworkReply *reply);
    void parsevasttrafikreply(QNetworkReply *reply);
    QString address;
    QString nameaddress;
    static Parser *mparser;
    static QList<Trip *> *trips;
    static QList<QString> *stops;

    explicit Parser(QObject *parent = 0);
};

#endif // PARSER_H
