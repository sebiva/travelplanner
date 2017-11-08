#include "vasttrafik.h"

QString Vasttrafik::address =
        "https://api.vasttrafik.se/bin/rest.exe/v2/trip";
QString Vasttrafik::nameaddress =
        "https://api.vasttrafik.se/bin/rest.exe/v2/location.name?input=";
Vasttrafik *Vasttrafik::mvasttrafik = 0;

QString Vasttrafik::secret = QByteArray("Basic WUcxMjltQ0VoZmpQMDF6QThLNmpvUURSVjVJYTpSZnBBcW12c3hXYllpMW9ZaFNSZlRxZ3dQSXNh", -1);
QByteArray token;
QDateTime expiry;

Vasttrafik::Vasttrafik()
{
}

Vasttrafik::~Vasttrafik() {
    qDebug() << "Deleting Vasttrafik";
}
/*
 * Returns the singleton västtrafik parser.
 */
Parser *Vasttrafik::getinstance() {
    if (mvasttrafik == 0) {
        mvasttrafik = new Vasttrafik();
    }
    return mvasttrafik;
}

QByteArray Vasttrafik::get_token() {
    if (expiry.isNull() || expiry < QDateTime::currentDateTimeUtc()) {
        //Get a new token.

        //Set up timer and loop to wait for the post request to complete and be parsed
        QTimer timer;
        timer.setSingleShot(true);
        QEventLoop loop;
        QNetworkAccessManager * manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(parse_token(QNetworkReply*)));
        connect(this, SIGNAL(parsed_token()),
                &loop, SLOT(quit()) );

        QUrl url = QUrl("https://api.vasttrafik.se/token");
        QNetworkRequest request = QNetworkRequest(url);
        request.setRawHeader(QByteArray("Content-Type", -1), QByteArray("application/x-www-form-urlencoded", -1));
        request.setRawHeader(QByteArray("Authorization", -1), secret.toUtf8());
        QString id = "42"; // Static for now

        manager->post(request, QByteArray("grant_type=client_credentials&scope=", -1) + id.toUtf8());

        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        //Wait max 5 seconds for the network request to arrive
        timer.start(5000);
        loop.exec();
        if(timer.isActive())
            qDebug() << "New token received";
        else
            qDebug() << "No new token, something failed";
        return token;
    } else {
        qDebug() << "Reusing token, expires: " << expiry.toString();
        return token;
    }
}

void Vasttrafik::parse_token(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error getting token: " << reply->error();
        sender()->deleteLater();
        return;
    }
    QString strReply = (QString)reply->readAll();

    qDebug() << "Response:" << strReply;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObj = jsonResponse.object();

    token = (jsonObj["token_type"].toString() + " " + jsonObj["access_token"].toString()).toUtf8();
    int expires_in = jsonObj["expires_in"].toInt();
    if (expiry.isNull()) {
        expiry = QDateTime::currentDateTimeUtc();
    }
    expiry = expiry.addSecs(expires_in);
    qDebug() << "token:" << token << "expires:" << expiry.toString();
    emit parsed_token();
}

QNetworkRequest Vasttrafik::build_request(QString url) {
    url = url + "&format=xml";
    QNetworkRequest request = QNetworkRequest(url);
    QByteArray token = this->get_token();
    request.setRawHeader(QByteArray("Authorization", -1), token);
    return request;
}

/*
 * Takes information about a trip and makes a search. Eventually emits ready() when the result is available
 * in the list trips.
 */
bool Vasttrafik::getXML(QString fromid, QString toid, QString date, QString time) {
    QString url = address +
                  "?originId=" + fromid +
                  "&destId=" + toid +
                  "&date=" + date +
                  "&time=" + time;

    qDebug()<<"ADDRESS::"<< url;

    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsereply(QNetworkReply*)) );
    manager->get(build_request(url));

    return true;
}

/*
 * Parses the search reply, putting the resulting trip info in the trips list. Emits ready() when done,
 * containing a message for any error  that occured.
 */
void Vasttrafik::parsereply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured: " << reply->error();
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
        bool cancelled = false;
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


            //Fix approriate line name
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
            } else if (leg->mline.split(" ").length() > 0 && leg->mline.split(" ").at(0) == "STENUNGSUND") {
                leg->mline = "sten.";
            } else if (leg->mline.split(" ").length() > 1 && leg->mline.split(" ").at(1) == "EXPRESS") {
                leg->mline = (leg->mline.split(" ").at(0)).toLower();
            } else if (leg->mline.split(" ").length() > 1 && (leg->mline.split(" ").at(1) == "ÄLVSNABBEN" || leg->mline.split(" ").at(1) == "ÄLVSNABBARE") ) {
                leg->mline = "älvs.";
            } else {
                leg->mline = sname;
            }

            if(attr.value("cancelled").toString() != "") {
                qDebug() << "Cancelled";
                leg->merrstatus = "Cancelled";
                cancelled = true;
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



            //There is no journeydetail for walking
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
                //qDebug() << "Walking" << leg->mdir << xml.name() << xml.isEndElement();
                continue;
            }
            //qDebug() << "Journey" << leg->mdir << xml.name() << xml.isEndElement();
            //Go to the journeydetails, and skip them
            while(xml.name() != "JourneyDetailRef") {
                xml.skipCurrentElement();
                xml.readNextStartElement();
            }


            //Skip journeydetails
            xml.skipCurrentElement();
            xml.readNextStartElement();
            //Read the next Leg(or trip)
            xml.readNextStartElement();

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

        if (Timehelper::beforenow(trip->deprtdate, trip->deprttime)) {
            trip->passed = true;
            trip->errmsg = tr("Departed");
        } else {
            trip->passed = false;
        }

        if (!trip->valid) {
            qDebug() << "Invalid trip";
            if (cancelled) {
                qDebug() << "Setting cancelled";
                trip->errmsg = tr("Cancelled");
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
    str = removespecials(str);
    qDebug() << "SEARCHING STOPS::" << str;
    QString url = QString(nameaddress + str);
    qDebug()<<"NAMEADDRESS::"<< url;

    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(parsestops(QNetworkReply*)) );
    manager->get(build_request(url));

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

        QString type = xml.name().toString();
        if (type == "StopLocation") {
            count++;
            QString name = xml.attributes().value("name").toString();
            QString id = xml.attributes().value("id").toString();
            stops->append(name + "#" + id);
        }

        xml.skipCurrentElement();
        xml.readNextStartElement();
    }
    emit stopsready("");
    sender()->deleteLater();
}

