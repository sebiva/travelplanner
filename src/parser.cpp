#include "parser.h"

Parser::Parser(QObject *parent) :
    QObject(parent)
{

    trips = new QList<Trip>();
}
QString hello = "a";
QString address = (QString) "http://api.vasttrafik.se/bin/rest.exe/" +
                  "v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
QString addresshardcoded = "http://api.vasttrafik.se/bin/rest.exe/v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml&date=2014-03-23&time=11:00&originId=9021014001200000&destId=9021014001050000";


bool Parser::getXML(QString fromid, QString toid,  QString date, QString time) {
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(XMLready(QNetworkReply*)) );
//    manager->get(QNetworkRequest(QUrl(address +
//                                      "&date=" + date +
//                                      "&time=" + time +
//                                      "&originId=" + fromid +
//                                      "&destId=" + toid)));

    manager->get(QNetworkRequest(QUrl(addresshardcoded)));
    return true;
}



void Parser::XMLready( QNetworkReply * reply){
    parsevasttrafikreply(reply);
}

void Parser::parsevasttrafikreply(QNetworkReply *reply) {
    QXmlStreamReader xml;
    xml.setDevice(reply);
    QXmlStreamAttributes attr;
    xml.readNextStartElement(); //Triplist
    xml.readNextStartElement(); //Trip
    while(!xml.isEndElement()) {
        Trip *trip = new Trip();
        xml.readNextStartElement();//Leg
        while(!xml.isEndElement()) {
            Leg *leg = new Leg();
            qDebug() << "Leg: " << xml.name() << xml.attributes().value("name");

            attr = xml.attributes();
            leg->line = attr.value("sname").toString();
            leg->dir = attr.value("direction").toString();
            leg->fgColour = attr.value("fgColor").toString();
            leg->bgColour = attr.value("bgColor").toString();

            xml.readNextStartElement(); //Leginfo

            while(!xml.isEndElement()) {
                qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");

                attr = xml.attributes();
                if (xml.name() == "Origin") {
                    leg->from = attr.value("name").toString();
                    leg->fromid = attr.value("name").toString();
                    leg->fromtrack = attr.value("track").toString();
                    leg->depdate = attr.value("date").toString();
                    leg->deptime = attr.value("time").toString();
                    leg->deprtdate = attr.value("rtDate").toString();
                    leg->deprttime = attr.value("rtTime").toString();
                } else if (xml.name() == "Destination") {
                    leg->to = attr.value("name").toString();
                    leg->toid = attr.value("name").toString();
                    leg->totrack = attr.value("track").toString();
                    leg->arivdate = attr.value("date").toString();
                    leg->arivtime = attr.value("time").toString();
                    leg->arivrtdate = attr.value("rtDate").toString();
                    leg->arivrttime = attr.value("rtTime").toString();
                }
                xml.skipCurrentElement();
                xml.readNextStartElement();
            }
            qDebug() << "after2" << xml.name();
            xml.readNextStartElement();
            qDebug() << "after3" << xml.name();





            // Fix name
            const Leg legadd = *leg;
            trip->addleg(legadd);
        }
        xml.readNextStartElement();
        qDebug() << "afterlegs2" << xml.name();
        const Trip tripadd = *trip;
        trips->append(tripadd);
    }
}

QString Parser::hej() {
    hello = hello + "a";
    return hello;
}
