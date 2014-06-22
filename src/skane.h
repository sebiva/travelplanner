#ifndef SKANE_H
#define SKANE_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

#include <QXmlStreamReader>

#include <QtDebug>

#include <QList>
#include <QStringList>
#include "trip.h"
#include "leg.h"
#include "parser.h"

class Skane : public Parser
{
    Q_OBJECT
public:
    static Parser *getinstance();
    ~Skane();

    bool getXML(QString fromid, QString toid, QString date, QString time);
    bool getstops(QString str);

signals:

public slots:
    void parsestops(QNetworkReply *reply);
    void parsereply(QNetworkReply *reply);
private:
    bool skiptoendof(QXmlStreamReader *reader, QString str);

    static QString address;
    static QString nameaddress;
    static Skane *mskane;
    explicit Skane();
};

#endif // SKANE_H
