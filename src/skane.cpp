#include "skane.h"

QString Skane::address =
        "http://api.vasttrafik.se/bin/rest.exe/v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
QString Skane::nameaddress =
        "http://www.labs.skanetrafiken.se/v2.2/querystation.asp?inpPointfr=";
Skane *Skane::mskane = 0;


Skane::Skane()
{
}

Skane::~Skane() {
    qDebug() << "Deleting Skane";
}
/*
 * Returns the singleton skånetrafiken parser.
 */
Parser *Skane::getinstance() {
    if (mskane == 0) {
        mskane = new Skane();
    }
    return mskane;
}

/*
 * Takes information about a trip and makes a search. Eventually emits ready() when the result is available
 * in the list trips.
 */
bool Skane::getXML(QString fromid, QString toid, QString date, QString time) {
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsereply(QNetworkReply*)) );
    // Make the search
    manager->get(QNetworkRequest(QUrl(address +
                                      "&date=" + date +
                                      "&time=" + time +
                                      "&originId=" + fromid +
                                      "&destId=" + toid)));
    qDebug()<<"ADDRESS::"<<(address+"&date="+date +"&time="+time+"&originId="+fromid+"&destId="+toid);
    return true;
}

/*
 * Parses the search reply, putting the resulting trip info in the trips list. Emits ready() when done,
 * containing a message for any error  that occured.
 */
void Skane::parsereply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured";
        emit replyready("No connection");
        sender()->deleteLater();
        return;
    }

    //qDebug() << "Parsing";
    QXmlStreamReader xml;
    xml.setDevice(reply);
    QXmlStreamAttributes attr;

    trips->clear();
    xml.readNextStartElement(); //Triplist





    qDebug() << "Parsing done, no errors!";
    emit replyready("");
    sender()->deleteLater();
}

bool Skane::getstops(QString str) {
    str = removespecials(str);
    qDebug() << "SEARCHING STOPS::" << str;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsestops(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(nameaddress + str)));
    qDebug()<<"NAMEADDRESS::"<<(nameaddress+str);
    return true;
}

void Skane::parsestops(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "No connection in stoplist search";
        sender()->deleteLater();
        return;
    }
    QXmlStreamReader xml;
    xml.setDevice(reply);
    stops->clear();
    xml.readNextStartElement(); //Envelope
    if (xml.hasError()) {
        qDebug() << "Error!!!";
        emit stopsready("Error searching for stop");
        return;
    }

    qDebug() << "Envelope" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Body" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Getstartendpointresponse" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Getstartendpointresult" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Code" << xml.name() << xml.isEndElement();
    xml.skipCurrentElement();
    qDebug() << "Code true" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Message" << xml.name() << xml.isEndElement();
    xml.skipCurrentElement();
    qDebug() << "Message true" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Startpoints" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();
    qDebug() << "Point" << xml.name() << xml.isEndElement();
    xml.readNextStartElement();


    int count = 0;
    while(!xml.isEndElement() && count < 10) {
        qDebug() << "Id" << xml.name() << xml.isEndElement();
        QString num =  xml.readElementText();
        xml.readNextStartElement();

        qDebug() << "Name" << xml.name() << xml.isEndElement();
        QString name = xml.readElementText();
        //xml.readNextStartElement();
        qDebug() << num << name;
        stops->append(name + "#" + num);

        xml.skipCurrentElement();
        qDebug() << "Point true" << xml.name() << xml.isEndElement();
        xml.readNextStartElement();
        qDebug() << "Point" << xml.name() << xml.isEndElement();
        xml.readNextStartElement();
        qDebug() << "Id" << xml.name() << xml.isEndElement();
        count++;
    }
    emit stopsready("");
    sender()->deleteLater();
}
