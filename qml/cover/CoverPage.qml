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


import QtQuick 2.0
import Sailfish.Silica 1.0

import searcher 1.0
import timehelp 1.0

CoverBackground {
    id: coverpage
    property string coverstatus: "started"
    property bool avail: mainWindow.avail
    property bool error: false
    property string from: mainWindow.from
    property string to: mainWindow.to
    property int searched: mainWindow.searched
    property string lang: mainWindow.lang
    property string err: mainWindow.errmsg
    //onErrChanged: search(false)
    onLangChanged: {
        if (coverstatus === "error") {
            placeholdertext.text = mainWindow.strappname + "\n" + mainWindow.strcovererr;
        }
    }


    function refresh(now) {
        console.log("REFRESHING COVER " + coverstatus)
        listmodel.clear()
        if (coverstatus === "error") {
            console.log("Coverpage in errorstate");
            placeholdertext.text = mainWindow.strappname + "\n" + mainWindow.strcovererr;
            return;
        }
        console.log("Refreshing Cover")
        var time = searcher.gettime()
        var date = searcher.getdate()
        if (now) {
            time = timehelp.getcurrenttimestr();
            date = timehelp.getcurrentdatestr();
        }
        searcher.search(searcher.getfromid(),searcher.gettoid(),date,time)
        searcher.setdateofsearch(timehelp.getcurrentdatestr())
        searcher.settimeofsearch(timehelp.getcurrenttimestr())
        coverstatus = "searching"
    }


    Search {
        id: searcher
        onReady: {
            console.log("Ready signal received in CoverPage")
            listmodel.clear();
            if (err === "") {
                //No error
                var tripindex = 0
                var trip
                while((trip = searcher.getTrip(tripindex))!==null) {
                    listmodel.append({  deptime: trip.getdeptime(),
                                         arivtime: trip.getarivtime(),
                                         depdate: trip.getdepdate(),
                                         arivdate: trip.getarivdate(),
                                         deprttime: trip.getdeprttime(),
                                         arivrttime: trip.getarivrttime(),
                                         deprtdate: trip.getdeprtdate(),
                                         arivrtdate: trip.getarivrtdate(),
                                         exchready: true})
                    tripindex++
                }
                from.text = searcher.getfrom()
                to.text = searcher.getto()
                coverstatus = "avail"
            } else {
                coverstatus = "error"
                placeholdertext.text = mainWindow.strappname + "\n" + mainWindow.strcovererr;
            }
        }
        //TODO: TEST
        onSearching: {
            console.log("CoverPage, onSearchnig")
            listmodel.clear()
        }
    }
    Timehelp {
        id: timehelp
    }


    Column {
        anchors.centerIn: parent
        width: parent.width
        Item {
            height: coverpage.height/3
            width: parent.width
            opacity: coverstatus === "error" || coverstatus === "started" ? 1 : 0.3

            Image {
                id: logo
                width: 86
                height: 86
                anchors.centerIn: parent
                source: "/usr/share/icons/hicolor/86x86/apps/harbour-travelplanner.png"
            }
        }
        Label {
            id: placeholdertext
            text: mainWindow.strappname
            visible: coverstatus === "error" || coverstatus === "started"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Column {
        anchors.fill: parent
        Label {id: from; x: Theme.paddingSmall; visible: (coverstatus === "avail"); font.pixelSize: Theme.fontSizeTiny; width: parent.width; truncationMode: TruncationMode.Elide; color: Theme.secondaryHighlightColor}
        Label {id: to; x: Theme.paddingSmall; visible: (coverstatus === "avail"); font.pixelSize: Theme.fontSizeTiny; width: parent.width; truncationMode: TruncationMode.Elide; color: Theme.highlightColor}

        ListView {
            id: triplist
            width: parent.width
            height: parent.height - from.height - to.height
            clip: true

            property bool searching: false


            //TODO: fix double text error
            function doneloading(msg) {
                searching = false;
                if(msg === 0) {
                } else {
                    error = true;
                    placeholdertext.text = mainWindow.errmsg;
                }


            }

            BusyIndicator {
                id: busy
                visible: coverstatus === "searching"
                running: triplist.searching
                width: parent.width / 3
                height: width
                anchors.horizontalCenter:  parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                size: BusyIndicatorSize.Large
            }

            model: ListModel {
                id: listmodel
            }

            delegate: BackgroundItem {
                id: delegate
                z: 1
                height: triplist.height / 3
                width: triplist.width

                Component.onCompleted: {
                    var legnr = 0
                    var leg
                    while((leg = searcher.getLeg(index, legnr)) !== null) {
                        iconmodel.append({name: leg.line, fgcolour: leg.fgcolour, bgcolour: leg.bgcolour, dir: leg.dir,
                                             fromname: leg.from.split(",")[0], fromtrack: leg.fromtrack,
                                             destname: leg.to.split(",")[0], totrack: leg.totrack,
                                             deptime: leg.deptime, depdate: leg.depdate, deprttime: leg.deprttime, deprtdate: leg.deprtdate,
                                             arivtime: leg.arivtime, arivdate: leg.arivdate, arivrttime: leg.arivrttime, arivrtdate: leg.arivrtdate})
                        legnr++;
                    }
                }

                Column {
                    width: parent.width
                    Row {
                        height: triplist.height / 6
                        Label {
                            id: deptimelabel
                            text: deptime + timehelp.delay(depdate, deptime, deprtdate, deprttime)
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: arivtimelabel
                            text: arivtime + timehelp.delay(arivdate, arivtime, arivrtdate, arivrttime)
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: durlabel
                            text: timehelp.duration(deprtdate, deprttime, arivrtdate, arivrttime)
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                    }
                    ListView {
                        id: iconlist
                        height: triplist.height / 6
                        width: coverpage.width
                        orientation: ListView.Horizontal

                        spacing: Theme.paddingSmall
                        property int recsize0: height//(iconlist.width - 6*iconlist.spacing) / 7

                        model: ListModel {
                            id: iconmodel
                        }

                        delegate : Component {
                            id: icondelegate
                            Row {
                                Rectangle {
                                    id: outerrect
                                    width: iconlist.recsize0
                                    height: iconlist.recsize0
                                    color: "#00000000"
                                    Rectangle {
                                        id: rect
                                        width: iconlist.recsize0
                                        height: width
                                        anchors.centerIn: parent
                                        color: fgcolour
                                        opacity: 1
                                        Label {
                                            id: recttext
                                            text: name
                                            color: bgcolour
                                            anchors.centerIn: parent
                                            font.pixelSize: Theme.fontSizeTiny
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    CoverActionList {
        id: coverAction
        enabled: (coverstatus === "avail")
        CoverAction {
            //Update the search for the current time
            iconSource: "image://theme/icon-cover-timer"
            onTriggered: coverpage.refresh(true);
        }
        CoverAction {
            //Update the search for the original or last used time
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: coverpage.refresh(false);
        }
    }
}


