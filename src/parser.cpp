#include "parser.h"

Parser *Parser::mparser = 0;

Parser::Parser(QObject *parent) :
    QObject(parent)
{
    trips = new QList<Trip *>();
    address = (QString) "http://api.vasttrafik.se/bin/rest.exe/" +
              "v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
    addresshardcoded = "http://api.vasttrafik.se/bin/rest.exe/v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml&date=2014-03-23&time=11:07&originId=9021014001200000&destId=9021014001050000";
}

//QObject *Parser::qobject_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
//{
//    Q_UNUSED(engine)
//    Q_UNUSED(scriptEngine)

//    Parser *p = new Parser();
//    return p;
//}

Parser *Parser::getinstance() {
    if (mparser == 0) {
        mparser = new Parser();
    }
    return mparser;
}

QString Parser::hello() {
    return "4242424242";
}



Trip * Parser::getTrip(int index) {
    return trips->at(index);
}

Leg * Parser::getLeg(int tripindex, int legindex) {
    return getTrip(tripindex)->leglist->at(legindex);
}


bool Parser::getXML(QString fromid, QString toid,  QString date, QString time) {
    qDebug() << hello();
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
            leg->mline = attr.value("name").toString();
            leg->mdir = attr.value("direction").toString();
            leg->mfgcolour = attr.value("fgColor").toString();
            leg->mbgcolour = attr.value("bgColor").toString();
            QString sname = attr.value("sname").toString();


            //TODO: translation
            if (leg->mline == "Gå") {
                qDebug() << "Hellog";
                leg->mline = "walk";
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
                leg->mdir = "Walk";
            } else if (leg->mline.split(" ").at(0) == "SJ") {
                qDebug() << "Hellos";
                leg->mline = "sj";
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#000000";
            } else if (leg->mline == "PENDELTÅG" || leg->mline == "VÄSTTÅGEN" || leg->mline == "TÅGAB REGIONTÅG") {
                qDebug() << "Hellot";
                leg->mline = "train";
                leg->mfgcolour = "#000000";
                leg->mbgcolour = "#ffffff";
            } else if (leg->mline.split(" ").at(1) == "EXPRESS") {
                qDebug() << "Helloe";
                leg->mline = (leg->mline.split(" ").at(0)).toLower();
            } else if (leg->mline.split(" ").at(1) == "ÄLVSNABBEN" || leg->mline.split(" ").at(1) == "ÄLVSNABBARE" ) {
                qDebug() << "Helloä";
                leg->mline = "älvs.";
            } else {
                leg->mline = sname;
            }
            qDebug() << "Hello";

            //Read the origin info
            xml.readNextStartElement(); //Leginfo
            attr = xml.attributes();
            if (xml.name() == "Origin") {
                leg->mfrom = attr.value("name").toString();
                leg->mfromid = attr.value("id").toString();
                leg->mfromtrack = attr.value("track").toString();
                leg->mdepdate = attr.value("date").toString();
                leg->mdeptime = attr.value("time").toString();
                leg->mdeprtdate = attr.value("rtDate").toString();
                leg->mdeprttime = attr.value("rtTime").toString();
            }
            qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");


            //Read the destination info
            xml.skipCurrentElement();
            xml.readNextStartElement();
            attr = xml.attributes();
            if (xml.name() == "Destination") {
                leg->mto = attr.value("name").toString();
                leg->mtoid = attr.value("id").toString();
                leg->mtotrack = attr.value("track").toString();
                leg->marivdate = attr.value("date").toString();
                leg->marivtime = attr.value("time").toString();
                leg->marivrtdate = attr.value("rtDate").toString();
                leg->marivrttime = attr.value("rtTime").toString();
            }
            qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");



            //There is no journeydetail for walking
            qDebug() << "Type: " << leg->mline << "Gå?" << (leg->mline=="Gå");
            if (leg->mline == "walk") { //TODO: translation

                //Go to next leg (or trip)
                xml.skipCurrentElement();
                xml.readNextStartElement();
                xml.readNextStartElement();

                //Skip unneccessary walk stuff
                if (leg->mfrom == leg->mto) {
                    qDebug() << "Walking to same stop ignored" << leg->mfrom;
                    continue;
                }

            }

            //Go to the journeydetails, and skip them
            xml.skipCurrentElement();
            xml.readNextStartElement();
            qDebug() << "Journey?: " << xml.name() << xml.attributes().value("name");
            //Skip journeydetails
            xml.skipCurrentElement();
            xml.readNextStartElement();
            qDebug() << "Journey2?: " << xml.name() << xml.attributes().value("name");

            //Read the next Leg(or trip)
            xml.readNextStartElement();
            qDebug() << "Journey3?: " << xml.name() << xml.attributes().value("name");


            Leg *legadd = leg;
            trip->addleg(legadd);
//            while(!xml.isEndElement()) {
//                qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");

//                attr = xml.attributes();
//                if (xml.name() == "Origin") {
//                    leg->mfrom = attr.value("name").toString();
//                    leg->mfromid = attr.value("name").toString();
//                    leg->mfromtrack = attr.value("track").toString();
//                    leg->mdepdate = attr.value("date").toString();
//                    leg->mdeptime = attr.value("time").toString();
//                    leg->mdeprtdate = attr.value("rtDate").toString();
//                    leg->mdeprttime = attr.value("rtTime").toString();
//                } else if (xml.name() == "Destination") {
//                    leg->mto = attr.value("name").toString();
//                    leg->mtoid = attr.value("name").toString();
//                    leg->mtotrack = attr.value("track").toString();
//                    leg->marivdate = attr.value("date").toString();
//                    leg->marivtime = attr.value("time").toString();
//                    leg->marivrtdate = attr.value("rtDate").toString();
//                    leg->marivrttime = attr.value("rtTime").toString();
//                }
//                xml.skipCurrentElement();
//                xml.readNextStartElement();
//            }

        }
        xml.readNextStartElement();
        qDebug() << "afterlegs2" << xml.name();
        Trip *tripadd = trip;
        trips->append(tripadd);
    }


    emit ready(0);
}

