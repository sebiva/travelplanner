#include "sl.h"

QString SL::address =
        "https://api.trafiklab.se/sl/reseplanerare.json?Timesel=depart&Lang=en&key=pZwMMjTu8Ye8bReCbEiegba4AHUoWnJg";
QString SL::nameaddress =
        "https://api.trafiklab.se/sl/realtid/GetSite.xml?key=pZwMMjTu8Ye8bReCbEiegba4AHUoWnJg&stationSearch=";
SL *SL::msl = 0;

QString SL::red = "#F1491c";
QString SL::green = "#6ec72d";
QString SL::blue = "#25368b";


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
    manager->get(QNetworkRequest(QUrl(address +
                                      "&Date=" + tosldate(date) +
                                      "&Time=" + time +
                                      "&S=" + fromid +
                                      "&Z=" + toid
                                      )));
    qDebug()<<"ADDRESS::"<<address + "&Date=" + tosldate(date) + "&Time=" + time + "&S=" + fromid + "&Z=" + toid;
    return true;
}

void SL::parsereply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured";
        emit replyready("No connection");
        sender()->deleteLater();
        return;
    }

    trips->clear();
    QTextCodec *cod = QTextCodec::codecForName("iso-8859-1");
    QString bytes = cod->toUnicode(reply->readAll());

    //qDebug() << bytes;
    QJsonParseError err;
    QJsonDocument jsondoc;
    jsondoc = QJsonDocument::fromJson(bytes.toUtf8(),&err);


    //qDebug() << jsondoc.isObject() << jsondoc.isArray();

    QJsonObject root = jsondoc.object();
    //qDebug() << root.keys();
    QJsonObject hafas = root.value("HafasResponse").toObject();
    //qDebug() << hafas.keys();


    QJsonArray triplist;
    int lentrips;
    if (hafas.value("Trip").isObject()) {
        lentrips = 1;
    } else {
        triplist = hafas.value("Trip").toArray();
        lentrips = triplist.size();
    }


    for (int i = 0; i < triplist.size(); i++) {
        //qDebug() << "######################### NEW TRIP" << i << "/" << triplist.size();
        Trip *strip = new Trip();
        strip->setParent(this);
        strip->valid = true;
        strip->passed = false;
        strip->errmsg = "";

        //Check if there is only one trip
        QJsonObject trip;
        if (lentrips == 1) {
            trip = hafas.value("Trip").toObject();
        } else {
            trip = triplist.at(i).toObject();
        }

        QJsonObject summary = trip.value("Summary").toObject();

        strip->depdate = toappdate(summary.value("DepartureDate").toString());
        strip->deprtdate = strip->depdate;
        strip->deptime = summary.value("DepartureTime").toObject().value("#text").toString();
        strip->deprttime = strip->deptime;
        strip->arivdate = toappdate(summary.value("ArrivalDate").toString());
        strip->arivrtdate = strip->arivdate;
        strip->arivtime = summary.value("ArrivalTime").toObject().value("#text").toString();
        strip->arivrttime = strip->arivtime;


        //qDebug() << strip->depdate << strip->deptime << strip->arivdate << strip->arivrttime;

        //See if the subtrips are in a list
        int len;
        QJsonArray subtriplist;
        if(trip.value("SubTrip").isObject()) {
            len = 1;
        } else {
            subtriplist = trip.value("SubTrip").toArray();
            len = subtriplist.size();
        }

        Leg *leg = NULL;
        for (int j = 0; j < len; j++) {
            //qDebug() << "######################### NEW SUBTRIP" << j << "/" << subtriplist.size();
            leg = new Leg();
            leg->setParent(strip);

            //Check if the Subtrips are in a list
            QJsonObject subtrip;
            if (len == 1) {
                subtrip = trip.value("SubTrip").toObject();
            } else {
                subtrip = subtriplist.at(j).toObject();
            }

            QJsonObject transport = subtrip.value("Transport").toObject();
            //Add leg info
            leg->mline = transport.value("Line").toString();
            leg->mdir = transport.value("Towards").toString();

            qDebug() << leg->mline << leg->mdir;

            QString type = transport.value("Type").toString();
            qDebug() << "Type" << type;
            if (type == "MET") {
                if (leg->mline == "10" || leg->mline == "11") {
                    leg->mfgcolour = blue;
                } else if (leg->mline == "13" || leg->mline == "14") {
                    leg->mfgcolour = red;
                } else if (leg->mline == "17" || leg->mline == "18" || leg->mline == "19") {
                    leg->mfgcolour = green;
                }
                leg->mbgcolour = "#ffffff";
                leg->mline = "T" + leg->mline;
            } else if (type == "BUS") {
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#00abe5";
            } else if (type == "SHP") {
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
            } else if (type == "TRN") {
                leg->mfgcolour = "#000000";
                leg->mbgcolour = "#ffffff";
            } else if (type == "TRM") {
                leg->mfgcolour = "#ffe600";
                leg->mbgcolour = "#00abe5";
            } else {
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#00abe5";
            }




            QJsonObject origin = subtrip.value("Origin").toObject();
            //Add origin info
            leg->mfrom = origin.value("#text").toString();
            leg->mfromid = "null";
            leg->mfromtrack = "";
            leg->mdepdate = toappdate(subtrip.value("DepartureDate").toString());
            leg->mdeprtdate = leg->mdepdate;
            leg->mdeptime = subtrip.value("DepartureTime").toObject().value("#text").toString();
            leg->mdeprttime = leg->mdeptime;


            //qDebug() << leg->mfrom << leg->mdepdate << leg->mdeptime;

            QJsonObject destination = subtrip.value("Destination").toObject();
            //Add destination info
            leg->mto = destination.value("#text").toString();
            leg->mtoid = "null";
            leg->mtotrack = "";
            leg->marivdate = toappdate(subtrip.value("ArrivalDate").toString());
            leg->marivrtdate = leg->marivdate;
            leg->marivtime = subtrip.value("ArrivalTime").toObject().value("#text").toString();
            leg->marivrttime = leg->marivtime;

            //qDebug() << leg->mto << leg->marivdate << leg->marivtime;


            //Add it
            leg->calculatetimes();
            strip->addleg(leg);
        }

        //Check if the departure time has passed, if so notify by setting errmsg.
        if (Timehelper::beforenow(strip->deprtdate, strip->deprttime)) {
            strip->passed = true;
            strip->errmsg = tr("Departed");
        }

        //Add it
        strip->calculatetimes();
        trips->append(strip);

    }

    emit replyready("");
    sender()->deleteLater();
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
    //qDebug() << "Hafas?" << xml.name() << xml.isEndElement();
    if(xml.name() == "ErrorMessage") {
        qDebug() << "No stops found";
        sender()->deleteLater();
        return;
    }


    xml.readNextStartElement(); //Execution time
    xml.skipCurrentElement();
    //qDebug() << "Exec true?" << xml.name() << xml.isEndElement();
    xml.readNextStartElement(); //Sites
    //qDebug() << "Sites?" << xml.name() << xml.isEndElement();

    int count = 0;
    xml.readNextStartElement(); //Read first Site
    while (!xml.isEndElement() && count < 10) {
        //qDebug() << "Site?" << xml.name() << xml.isEndElement();

        xml.readNextStartElement(); //Number
        //qDebug() << "Number?" << xml.name() << xml.isEndElement();

        QString num = xml.readElementText();

        xml.readNextStartElement(); //Name
        //qDebug() << "Name?" << xml.name() << xml.isEndDocument();

        QString name  = xml.readElementText();
        //qDebug() << "RES:" << num << name;
        stops->append(name + "#" + num);

        xml.skipCurrentElement();//Skip the site end tag
        //qDebug() << "Site true?" << xml.name() << xml.isEndElement();
        xml.readNextStartElement();//Read next site
        //qDebug() << "Site | Sites true?" << xml.name() << xml.isEndElement();
        count++;
    }
    emit stopsready("");
    sender()->deleteLater();
}




QString SL::toappdate(QString sldate) {
    QStringList l = sldate.split(".");
    return "20" + l.at(2) + "-" + l.at(1) + "-" + l.at(0);
}

QString SL::tosldate(QString appdate) {
    QStringList l = appdate.split("-");
    return l.at(2) + "." + l.at(1) + "." + l.at(0).right(2);
}
