#include "vasttrafik.h"

QString Vasttrafik::address =
        "http://api.vasttrafik.se/bin/rest.exe/v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
QString Vasttrafik::nameaddress =
        "http://api.vasttrafik.se/bin/rest.exe/v1/location.name?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml&input=";
Vasttrafik *Vasttrafik::mvasttrafik = 0;


Vasttrafik::Vasttrafik()
{
}

Vasttrafik::~Vasttrafik() {
    qDebug() << "Deleting Vasttrafik";
}

Parser *Vasttrafik::getinstance() {
    if (mvasttrafik == 0) {
        mvasttrafik = new Vasttrafik();
    }
    return mvasttrafik;
}


bool Vasttrafik::getXML(QString fromid, QString toid, QString date, QString time) {
    qDebug() << "SEARCHING::" << date << time << fromid << toid;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsereply(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(address +
                                      "&date=" + date +
                                      "&time=" + time +
                                      "&originId=" + fromid +
                                      "&destId=" + toid)));
    qDebug()<<"ADDRESS::"<<(address+"&date="+date +"&time="+time+"&originId="+fromid+"&destId="+toid);
    return true;
}

void Vasttrafik::parsereply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured";
        emit replyready("No connection");
        sender()->deleteLater();
        return;
    }

    qDebug() << "Parsing";
    QXmlStreamReader xml;
    xml.setDevice(reply);
    QXmlStreamAttributes attr;

    trips->clear();
    xml.readNextStartElement(); //Triplist
    QString error = xml.attributes().value("error").toString();
    if (error != "") {
        qDebug() << "Error!!!" << xml.attributes().value("errorText").toString();
        emit replyready(xml.attributes().value("errorText").toString());
        return;
    }
    xml.readNextStartElement(); //Trip
    while(!xml.isEndElement()) {
        Trip *trip = new Trip();
        trip->setParent(this);
        if (xml.attributes().value("valid") == "false") {
            qDebug() << "Invalid trip!";
            trip->valid = false;
        } else {
            trip->valid = true;
        }
        //Keep track of first leg for each trip
        bool first = true;
        xml.readNextStartElement();//Leg
        Leg *leg = NULL;
        bool canceled = false;
        bool risktomiss = false;

        while(!xml.isEndElement()) {
            leg = new Leg();
            leg->setParent(trip);
            //qDebug() << "Leg: " << xml.name() << xml.attributes().value("name");

            attr = xml.attributes();
            leg->mline = attr.value("name").toString();
            leg->mdir = attr.value("direction").toString();
            leg->mfgcolour = attr.value("fgColor").toString();
            leg->mbgcolour = attr.value("bgColor").toString();
            QString sname = attr.value("sname").toString();

            if (leg->mline == "Gå") {
                leg->mline = tr("walk");
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
                leg->mdir = tr("Walk");
            } else if (leg->mline.split(" ").length() > 0 && leg->mline.split(" ").at(0) == "SJ") {
                leg->mdir.append("; ").append(leg->mline.toLower());
                leg->mline = "sj";
                leg->mfgcolour = "#000000";
                leg->mbgcolour = "#ffffff";
            } else if (leg->mline == "PENDELTÅG" || leg->mline == "VÄSTTÅGEN" || leg->mline == "NSB REGIONTÅG" ||
                       leg->mline == "TÅGAB REGIONTÅG" || leg->mline == "ÖRESUNDSTÅG") {
                leg->mdir.append("; ").append(leg->mline.toLower());
                leg->mline = tr("train");
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#000000";
            } else if (leg->mline.split(" ").length() > 1 && leg->mline.split(" ").at(0) == "STENUNGSUND") {
                leg->mline = "sten.";
            } else if (leg->mline.split(" ").length() > 1 && leg->mline.split(" ").at(1) == "EXPRESS") {
                leg->mline = (leg->mline.split(" ").at(0)).toLower();
            } else if (leg->mline.split(" ").at(1) == "ÄLVSNABBEN" || leg->mline.split(" ").at(1) == "ÄLVSNABBARE" ) {
                leg->mline = "älvs.";
            } else {
                leg->mline = sname;
            }

            //TODO: Test
            if(attr.value("canceled").toString() != "") {
                qDebug() << "Canceled";
                leg->merrstatus = "Canceled";
                canceled = true;
            } else if (attr.value("reachable").toString() != "") {
                qDebug() << "Risk to miss";
                leg->merrstatus = "Risk to miss";
                risktomiss = true;
            }

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
                if(leg->mdeprtdate == "") {
                    leg->mdeprtdate = leg->depdate();
                }
                leg->mdeprttime = attr.value("rtTime").toString();
                if(leg->mdeprttime == "") {
                    leg->mdeprttime = leg->mdeptime;
                }

                if(first) {
                    first = false;
                    trip->depdate = leg->mdepdate;
                    trip->deptime = leg->mdeptime;
                    trip->deprtdate = leg->mdeprtdate;
                    trip->deprttime = leg->mdeprttime;
                }
            }

            //qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");

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
                if(leg->marivrtdate == "") {
                    leg->marivrtdate = leg->marivdate;
                }
                leg->marivrttime = attr.value("rtTime").toString();
                if(leg->marivrttime == "") {
                    leg->marivrttime = leg->marivtime;
                }
            }
            //qDebug() << "Leginfo: " << xml.name() << xml.attributes().value("name");



            //There is no journeydetail for walking
            //qDebug() << "Type: " << leg->mline << "Gå?";
            if (leg->mline == tr("walk")) {

                //Go to next leg (or trip)
                xml.skipCurrentElement();
                xml.readNextStartElement();
                xml.readNextStartElement();

                //Skip unneccessary walk stuff
                if (leg->mfrom == leg->mto) {
                    qDebug() << "Walking to same stop ignored" << leg->mfrom;
                } else {
                    trip->addleg(leg);
                }
                continue;
            }

            //Go to the journeydetails, and skip them
            xml.skipCurrentElement();
            xml.readNextStartElement();
            //qDebug() << "Journey?: " << xml.name() << xml.attributes().value("name");
            //Skip journeydetails
            xml.skipCurrentElement();
            xml.readNextStartElement();
            //qDebug() << "Journey2?: " << xml.name() << xml.attributes().value("name");

            //Read the next Leg(or trip)
            xml.readNextStartElement();
            //qDebug() << "Journey3?: " << xml.name() << xml.attributes().value("name");

            leg->calculatetimes();
            trip->addleg(leg);
        }

        if (leg != NULL) {
            trip->arivdate = leg->marivdate;
            trip->arivtime = leg->marivtime;
            trip->arivrtdate = leg->marivrtdate;
            trip->arivrttime = leg->marivrttime;
        }

        xml.readNextStartElement();
        //qDebug() << "afterlegs2" << xml.name();

        if (Timehelper::beforenow(trip->deprtdate, trip->deprttime)) {
            trip->passed = true;
            trip->errmsg = tr("Departed");
        } else {
            trip->passed = false;
        }

        if (!trip->valid) {
            qDebug() << "Invalid trip";
            if (canceled) {
                qDebug() << "Setting canceled";
                trip->errmsg = tr("Canceled");
            } else if (risktomiss) {
                qDebug() << "Setting risk";
                trip->errmsg = tr("Risk to miss");
            }
        }
        trip->calculatetimes();

        trips->append(trip);
    }
    qDebug() << "Parsing done, no errors!";
    emit replyready("");
    sender()->deleteLater();
}

bool Vasttrafik::getstops(QString str) {
    qDebug() << "SEARCHING STOPS::" << str;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsestops(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(nameaddress + str)));
    qDebug()<<"NAMEADDRESS::"<<(nameaddress+str);
    return true;
}

void Vasttrafik::parsestops(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "No connection in stoplist search";
        sender()->deleteLater();
        return;
    }
    QXmlStreamReader xml;
    xml.setDevice(reply);
    stops->clear();
    xml.readNextStartElement(); //Locationlist
    QString error = xml.attributes().value("error").toString();
    if (error != "") {
        qDebug() << "Error!!!" << xml.attributes().value("errorText").toString();
        emit stopsready(xml.attributes().value("errorText").toString());
        return;
    }
    int count = 0;
    xml.readNextStartElement(); //First stop element
    while(!xml.isEndElement() && count < 10) {
        //qDebug() << xml.name() << xml.attributes().value("name");

        QString type = xml.name().toString();
        if (type == "StopLocation") {
            count++;
            QString name = xml.attributes().value("name").toString();
            QString id = xml.attributes().value("id").toString();
            stops->append(name + "#" + id);
        }
        //qDebug() << xml.name() << xml.attributes().value("name");
        xml.skipCurrentElement();
        //qDebug() << xml.name() << xml.isEndElement();
        xml.readNextStartElement();
        //qDebug() << xml.name() << xml.isEndElement();
    }
    emit stopsready("");
    sender()->deleteLater();
}

