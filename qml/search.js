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

.import "vasttrafik.js" as Vasttrafikjs
.import "parsehelper.js" as ParseHelpjs
.import "time.js" as Timejs

var http
var triparray = [];
//function gettrip(x) {
//    return triparray[x];
//}

function sendnamerequest(name, func) {
    var namequery = Vasttrafikjs.namequery
    var

    name = ParseHelpjs.removespecials(name);
    http = new XMLHttpRequest();
    http.onreadystatechange = function() {
        if (http.readyState === XMLHttpRequest.DONE) {
            var resp = (http.responseText);
            return func(Vasttrafikjs.parselist(resp));
        }
    }
    console.log(namequery + name)
    http.open("GET",namequery + name,true);
    http.send();
}

function sendrequest(fromstop, tostop, date, time, func, model, changetime) {

    console.log("CHANGETIME::" +changetime)
    var formateddate = Timejs.convertdate(date);
    var formatedtime = time.substring(0,2) + ":" + time.substring(3,5);
    console.log("Searching at: " + formateddate + " " + formatedtime);

    console.log("\n" + Vasttrafikjs.buildrequest(fromstop, tostop, formateddate, formatedtime, changetime) + "\n");

    getPage(Vasttrafikjs.buildrequest(fromstop, tostop, date, time, changetime), func, model)

}

function setuplist(answer, model) {
    for(var x=0; x< answer.length; x++) {
        console.log("First lap" + answer.length + "::" + answer[x])
        var leg = answer[x].leglist.head;
        if (leg === null) {
            console.log("No answer from server...")
            return;
        }

        var tripdeptime = leg.data.deptime;
        var tripdeprttime = (leg.data.deprttime === null) ? leg.data.deptime: leg.data.deprttime;
        var tripdepdate = leg.data.depdate;
        var tripdeprtdate = (leg.data.deprtdate === null) ? leg.data.depdate: leg.data.deprtdate;

        if (leg.data.arivrttime === undefined) {
            console.log("hello!!" + leg.data.arivrttime)
        }

        var exchanges = [];
        var i = 0;
        var d;
        while (leg !==null) {
            d = leg.data;
            exchanges[i] = d;
            i++;
            leg = leg.next;
        }
        //console.log("DERP " + d.name + d.origname + d.arivrttime)
        if( d.arivrttime === undefined) {
            console.log("42");
        }

        mainWindow.lastresponseexchanges[x] = exchanges
        //triparray[x] = exchanges;
        model.append({deptime: tripdeptime,
                      arivtime: d.arivtime,
                      depdate: tripdepdate,
                      arivdate: d.arivdate,
                      deprttime: tripdeprttime,
                      arivrttime: ((d.arivrttime === undefined) || (d.arivrttime === null)) ? d.arivtime : d.arivrttime,
                      deprtdate: tripdeprtdate,
                      arivrtdate: ((d.arivrtdate === undefined) || (d.arivrtdate === null) ) ? d.arivdate : d.arivrtdate ,
                      exchready: true})
    }
}

function getPage(url, functionToCallWhenDone, model) {
    http = new XMLHttpRequest();
    http.onreadystatechange = function() {
        if (http.readyState === XMLHttpRequest.DONE) {
            var resp = (http.responseText);
            var parsed = Vasttrafikjs.parse(resp);
            if (parsed.indexOf("ERROR") === 0) {
                return functionToCallWhenDone(parsed.substring(5));
            }
            setuplist(parsed, model);
            mainWindow.lastparsedtrips = parsed;
            return functionToCallWhenDone(0);
        }
    }
    http.open("GET",url,true);
    http.send();
    return 0;
}

function addicons(iconmodel, index) {
    //console.log("EXCH ÄNDRAD "+ index + " exec: " + gettrip(index)[0].name + " :" + gettrip(index).length);
    //var exchs = gettrip(index);
    var exchs = mainWindow.lastresponseexchanges[index]
    ///
    if (exchs === undefined) {
        console.log("UNDEFINED!!!!!")
        return;
    }

    for(var i=0; i<exchs.length; i++) {
        var exch = exchs[i];
        var legname;
        if (exch.name === "Gå") {
            iconmodel.append({name: "walk", color1: "#ffffff", color2: "#00abe5", dir: "Walk",
                                 origname: exch.origname.split(",")[0], destname: exch.destname.split(",")[0],
                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivdate});
            continue;
        } else if (exch.name.split(" ")[0] === "SJ") {
            iconmodel.append({name: "sj", color1: "#000000", color2: "#ffffff", dir: exch.dir,
                                 origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
                                 destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivrtdate});
            continue;
        } else if ((exch.name === "PENDELTÅG") || (exch.name === "VÄSTTÅGEN") || (exch.name === "TÅGAB REGIONTÅG")) {
            iconmodel.append({name: "train", color1: "#ffffff", color2: "#000000", dir: exch.dir,
                                 origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
                                 destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
                                 deptime: exch.deptime, depdate: exch.depdate, arivtime: exch.arivtime, arivdate: exch.arivdate,
                                 deprttime: exch.deptime, deprtdate: exch.depdate, arivrttime: exch.arivtime, arivrtdate: exch.arivrtdate});
            continue;
        } else if (exch.name.split(" ")[1] === "EXPRESS") {
            legname = exch.name.split(" ")[0].toString().toLowerCase();
        } else if ((exch.name.split(" ")[1] === "ÄLVSNABBEN") || (exch.name.split(" ")[1] === "ÄLVSNABBARE")) {
            legname = "älvs."
        } else {
            legname = exch.sname;
        }
        iconmodel.append({name: legname, color1: exch.color1, color2: exch.color2, dir: exch.dir,
                             origname: exch.origname.split(",")[0], origtrack: exch.origtrack,
                             destname: exch.destname.split(",")[0], desttrack: exch.desttrack,
                             deptime: exch.deptime, depdate: exch.depdate, deprttime: exch.deprttime, deprtdate: exch.deprtdate,
                             arivtime: exch.arivtime, arivdate: exch.arivdate, arivrttime: exch.arivrttime, arivrtdate: exch.arivrtdate});
    }
}
