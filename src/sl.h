#ifndef SL_H
#define SL_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>

#include <QXmlStreamReader>
#include <QTemporaryFile>
#include <QJsonDocument>
#include <QByteArray>
#include <QTextCodec>

#include <QtDebug>

#include <QList>
#include <QStringList>
#include "trip.h"
#include "leg.h"
#include "parser.h"

class SL : public Parser
{
    Q_OBJECT
public:
    static Parser *getinstance();
    ~SL();

    bool getXML(QString fromid, QString toid, QString date, QString time);
    bool getstops(QString str);
signals:

public slots:
    void parsestops(QNetworkReply *reply);
    void parsereply(QNetworkReply *reply);

private:   
    static QString address;
    static QString nameaddress;
    static SL *msl;

    explicit SL();


    QString toappdate(QString sldate);
    QString tosldate(QString appdate);

    static QString red;
    static QString green;
    static QString blue;
};

#endif // SL_H
