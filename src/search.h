#ifndef SEARCH_H
#define SEARCH_H

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
#include "parser.h"

#include <QQmlEngine>
#include <QJSEngine>
#include <QtQuick>
#include <qqml.h>

#include <QObject>




class Search : public QObject
{
    Q_OBJECT
public:
    explicit Search(QObject *parent = 0);

    Q_INVOKABLE bool search();
    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString time);
    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString hour, QString minute);

    Q_INVOKABLE QObject * getTrip(int index);
    Q_INVOKABLE QObject * getLeg(int tripindex, int legindex);
    Q_INVOKABLE int getnumlegs(int tripindex);

    Q_INVOKABLE void cleartrips();

    Q_INVOKABLE QString getfromid() {
        return mparser->fromid;
    }
    Q_INVOKABLE QString gettoid() {
        return mparser->toid;
    }
    Q_INVOKABLE QString getfrom() {
        return mparser->from;
    }
    Q_INVOKABLE QString getto() {
        return mparser->to;
    }
    Q_INVOKABLE QString getdate() {
        return mparser->date;
    }
    Q_INVOKABLE QString gettime() {
        return mparser->time;
    }
    Q_INVOKABLE QString getdateofsearch() {
        return mparser->dateofsearch;
    }
    Q_INVOKABLE QString gettimeofsearch() {
        return mparser->timeofsearch;
    }

    Q_INVOKABLE void setfromid(QString fromid) {
        mparser->fromid = fromid;
    }
    Q_INVOKABLE void settoid(QString toid) {
        mparser->toid = toid;
    }
    Q_INVOKABLE void setfrom(QString from) {
        mparser->from = from;
    }
    Q_INVOKABLE void setto(QString to) {
        mparser->to = to;
    }
    Q_INVOKABLE void setdate(QString date) {
        mparser->date = date;
    }
    Q_INVOKABLE void settime(QString time) {
        mparser->time = time;
    }
    Q_INVOKABLE void setdateofsearch(QString dateofsearch) {
        mparser->dateofsearch = dateofsearch;
    }
    Q_INVOKABLE void settimeofsearch(QString timeofsearch) {
        mparser->timeofsearch = timeofsearch;
    }
signals:
    void ready(QString err);
    void searching();

public slots:
    void parseready(QString err);

private:
    Parser * mparser;
};
#endif // SEARCH_H
