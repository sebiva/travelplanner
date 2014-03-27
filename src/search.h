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

    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString time);
    Q_INVOKABLE bool search(QString fromid, QString toid, QString date, QString hour, QString minute);

    Q_INVOKABLE QObject * getTrip(int index);
    Q_INVOKABLE QObject * getLeg(int tripindex, int legindex);
    signals:
    void ready(int err);

public slots:
    void parseready(int err);

private:
    Parser * mparser;
};
#endif // SEARCH_H
