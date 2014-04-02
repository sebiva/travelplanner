#include "parser.h"

Parser *Parser::mparser = 0;
QList<Trip *> *Parser::trips = new QList<Trip *>();

Parser::Parser(QObject *parent) :
    QObject(parent)
{
    address = (QString) "http://api.vasttrafik.se/bin/rest.exe/" +
            "v1/trip?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml";
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

Parser::~Parser() {
    qDebug() << "Deleting parser!";
}

int Parser::numtrips() {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return -1;
    }
    return trips->length();
}

int Parser::numlegs(int tripindex) {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return -1;
    }

    if (tripindex >= trips->length()) {
        return -1;
    } else {
        return trips->at(tripindex)->leglist->length();
    }
}

Trip * Parser::getTrip(int index) {


    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return NULL;
    }
    if (index >= trips->length()) {
        return NULL;
    }
    return trips->at(index);
}

Leg * Parser::getLeg(int tripindex, int legindex) {
    if (trips == NULL) {
        qDebug() << "Trips NULL";
        return NULL;
    }
    if (tripindex >= trips->length() || legindex >= trips->at(tripindex)->leglist->length()) {
        return NULL;
    }
    return getTrip(tripindex)->leglist->at(legindex);
}

//TODO: TEST
void Parser::cleartrips() {
    for (int i = 0; i < trips->length(); i++) {
        delete trips->at(i);

    }
    trips->clear();
}

bool Parser::getXML(QString fromid, QString toid,  QString date, QString time) {
    qDebug() << "SEARCHING::" << date << time << fromid << toid;
    QNetworkAccessManager * manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(XMLready(QNetworkReply*)) );
    manager->get(QNetworkRequest(QUrl(address +
                                      "&date=" + date +
                                      "&time=" + time +
                                      "&originId=" + fromid +
                                      "&destId=" + toid)));
    qDebug()<<"ADDRESS::"<<(address+"&date="+date +"&time="+time+"&originId="+fromid+"&destId="+toid);
    return true;
}

void Parser::XMLready( QNetworkReply * reply){
    //TODO: TEST this
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "An error occured";
        emit ready("No connection");
        return;
    }

    parsevasttrafikreply(reply);
}

void Parser::parsevasttrafikreply(QNetworkReply *reply) {
    qDebug() << "Parsing";
    QXmlStreamReader xml;
    xml.setDevice(reply);
    QXmlStreamAttributes attr;

    trips->clear();
    xml.readNextStartElement(); //Triplist
    QString error = xml.attributes().value("error").toString();
    if (error != "") {
        qDebug() << "Error!!!" << xml.attributes().value("errorText").toString();
        emit ready(xml.attributes().value("errorText").toString());
        return;
    }
    xml.readNextStartElement(); //Trip
    while(!xml.isEndElement()) {
        Trip *trip = new Trip();
        trip->setParent(this);
        //Keep track of first leg for each trip
        bool first = true;
        xml.readNextStartElement();//Leg
        Leg *leg = NULL;
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


            //TODO: translation
            if (leg->mline == "Gå") {
                leg->mline = "walk";
                leg->mfgcolour = "#00abe5";
                leg->mbgcolour = "#ffffff";
                leg->mdir = "Walk";
            } else if (leg->mline.split(" ").at(0) == "SJ") {
                leg->mline = "sj";
                leg->mfgcolour = "#ffffff";
                leg->mbgcolour = "#000000";
            } else if (leg->mline == "PENDELTÅG" || leg->mline == "VÄSTTÅGEN" || leg->mline == "TÅGAB REGIONTÅG") {
                leg->mline = "train";
                leg->mfgcolour = "#000000";
                leg->mbgcolour = "#ffffff";
            } else if (leg->mline.split(" ").at(1) == "EXPRESS") {
                leg->mline = (leg->mline.split(" ").at(0)).toLower();
            } else if (leg->mline.split(" ").at(1) == "ÄLVSNABBEN" || leg->mline.split(" ").at(1) == "ÄLVSNABBARE" ) {
                leg->mline = "älvs.";
            } else {
                leg->mline = sname;
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
            if (leg->mline == "walk") { //TODO: translation

                //Go to next leg (or trip)
                xml.skipCurrentElement();
                //qDebug() << "WALK1" << xml.name();
                xml.readNextStartElement();
                //qDebug() << "WALK1" << xml.name();
                xml.readNextStartElement();
                //qDebug() << "WALK1" << xml.name();

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
        Trip *tripadd = trip;
        trips->append(tripadd);
    }
    qDebug() << "Parsing done, no errors!";
    emit ready("");
}

