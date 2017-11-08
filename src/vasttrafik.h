#ifndef VASTTRAFIK_H
#define VASTTRAFIK_H

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

class Vasttrafik : public Parser
{
    Q_OBJECT
public:
    static Parser *getinstance();
    ~Vasttrafik();

    bool getXML(QString fromid, QString toid, QString date, QString time);
    bool getstops(QString str);
    QByteArray get_token();

    QNetworkRequest build_request(QString url);

signals:
    void parsed_token();
public slots:
    void parsestops(QNetworkReply *reply);
    void parsereply(QNetworkReply *reply);
    void parse_token(QNetworkReply *reply);
private:
    static QString address;
    static QString nameaddress;
    static Vasttrafik *mvasttrafik;
    static QString secret;
    QByteArray token;
    QDateTime expiry;
    explicit Vasttrafik();
};

#endif // VASTTRAFIK_H
