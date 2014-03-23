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
#include "trip.h"
#include "leg.h"

#define PARSER_H

#include <QObject>




class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);
    Q_INVOKABLE QString hej();
    Q_INVOKABLE bool getXML(QString fromid, QString toid, QString date, QString time);
    void parsevasttrafikreply(QNetworkReply * reply);

    QList<Trip> *trips;
signals:

public slots:
    void XMLready(QNetworkReply *);

};

#endif // PARSER_H
