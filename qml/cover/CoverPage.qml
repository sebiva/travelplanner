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

import timehelp 1.0

CoverBackground {
    id: coverpage
    /*
      Keeps track of the status of the cover. Can be either in:
      "started"; the app was just started, cover showing the logo
      "error"; an error has occured, and a message is shown
      "searching"; waiting for a reply
      "avail"; a result is displayed
      */
    property string coverstatus: "started"
    property string backend: mainWindow.backend

    /*
      Updates the cover, now specifying whether to use the original search time or
      the current time to search. Updates the internal search object, thus making
      this new search visible to the SearchPage as well.
      */
    function refresh(now) {
        console.log("REFRESHING COVER " + coverstatus)
        listmodel.clear()
        if (coverstatus === "error") {
            console.log("Coverpage in errorstate");
            placeholdertext.text = qsTr("Travelplanner") + "\n" + qsTr("Search failed")
        }
        console.log("Refreshing Cover")
        var time = searchx.gettime()
        var date = searchx.getdate()
        if (now) {
            time = timehelp.getcurrenttimestr();
            date = timehelp.getcurrentdatestr();
        }
        searchx.search(searchx.getfromid(),searchx.gettoid(),date,time)
        searchx.setdateofsearch(timehelp.getcurrentdatestr())
        searchx.settimeofsearch(timehelp.getcurrenttimestr())
        coverstatus = "searching"
    }

    onBackendChanged: {
        console.log("Database changed according to coverpage")

        //Clear the search
        listmodel.clear()
        coverstatus = "started"

    }

    Component.onDestruction: {
        console.log("Destroying Coverpage")
        conn.destroy()
    }

    /*
      Used to perform searches, and intercept signals.
      */
    Connections {
        id: conn
        target: searchx

        /*
          Called when a search is ready in the Search class.
          */
        onReady: {
            console.log("Ready signal received in CoverPage")
            listmodel.clear()
            if (err === "") {
                console.log("No error in search")
                //No error
                var tripindex = 0
                var trip
                // Add the result to the list (only add the first 3, as the others won't be visible
                while((trip = searchx.getTrip(tripindex))!==null && tripindex < 3) {
                    listmodel.append({  deptime: trip.getdeptime(),
                                         arivtime: trip.getarivtime(),
                                         depdate: trip.getdepdate(),
                                         arivdate: trip.getarivdate(),
                                         deprttime: trip.getdeprttime(),
                                         arivrttime: trip.getarivrttime(),
                                         deprtdate: trip.getdeprtdate(),
                                         arivrtdate: trip.getarivrtdate(),
                                         depdelay: trip.getdepdelay(),
                                         arivdelay: trip.getarivdelay(),
                                         duration: trip.getduration()  })
                    tripindex++
                }
                from.text = searchx.getfrom()
                to.text = searchx.getto()
                coverstatus = "avail"

            } else {
                console.log("Error in search")
                coverstatus = "error"
                placeholdertext.text = qsTr("Travelplanner") + "\n" + qsTr("Search failed")
            }
            console.log("onReady done")
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
            text: qsTr("Travelplanner")
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

            BusyIndicator {
                id: busy
                visible: coverstatus === "searching"
                running: coverstatus === "searching"
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

                /*
                  Sets up the icons for the trip, using data from the Search object.
                  */
                Component.onCompleted: {
                    console.log("In oncompleted, delegate")
                    var legnr = 0
                    var leg
                    while((leg = searchx.getLeg(index, legnr)) !== null) {
                        iconmodel.append({name: leg.line, fgcolour: leg.fgcolour, bgcolour: leg.bgcolour, dir: leg.dir,
                                             fromname: leg.from.split(",")[0], fromtrack: leg.fromtrack,
                                             destname: leg.to.split(",")[0], totrack: leg.totrack,
                                             deptime: leg.deptime, depdate: leg.depdate, deprttime: leg.deprttime, deprtdate: leg.deprtdate,
                                             arivtime: leg.arivtime, arivdate: leg.arivdate, arivrttime: leg.arivrttime, arivrtdate: leg.arivrtdate})
                        legnr++;
                    }
                    console.log("oncompleted done")
                }

                Column {
                    width: parent.width
                    Row {
                        height: triplist.height / 6
                        Label {
                            id: deptimelabel
                            text: deptime + depdelay
                            truncationMode: TruncationMode.Fade
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: arivtimelabel
                            text: arivtime + arivdelay
                            truncationMode: TruncationMode.Fade
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: durlabel
                            text: duration
                            truncationMode: TruncationMode.Fade
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
                        property int recsize0: height

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


