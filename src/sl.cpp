#include "sl.h"

QString SL::address = "https://api.sl.se/api2/TravelplannerV2/trip.json?key=6cce74a4716e4b75a84f06fd6c987fbb";

QString SL::nameaddress =
        "https://api.sl.se/api2/typeahead.json?key=4b4b702b390d4c07a0f57b9b1d040d89&stationsonly=True&maxresults=10&searchstring=";

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
    QString req = (address +
            "&date=" + date + //tosldate(date) +
            "&time=" + time +
            "&originId=" + fromid +
            "&destId=" + toid);
    manager->get(QNetworkRequest(QUrl(req)));
    qDebug()<<"ADDRESS::"<< req;
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
    QString bytes2 = QString(bytes.toLatin1()); // Seems to work..

    //qDebug() << QString(bytes.toLatin1());
    QJsonParseError err;
    QJsonDocument jsondoc;
    jsondoc = QJsonDocument::fromJson(bytes2.toUtf8(),&err);


    //qDebug() << jsondoc.isObject() << jsondoc.isArray();

    QJsonObject root = jsondoc.object();
    //qDebug() << root.keys();

    QJsonObject root2 = root["TripList"].toObject();
    //qDebug() << "List?" << root2["Trip"].isArray();
    if (!root2.contains("Trip")) {
        qDebug() << "No trips found";
        emit replyready("No trips found");
        sender()->deleteLater();
        return;
    }
    QJsonArray triplist = QJsonArray();
    // If there is only on trip, its not in a list...
    if (!root2["Trip"].isArray() && root2["Trip"].isObject()) {
        triplist.append(root2["Trip"].toObject());
    } else {
        triplist = root2["Trip"].toArray();
    }

    for (int i = 0; i < triplist.size(); i++) {
        qDebug() << "######################### NEW TRIP" << i << "/" << triplist.size();
        Trip *strip = new Trip();
        strip->setParent(this);
        strip->valid = true;
        strip->passed = false;
        strip->errmsg = "";

        QJsonObject trip = triplist.at(i).toObject();
        QJsonObject trip2 = trip["LegList"].toObject();
        //qDebug() << "Legs" << trip2["Leg"].isArray();

        QJsonArray leglist = QJsonArray();
        // If there is only one leg, its not in a list...
        if (!trip2["Leg"].isArray() && trip2["Leg"].isObject()) {
            leglist.append(trip2["Leg"].toObject());
        } else {
            leglist = trip2["Leg"].toArray();
        }
        Leg *leg = NULL;
        for (int j = 0; j < leglist.size(); j++) {
            qDebug() << "######################### NEW Leg" << j << "/" << leglist.size();
            leg = new Leg();
            leg->setParent(strip);

            QJsonObject legobj = leglist.at(j).toObject();
            //qDebug() << legobj.keys();
            if (legobj.contains("RTUMessages")) {
                QJsonArray errlist = QJsonArray();
                QJsonObject rtus = legobj["RTUMessages"].toObject();
                if (rtus.contains("RTUMessage") && !rtus["RTUMessage"].isArray() && rtus["RTUMessage"].isObject()) {
                    errlist.append(rtus["RTUMessage"].toObject());
                } else if (rtus["RTUMessage"].isArray()){
                    errlist = rtus["RTUMessage"].toArray();
                }
                for (int k = 0; k < errlist.size(); k++) {
                    QJsonObject rtu = errlist.at(k).toObject();
                    qDebug() << "RTU Message found!" << rtu.keys();
                    if (rtu.contains("$")) {
                        QString msg = rtu["$"].toString();
                        qDebug() << "Message: " << msg;
                        if (msg.contains("inställd", Qt::CaseInsensitive )) {
                            qDebug() << "Cancelled!!";
                            strip->errmsg = tr("Cancelled");
                        }
                    }
                }
            }


            QJsonObject origin = legobj["Origin"].toObject();
            QJsonObject dest = legobj["Destination"].toObject();

            QString type = legobj["type"].toString();
            if (type == "WALK") {
                leg->mdir = tr("Walk");
                leg->mline = tr("Walk");
            } else {
                leg->mdir= legobj["dir"].toString();
                leg->mline = legobj["line"].toString();
            }
            qDebug() << leg->mline << (leg->mdir);

            if (type == "METRO") {
                if (leg->mline == "10" || leg->mline == "11") {
                    leg->mfgcolour = blue;
                } else if (leg->mline == "13" || leg->mline == "14") {
                    leg->mfgcolour = red;
                } else if (leg->mline == "17" || leg->mline == "18" || leg->mline == "19") {
                    leg->mfgcolour = green;
                }
                leg->mbgcolour = "#ffffff";
                leg->mline = "T" + leg->mline;
            } else if (type == "BUS" || type == "NARBUSS") {
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#00abe5";
            } else if (type == "SHIP" || type == "FERRY") {
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
            } else if (type == "TRAIN") {
                leg->mfgcolour = "#000000";
                leg->mbgcolour = "#ffffff";
            } else if (type == "TRAM") {
                leg->mfgcolour = "#ffe600";
                leg->mbgcolour = "#00abe5";
            } else {
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#00abe5";
            }

            //Add origin info
            leg->mfrom = origin["name"].toString();
            leg->mfromid = "null";
            leg->mfromtrack = "";
            leg->mdepdate = origin["date"].toString();
            if (origin.contains("rtDate")) {
                leg->mdeprtdate = origin["rtDate"].toString();
            } else {
                leg->mdeprtdate = leg->mdepdate;
            }
            leg->mdeptime = origin["time"].toString();
            if (origin.contains("rtTime")) {
                leg->mdeprttime = origin["rtTime"].toString();
            } else {
                leg->mdeprttime = leg->mdeptime;
            }



            qDebug() << leg->mfrom << leg->mdepdate << leg->mdeptime;

            //Add destination info
            leg->mto = dest["name"].toString();
            leg->mtoid = "null";
            leg->mtotrack = "";
            leg->marivdate = dest["date"].toString();
            if (dest.contains("rtDate")) {
                leg->marivrtdate = dest["rtDate"].toString();
            } else {
                leg->marivrtdate = leg->marivdate;
            }
            leg->marivtime = dest["time"].toString();
            if (dest.contains("rtTime")) {
                leg->marivrttime = dest["rtTime"].toString();
            } else {
                leg->marivrttime = leg->marivtime;
            }

            // Remove unnecessary walking
            if (type == "WALK" && (leg->mfrom == leg->mto) && (j != (leglist.size() -1))) {
                continue;
            }

            qDebug() << leg->mto << leg->marivdate << leg->marivtime;

            if (j == 0) { //First leg -> Trip departure
                strip->depdate = leg->mdepdate;
                strip->deprtdate = leg->mdeprtdate;
                strip->deptime = leg->mdeptime;
                strip->deprttime = leg->mdeptime;
            }
            if (j == (leglist.size() -1)) { //Last leg -> Trip arrival
                strip->arivdate = leg->marivdate;
                strip->arivrtdate = leg->marivrtdate;
                strip->arivtime = leg->marivtime;
                strip->arivrttime = leg->marivrttime;
            }

            //Add it
            leg->calculatetimes();
            strip->addleg(leg);
        }

        //Check if the departure time has passed, if so notify by setting errmsg.
        if (Timehelper::beforenow(strip->deprtdate, strip->deprttime)) {
            strip->passed = true;
            if (strip->errmsg == "") { // Don't overwrite cancelled status
                strip->errmsg = tr("Departed");
            }

        }

        //Add it
        strip->calculatetimes();
        trips->append(strip);

    }

    qDebug() << "Parsing done, no errors!";
    emit replyready("");
    sender()->deleteLater();
}


bool SL::getstops(QString str) {
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsestops_json(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(nameaddress + removespecials_sl_skane(str))));
    qDebug()<<"NAMEADDRESS::"<<(nameaddress+removespecials_sl_skane(str));
    return true;
}

void SL::parsestops_json(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "No connection in stoplist search";
        sender()->deleteLater();
        return;
    }
    qDebug() << "Parsing stops!";

    QString strReply = (QString)reply->readAll();
    stops->clear();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject obj = jsonResponse.object();

    //qDebug() << obj.keys();
    //qDebug() << obj["ResponseData"].toArray();
    QJsonArray array = obj["ResponseData"].toArray();
    int count = 0;
    while(array.size() > count && count < 10) {
        if (!array.at(count).isObject()) {
            break;
        }
        QJsonObject elem = array.at(count).toObject();
        qDebug() << "Name" << elem["Name"].toString() << "Id" << elem["SiteId"].toString().toInt()
                    ;
        count++;
        QString id = QString::number(elem["SiteId"].toString().toInt());
        stops->append(elem["Name"].toString() + "#" + id);
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

// For testing RTU-messages..
QString SL::teststr = "{\"TripList\":{"
                 " \"noNamespaceSchemaLocation\":\"hafasRestTrip.xsd\","
                  "\"Trip\":{"
                   " \"dur\":\"10\","
                    "\"chg\":\"0\","
                    "\"co2\":\"0.00\","
                    "\"LegList\":{"
                    "  \"Leg\":{"
                     "   \"idx\":\"0\","
                      "  \"name\":\"tunnelbanans gröna linje 19\","
                      "  \"type\":\"METRO\","
                      "  \"dir\":\"Hagsätra\","
                      "  \"line\":\"19\","
                      "  \"Origin\":{"
                      "    \"name\":\"Hässelby strand\","
                      "    \"type\":\"ST\","
                      "    \"id\":\"400101332\","
                      "    \"lon\":\"17.832468\","
                      "    \"lat\":\"59.361741\","
                      "    \"routeIdx\":\"0\","
                      "    \"time\":\"15:30\","
                      "    \"date\":\"2015-02-14\""
                      "    },"
                      "  \"Destination\":{"
                      "    \"name\":\"Islandstorget\","
                      "    \"type\":\"ST\","
                      "    \"id\":\"400101262\","
                      "    \"lon\":\"17.893217\","
                      "    \"lat\":\"59.345992\","
                      "    \"routeIdx\":\"6\","
                      "    \"time\":\"15:40\","
                      "    \"date\":\"2015-02-14\""
                      "    },"
                      "  \"RTUMessages\":{"
                      "      \"RTUMessage\":[{"
                      "          \"$\":\"Inställd pga trafikhändelse\""
                      "      },"
                      "      {\"$\":\"Hej\"}]"
                      "    },"
                      "  \"JourneyDetailRef\":{"
                      "    \"ref\":\"ref%3D152385%2F56781%2F314894%2F106654%2F74%3Fdate%3D2015-02-14%26station_evaId%3D400101332%26station_type%3Ddep%26lang%3Dsv%26format%3Djson%26\""
                      "    },"
                      "  \"GeometryRef\":{"
                      "    \"ref\":\"ref%3D152385%2F56781%2F314894%2F106654%2F74%26startIdx%3D0%26endIdx%3D6%26lang%3Dsv%26format%3Djson%26\""
                      "    }"
                      "  }"
                      "},"
                    "\"PriceInfo\":{"
                     " \"TariffZones\":{"
                      "  \"$\":\"A\""
                      "  },"
                      "\"TariffRemark\":{"
                      "  \"$\":\"2 biljett\""
                      "  }"
                      "}"
                    "}"
                  "}"
                "}";
