/*
    Copyright (C) 2014 Sebastian Ivarsson
    Contact: Sebastian Ivarsson <sebiva@student.chalmers.se>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

Qt.include("time.js")
Qt.include("parsehelper.js")
//. import "time.js" as Timejs;
//var http;

var key = "authKey=924b3c93-d187-47ab-bfde-12c230a7e97b"
var baseadress = "http://api.vasttrafik.se/bin/rest.exe/v1/trip?"
var format = "&format=xml"
var addchgtime = "&disregardDefaultChangeMargin=1&additionalChangeTime="
var namequery = "http://api.vasttrafik.se/bin/rest.exe/v1/location.name?authKey=924b3c93-d187-47ab-bfde-12c230a7e97b&format=xml&input="

//var triparray = [];
//function gettrip(x) {
//    return triparray[x];
//}

function buildrequest(fromstop, tostop, formateddate, formatedtime, changetime) {
    return baseadress + key + format + "&date=" + formateddate + "&time=" + formatedtime + "&originId=" + fromstop + "&destId=" + tostop; // + addchgtime + changetime;
}


//function sendrequest(fromstop, tostop, date, time, func, model, changetime) {

//    console.log("CHANGETIME::" +changetime)
//    var formateddate = convertdate(date);
//    var formatedtime = time.substring(0,2) + ":" + time.substring(3,5);
//    console.log("Searching at: " + formateddate + " " + formatedtime);

//    console.log("\n" + buildrequest(fromstop, tostop, date, time, changetime) + "\n");

//    getPage(buildrequest(fromstop, tostop, date, time, changetime), func, model)

//}


//function sendnamerequest(name, func) {
//    name = removespecials(name);
//    http = new XMLHttpRequest();
//    http.onreadystatechange = function() {
//        if (http.readyState == XMLHttpRequest.DONE) {
//            var resp = (http.responseText);
//            return func(parselist(resp));
//        }
//    }
//    console.log(namequery + name)
//    http.open("GET",namequery + name,true);
//    http.send();
//}




function parselist(answer) {
    var xmlpos = getTag(answer, "<?", "?>");
    answer = answer.substring(xmlpos[1]+2);
    var locationlistpos = getTag(answer, "<LocationList", ">");
    answer = answer.substring(locationlistpos[1] +1);

    var stops = [];

    for(var i=0;i<10;i++) {
        var stoppos = getTag(answer, "<StopLocation", "/>");
        if (stoppos === null) {
            stoppos = getTag(answer, "<CoordLocation", "/>");
            if (stoppos === null) {
                return stops;
            }
            answer = answer.substring(stoppos[1]+2);
            continue;
        }

        var stopstr = answer.substring(stoppos[0], stoppos[1]);
        answer = answer.substring(stoppos[1]+2);

        var stop = {
            name: null, id: null
        }
        stop.name = getAttr(stopstr, "name");
        if (stop.name.indexOf(".") === 0) {
            continue;
        }

        stop.id = getAttr(stopstr, "id");

        stops[i] = stop;

    }
    return stops;
}

//function addicons(iconmodel, index) {
//    //console.log("EXCH ÄNDRAD "+ index + " exec: " + gettrip(index)[0].name + " :" + gettrip(index).length);
//    var exchs = gettrip(index);

//    ///
//    if (exchs === undefined) {
//        console.log("UNDEFINED!!!!!")
//        return;
//    }

//    for(var i=0; i<exchs.length; i++) {
//        var exch = exchs[i];
//        var legname;
//        if (exch.name === "Gå") {
//            iconmodel.append({name: "walk", color1: "#ffffff", color2: "#00abe5", dir: "Walk",
//                                 origname: exch.origname.split(",")[0], destname: exch.destname.split(",")[0],
//                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
//                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivdate});
//            continue;
//        } else if (exch.name.split(" ")[0] === "SJ") {
//            iconmodel.append({name: "sj", color1: "#000000", color2: "#ffffff", dir: exch.dir,
//                                 origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
//                                 destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
//                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
//                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivrtdate});
//            continue;
//        } else if ((exch.name === "PENDELTÅG") || (exch.name === "VÄSTTÅGEN") || (exch.name === "TÅGAB REGIONTÅG")) {
//            iconmodel.append({name: "train", color1: "#ffffff", color2: "#000000", dir: exch.dir,
//                                 origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
//                                 destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
//                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
//                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivrtdate});
//            continue;
//        } else if (exch.name.split(" ")[1] === "EXPRESS") {
//            legname = exch.name.split(" ")[0].toString().toLowerCase();
//        } else if ((exch.name.split(" ")[1] === "ÄLVSNABBEN") || (exch.name.split(" ")[1] === "ÄLVSNABBARE")) {
//            legname = "älvs."
//        } else {
//            legname = exch.sname;
//        }
//        iconmodel.append({name: legname, color1: exch.color1, color2: exch.color2, dir: exch.dir,
//                             origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
//                             destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
//                             deptime: exch.deptime, depdate: exch.depdate, deprttime: exch.deprttime, deprtdate: exch.deprtdate,
//                             arivtime: exch.arivtime, arivdate: exch.arivdate, arivrttime: exch.arivrttime, arivrtdate: exch.arivrtdate});
//    }
//}

//function getPage(url, functionToCallWhenDone, model) {
//    http = new XMLHttpRequest();
//    http.onreadystatechange = function() {
//        if (http.readyState == XMLHttpRequest.DONE) {
//            var resp = (http.responseText);
//            var parsed = parse(resp);
//            if (parsed.indexOf("ERROR") === 0) {
//                return functionToCallWhenDone(parsed.substring(5));
//            }
//            setuplist(parsed, model);
//            mainWindow.response = resp;
//            return functionToCallWhenDone(0);
//        }
//    }
//    http.open("GET",url,true);
//    http.send();
//    return 0;
//}

function parse(response) {
    console.log(response);
    var xmlpos = getTag(response, "<?", "?>");
    if (response == null || response == ""|| xmlpos == null) {
        //Error, no response
        return "ERRORNo response";
    }
    response = response.substring(xmlpos[1]+2);
    var err = getAttr(response.substring(0,400), "errorText");
    if (err != null) {
        //Error, No connections found
        return "ERROR" + err;
    }

    var triplistpos = getTag(response, "<TripList", ">");

    console.log("TRIPLIST: " + response.substring(triplistpos[0], triplistpos[1]));
    response = response.substring(triplistpos[1]+1);

    var alltrips = [];

    console.log("CLEARING triplist!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

    for(var i=0; i<10; i++) {
        console.log("READING NEW TRIP "+ i + " __________________________________________________________")

        var trippos = getTag(response, "<Trip", "</Trip>");
        if (trippos === null) {
            console.log(i + "Trips found")
            break;
        }

        var trip = response.substring(trippos[0], trippos[1]);
        response = response.substring(trippos[1] + 7);

        //console.log("TRIP: "+  trip);

        var currenttrip = {
            leglist: null
        }
        alltrips[i] = currenttrip;
        mainWindow.lastresponseexchanges[i] = currenttrip;


        var leglist = {
            head: null,
            tail: null
        }
        currenttrip.leglist = leglist;

        //Read all the legs of the trip
        var legpos;
        while ( (legpos = getTag(trip, "<Leg", ">"))!== null) {
            console.log("READING NEW LEG __________________________________________________________")
            var leginfo = {
                name: null,
                sname: null,
                dir: null,
                color1: null,
                color2: null,
                origname: null,
                deptime: null,
                depdate: null,
                origtrack: null,
                deprttime: null,
                deprtdate: null,
                destname: null,
                arivtime: null,
                arivdate: null,
                desttrack: null,
                arivrttime: null,
                arivrtdate: null
            }

            var leg = trip.substring(legpos[0], legpos[1]);

            //console.log("LEGTEXT: " + leg);
            //Leginfo
            leginfo.name = getAttr(leg, "name");
            leginfo.sname = getAttr(leg, "sname");
            leginfo.dir = getAttr(leg, "direction");
            leginfo.color1 = getAttr(leg, "fgColor");
            leginfo.color2 = getAttr(leg, "bgColor");

            trip = trip.substring(legpos[1]+1);
            var originpos = getTag(trip, "<Origin", ">");
            var origin = trip.substring(originpos[0], originpos[1]);

            //console.log(origin)

            //Origininfo
            leginfo.origname = getAttr(origin, "name");
            leginfo.deptime = getAttr(origin, "time");
            leginfo.depdate = getAttr(origin, "date");
            leginfo.origtrack = getAttr(origin, "track");
            if(leginfo.origtrack === null) {
                leginfo.origtrack = "";
            }

            leginfo.deprttime = getAttr(origin, "rtTime");
            if(leginfo.deprttime === null) {
                leginfo.deprttime = leginfo.deptime;
            }
            leginfo.deprtdate = getAttr(origin, "rtDate");
            if(leginfo.deprtdate === null) {
                leginfo.deprtdate = leginfo.depdate;
            }
            //console.log("42" + leginfo.deprtdate + leginfo.deprttime);

            trip = trip.substring(originpos[1]+10);
            var destpos = getTag(trip, "<Destination", ">");
            var dest = trip.substring(destpos[0], destpos[1]);

            //console.log(dest)

            //Destinationinfo
            leginfo.destname = getAttr(dest, "name");
            leginfo.arivtime = getAttr(dest, "time");
            leginfo.arivdate = getAttr(dest, "date");
            leginfo.desttrack = getAttr(dest, "track")
            if(leginfo.desttrack === null) {
                leginfo.desttrack = "";
            }

            leginfo.arivrttime = getAttr(dest, "rtTime");
            if(leginfo.arivrttime === null) {
                leginfo.arivrttime = leginfo.arivtime;
            }

            leginfo.arivrtdate = getAttr(dest, "rtDate");
            if(leginfo.arivrtdate === null) {
                leginfo.arivrtdate = leginfo.arivdate;
            }

            //console.log("42" + leginfo.arivrtdate + leginfo.arivrttime)

            trip = trip.substring(destpos[1]+15)

            //Skip unneccessary walk stuff
            if (leginfo.origname === leginfo.destname) {
                console.log("WALKING TO SAME STOP IGNORED: " + leginfo.origname);
                continue;
            }


            var leglistelem = {
                data: leginfo,
                next: null
            }
            if (leglist.head === null) {
                leglist.head = leglistelem;
            } else {
                leglist.tail.next = leglistelem;
            }
            leglist.tail = leglistelem;
            console.log("RESULT LEG: " + leginfo.name + leginfo.destname + leginfo.arivtime)
        }

    }
    //mainWindow.lastresponseexchanges = alltrips;
    return alltrips;
}

//function setuplist(answer, model) {
//    for(var x=0; x< answer.length; x++) {
//        var leg = answer[x].leglist.head;
//        if (leg === null) {
//            console.log("No answer from server...")
//            return;
//        }

//        var tripdeptime = leg.data.deptime;
//        var tripdeprttime = (leg.data.deprttime === null) ? leg.data.deptime: leg.data.deprttime;
//        var tripdepdate = leg.data.depdate;
//        var tripdeprtdate = (leg.data.deprtdate === null) ? leg.data.depdate: leg.data.deprtdate;

//        if (leg.data.arivrttime === undefined) {
//            console.log("hello!!" + leg.data.arivrttime)
//        }

//        var exchanges = [];
//        var i = 0;
//        var d;
//        while (leg !==null) {
//            d = leg.data;
//            exchanges[i] = d;
//            i++;
//            leg = leg.next;
//        }
//        //console.log("DERP " + d.name + d.origname + d.arivrttime)
//        if( d.arivrttime === undefined) {
//            console.log("42");
//        }

//        triparray[x] = exchanges;
//        model.append({deptime: tripdeptime,
//                      arivtime: d.arivtime,
//                      depdate: tripdepdate,
//                      arivdate: d.arivdate,
//                      deprttime: tripdeprttime,
//                      arivrttime: ((d.arivrttime === undefined) || (d.arivrttime === null)) ? d.arivtime : d.arivrttime,
//                      deprtdate: tripdeprtdate,
//                      arivrtdate: ((d.arivrtdate === undefined) || (d.arivrtdate === null) ) ? d.arivdate : d.arivrtdate ,
//                      exchready: true})
//    }
//}