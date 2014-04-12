#include "sl.h"

QString SL::address =
        "http://api.vasttrafik.se/bin/rest.exe/v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
QString SL::nameaddress =
        "https://api.trafiklab.se/sl/realtid/GetSite.xml?key=pZwMMjTu8Ye8bReCbEiegba4AHUoWnJg&stationSearch=";
SL *SL::msl = 0;


SL::SL()
{
}

SL::~SL() {
    qDebug() << "Deleting SL";
}

Parser *SL::getinstance() {
    if (msl == 0) {
        msl = new SL();
    }
    return msl;
}

bool SL::getXML(QString fromid, QString toid, QString date, QString time) {
    qDebug() << "SEARCHING::" << date << time << fromid << toid;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsereply(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(address)));
    qDebug()<<"ADDRESS::"<<address;
    return true;
}

void SL::parsereply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured";
        emit replyready("No connection");
        sender()->deleteLater();
        return;
    }
    QXmlStreamReader xml;
}


bool SL::getstops(QString str) {
    qDebug() << "SEARCHING STOPS::" << str;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsestops(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(nameaddress + str)));
    qDebug()<<"NAMEADDRESS::"<<(nameaddress+str);
    return true;
}

void SL::parsestops(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "No connection in stoplist search";
        sender()->deleteLater();
        return;
    }
    QXmlStreamReader xml;
    xml.setDevice(reply);
    stops->clear();
    xml.readNextStartElement(); //Hafas
    qDebug() << "Hafas?" << xml.name() << xml.isEndElement();
    if(xml.name() == "ErrorMessage") {
        qDebug() << "No stops found";
        sender()->deleteLater();
        return;
    }


    xml.readNextStartElement(); //Execution time
    xml.skipCurrentElement();
    qDebug() << "Exec true?" << xml.name() << xml.isEndElement();
    xml.readNextStartElement(); //Sites
    qDebug() << "Sites?" << xml.name() << xml.isEndElement();

    int count = 0;
    xml.readNextStartElement(); //Read first Site
    while (!xml.isEndElement() && count < 10) {
        qDebug() << "Site?" << xml.name() << xml.isEndElement();

        xml.readNextStartElement(); //Number
        qDebug() << "Number?" << xml.name() << xml.isEndElement();

        QString num = xml.readElementText();

        xml.readNextStartElement(); //Name
        qDebug() << "Name?" << xml.name() << xml.isEndDocument();

        QString name  = xml.readElementText();
        qDebug() << "RES:" << num << name;
        stops->append(name + "#" + num);
        count++;

        xml.skipCurrentElement();//Skip the site end tag
        qDebug() << "Site true?" << xml.name() << xml.isEndElement();
        xml.readNextStartElement();//Read next site
        qDebug() << "Site | Sites true?" << xml.name() << xml.isEndElement();
        count++;
    }



}
