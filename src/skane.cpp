#include "skane.h"

QString Skane::address =
        "http://www.labs.skanetrafiken.se/v2.2/resultspage.asp?cmdaction=next&selPointFr="; //&LastStart=2014-06-22%2016:38;
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
                                      "|" + fromid + "|0" +
                                      "&selPointTo=|" + toid + "|0" +
                                      "&LastStart=" + date + "%20" + time)));
    qDebug()<<"ADDRESS::"<<(address + "|" + fromid + "|0" + "&selPointTo=|" + toid + "|0" + "&LastStart=" + date + "%20" + time);
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

//    xml.readNextStartElement(); //Envelope
//    qDebug() << xml.name() << xml.isEndElement();

//    xml.readNextStartElement(); //Body
//    qDebug() << xml.name() << xml.isEndElement();

//    xml.readNextStartElement(); //Getjourneyresponse
//    qDebug() << xml.name() << xml.isEndElement();

//    xml.readNextStartElement(); //Getjourneyresult
//    qDebug() << xml.name() << xml.isEndElement();

//    skiptoendof(&xml, "Code");
//    qDebug() << xml.name() << xml.isEndElement();

//    skiptoendof(&xml, "Message");
//    qDebug() << xml.name() << xml.isEndElement();

//    skiptoendof(&xml, "JourneyResultKey");
//    qDebug() << xml.name() << xml.isEndElement(); //Journeys

//    xml.readNextStartElement(); //Journey
//    qDebug() << xml.name() << xml.isEndElement();

    skiptostartof(&xml, "Journeys");
    qDebug() << xml.name() << xml.isEndElement();



    //While
    while (xml.readNextStartElement() && (xml.name() == "Journey")) {
        Trip *trip = new Trip();
        trip->setParent(this);
        trip->valid = true;
        trip->passed = true;
        trip->errmsg = "";


        skiptostartof(&xml, "DepDateTime");
        qDebug() << xml.name() << xml.isEndElement();

        QStringList dep = xml.readElementText().split("T");
        trip->depdate = dep.at(0);
        trip->deprtdate = trip->depdate;
        QString deptime = dep.at(1);
        deptime.chop(3); // Remove seconds
        trip->deptime = deptime;
        trip->deprttime = trip->deptime;

        qDebug() << "Deptime" << trip->deptime;

        skiptostartof(&xml, "ArrDateTime");
        qDebug() << xml.name() << xml.isEndElement();

        QStringList ariv = xml.readElementText().split("T");
        trip->arivdate = ariv.at(0);
        trip->arivrtdate = trip->arivdate;
        QString arivtime = ariv.at(1);
        arivtime.chop(3);
        trip->arivtime = arivtime;
        trip->arivrttime = trip->arivtime;

        qDebug() << "Arrtime" << trip->arivtime;

        skiptostartof(&xml, "RouteLinks");
        qDebug() << xml.name() << xml.isEndElement();

        //While2
        while (xml.readNextStartElement() && (xml.name() == "RouteLink")) {
            //xml.readNextStartElement(); //Routelink
            qDebug() << xml.name() << xml.isEndElement();

            Leg *leg = NULL;
            leg = new Leg();
            leg->setParent(trip);

            skiptostartof(&xml, "DepDateTime");
            qDebug() << xml.name() << xml.isEndElement();

            QStringList depleg = xml.readElementText().split("T");
            leg->mdepdate = depleg.at(0);
            leg->mdeprtdate = leg->mdepdate; //TODO
            QString deplegtime = depleg.at(1);
            deplegtime.chop(3);
            leg->mdeptime = deplegtime;
            leg->mdeprttime = leg->mdeptime;

            qDebug() << "LegDepTime" << leg->mdeptime;

            skiptostartof(&xml, "ArrDateTime");
            qDebug() << xml.name() << xml.isEndElement();

            QStringList arivleg = xml.readElementText().split("T");
            leg->marivdate = arivleg.at(0);
            leg->marivrtdate = leg->marivdate; //TODO RT
            QString arivlegtime = arivleg.at(1);
            arivlegtime.chop(3);
            leg->marivtime = arivlegtime;
            leg->marivrttime = leg->marivtime;

            qDebug() << "LegArrTime" << leg->mdeptime;

            skiptostartof(&xml, "From");
            skiptostartof(&xml, "Id");
            qDebug() << xml.name() << xml.isEndElement();
            leg->mfromid = xml.readElementText();
            skiptostartof(&xml, "Name");
            qDebug() << xml.name() << xml.isEndElement();
            leg->mfrom = xml.readElementText();

            skiptostartof(&xml, "To");
            skiptostartof(&xml, "Id");
            qDebug() << xml.name() << xml.isEndElement();
            leg->mtoid = xml.readElementText();
            skiptostartof(&xml, "Name");
            qDebug() << xml.name() << xml.isEndElement();
            leg->mto = xml.readElementText();

            skiptostartof(&xml, "Line");
            skiptostartof(&xml, "Name");
            qDebug() << xml.name() << xml.isEndElement();
            QString line = xml.readElementText();

            skiptostartof(&xml, "Towards");
            qDebug() << xml.name() << xml.isEndElement();
            leg->mdir = xml.readElementText();

            qDebug() << line << (line == "PågaTåg");
            if ((line == "Pågatåg") || (line == "Öresundståg")) {
                qDebug() << "TRAIN!!";
                leg->mdir.append("; ").append(line.toLower());
                leg->mline = tr("train");
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#000000";
            } else {
                leg->mline = line;
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
            }

            leg->calculatetimes();
            trip->addleg(leg);

            skiptoendof(&xml, "RouteLink");
            qDebug() << xml.name() << xml.isEndElement();
        }




        //end while2

        if (Timehelper::beforenow(trip->deprtdate, trip->deprttime)) {
            trip->passed = true;
            trip->errmsg = tr("Departed");
        } else {
            trip->passed = false;
        }


        bool canceled = false; //TODO
        bool risktomiss = false; //TODO
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

        skiptoendof(&xml, "Journey");
        qDebug() << xml.name() << xml.isEndElement();
    }


    //end while

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

    xml.readNextStartElement(); //Body
    xml.readNextStartElement(); //Getstartendpointresponse
    xml.readNextStartElement(); //Getstartendpointresult
    xml.readNextStartElement(); //Code
    xml.skipCurrentElement(); //Code end
    xml.readNextStartElement(); //Message
    xml.skipCurrentElement(); //Message end
    xml.readNextStartElement(); //Startpoints
    xml.readNextStartElement(); //Point
    xml.readNextStartElement(); //Id


    int count = 0;
    while(!xml.isEndElement() && count < 10) {
        QString id =  xml.readElementText();
        xml.readNextStartElement(); //Name
        QString name = xml.readElementText();

        stops->append(name + "#" + id);

        xml.skipCurrentElement(); //Point end
        xml.readNextStartElement(); //Point
        xml.readNextStartElement(); //Id
        count++;
    }
    emit stopsready("");
    sender()->deleteLater();
}
