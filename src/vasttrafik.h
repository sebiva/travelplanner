#ifndef VASTTRAFIK_H
#define VASTTRAFIK_H

#include <QObject>


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

#include <QDateTime>
#include <QDate>
#include <QTime>

class Vasttrafik : public Parser
{
    Q_OBJECT
public:
    static Parser *getinstance();
    ~Vasttrafik();


    bool getXML(QString fromid, QString toid, QString date, QString time);
    bool getstops(QString str);

signals:

public slots:
    void parsestops(QNetworkReply *reply);
    void parsereply(QNetworkReply *reply);
private:
    static QString address;
    static QString nameaddress;
    static Vasttrafik *mvasttrafik;

    explicit Vasttrafik();
};


#endif // VASTTRAFIK_H
