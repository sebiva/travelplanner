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

import "../database.js" as DBjs

Page {
    id: searchpage

    property bool searching : true
    property bool error : false

    property bool active : Qt.application.active
    property bool coversearch : false
    property string searcherr: ""

    onStatusChanged: {
        if (status === PageStatus.Active) {
            console.log("SEARCHING from SearchPage")
            listmodel.clear()
            searchx.search()
            searching=true
            searchx.setdateofsearch(timehelp.getcurrentdatestr())
            searchx.settimeofsearch(timehelp.getcurrenttimestr())
            fromlabel.text = qsTr("From") + ": " + searchx.getfrom()
            tolabel.text = qsTr("To") + ": " + searchx.getto()
            datelabel.text = searchx.getdate()
            timelabel.text = searchx.gettime()
        }
    }

    onActiveChanged: {
        console.log("Searchpage in focus?" + active)
        if (active && coversearch) {
            console.log("Searchpage refreshing bc of coversearch")
            coversearch = false
            replyready(searcherr) //TODO: test
        }
    }

    Component.onDestruction: {
        console.log("Destroying SearchPage")
        conn.destroy()
    }



    Connections {
        id: conn
        target: searchx
        onReady: {
            if (!active) {
                //If the Searchpage isn't active, wait to search until it becomes active
                console.log("Searchpage: Coversearch detected")
                coversearch = true
                searcherr = err
            } else {
               replyready(err)
            }
        }
        //TODO: TEST
        onSearching: {
            console.log("SearchPage, onSearching")
            searching = true
            listmodel.clear()
            fromlabel.text = qsTr("From") + ": " + searchx.getfrom()
            tolabel.text = qsTr("To") + ": " + searchx.getto()
            datelabel.text = searchx.getdate()
            timelabel.text = searchx.gettime()
        }
    }

//    function search() {
//        console.log("Searchpage, search()")
//        mainWindow.timeofsearch = timehelp.getcurrenttimestr()
//        mainWindow.dateofsearch = timehelp.getcurrentdatestr()
//        listmodel.clear()
//    }


    function replyready(err) {
        console.log("Ready signal received in SearchPage")
        if (coversearch) {
            searching = false
        }

        listmodel.clear()
        if (err === "") {
            //No error
            listView.setup()
            DBjs.setlastsearch(searchx.getfromid(), searchx.gettoid(),
                               searchx.getfrom(), searchx.getto())
            mainWindow.incDB()
            searchpage.searching = false
            searchpage.error = false
        } else {
            console.log("ERROR IN SEARCHPAGE: " + err);
            searchpage.searching = false;
            searchpage.error = true
            mainWindow.errmsg = qsTr("Search failed:") + "\n" + err; //TODO: formating on long messages
        }
        console.log("Searchpage, onReady done")
    }

    Column {
        anchors.fill: parent
        SilicaFlickable {
            id: flick
            height: column.height
            width: parent.width
            contentHeight: column.height
            PullDownMenu {
                MenuItem {
                    text: qsTr("Save as favourite")
                    onClicked: {
                        // Database not yet setup?
                        if (mainWindow.verDB == 0) {
                            return
                        }
                        DBjs.setfav(searchx.getfromid(), searchx.gettoid(), searchx.getfrom(), searchx.getto());
                        mainWindow.incDB()
                    }
                }
            }
            Column {
                id: column
                width: parent.width
                PageHeader {
                    id: head
                    title: qsTr("Search results")
                }
                Row {
                    id: row1
                    x: Theme.paddingSmall
                    width: parent.width - 2*x
                    Label {
                        id: fromlabel
                        text: qsTr("From") + ": " + searchx.getfrom()
                        truncationMode: TruncationMode.Elide
                        width: parent.width - timelabel.width
                        color: Theme.secondaryHighlightColor
                    }
                    Label {
                        id: timelabel
                        text: searchx.gettime()
                        horizontalAlignment: Text.AlignRight
                    }
                }

                Row {
                    id: row2
                    x: Theme.paddingSmall
                    width: parent.width - 2*x
                    Label {
                        id: tolabel
                        text: qsTr("To") + ": " + searchx.getto()
                        truncationMode: TruncationMode.Elide
                        width: parent.width - datelabel.width
                        color: Theme.highlightColor
                    }
                    Label {
                        id: datelabel
                        text: searchx.getdate()
                        horizontalAlignment: Text.AlignRight
                    }
                }
            }

        }

        SilicaListView {
            id: listView
            width: parent.width
            height: searchpage.height - flick.height
            contentHeight: searchpage.height/4 * listmodel.count
            clip: true

            cacheBuffer: searchpage.height * 2

            // The reply is available in Search
            function setup() {
                console.log("ListView: Setting up list")
                var tripindex = 0
                var trip
                allstates = []
                while((trip = searchx.getTrip(tripindex))!==null) {
                    allstates[tripindex] = "small"
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
            }

            property var allstates: []
            Label {
                id: placeholdertext
                visible: error
                anchors.centerIn: parent
                text: mainWindow.errmsg
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.highlightColor
            }

            BusyIndicator {
                id: busy
                visible: searchpage.searching
                running: searchpage.searching
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
                id: trip
                z: 1
                height: searchpage.height / 5
                width: listView.width
                highlighted: mouse.pressed || (state === "big")
                state: listView.allstates[index] === undefined ? "small" : listView.allstates[index]

                property int addanimdur: 200
                property int tripindex: index

                Component.onCompleted: {
                    iconlist.setupicons()
                }

                Column {
                    width: parent.width
                    Row {
                        height: searchpage.height / 10
                        Component.onCompleted: {
                            var depnumdays = timehelp.daysfromtoday(depdate,deptime)
                            var depnextday = depnumdays > 0 ? "+" + depnumdays : depnumdays
                            deptimelabel.text = deptime + depdelay + (depnumdays !== 0 ? " [" + depnextday + "d]" : "")
                            var numdays = timehelp.daysfromtoday(arivdate,arivtime)
                            var nextday = numdays > 0 ? "+" + numdays : numdays
                            arivtimelabel.text = arivtime + arivdelay + (numdays !== 0 ? " [" + nextday + "d]" : "")
                            durlabel.text = duration

                            iconlist.numlegs = searchx.getnumlegs(index)
                        }

                        Label {
                            id: deptimelabel
                            width: searchpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            color: trip.highlighted ? Theme.highlightColor : Theme.primaryColor
                        }
                        Label {
                            id: arivtimelabel
                            width: searchpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            color: trip.highlighted ? Theme.highlightColor : Theme.primaryColor
                        }
                        Label {
                            id: durlabel
                            width: searchpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            color: trip.highlighted ? Theme.highlightColor : Theme.primaryColor
                        }
                    }
                    ListView {
                        id: iconlist
                        z: -1
                        height: searchpage.height / 10
                        width: searchpage.width
                        orientation: ListView.Horizontal


                        cacheBuffer: searchpage.height * 2

                        spacing: Theme.paddingSmall
                        property int recsize0: (iconlist.width - 6*iconlist.spacing) / 7
                        property int recsize: recsize0
                        property bool textvis: false
                        property int numlegs: 0

                        function setupicons() {
                            var legnr = 0
                            var leg
                            while((leg = searchx.getLeg(index, legnr)) !== null) {
                                iconmodel.append({name: leg.line, fgcolour: leg.fgcolour, bgcolour: leg.bgcolour, dir: leg.dir,
                                                     fromname: leg.from.split(",")[0], fromtrack: leg.fromtrack,
                                                     destname: leg.to.split(",")[0], totrack: leg.totrack,
                                                     deptime: leg.deptime, depdate: leg.depdate, deprttime: leg.deprttime, deprtdate: leg.deprtdate,
                                                     arivtime: leg.arivtime, arivdate: leg.arivdate, arivrttime: leg.arivrttime, arivrtdate: leg.arivrtdate,
                                                     depdelay: leg.depdelay, arivdelay: leg.arivdelay})
                                legnr++;
                            }
                        }

                        model: ListModel {
                            id: iconmodel
                        }

                        delegate : Component {
                            id: icondelegate
                            Row {
                                Rectangle {
                                    id: outerrect
                                    width: iconlist.recsize0
                                    height: iconlist.recsize
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
                                        }
                                    }
                                }
                                Column {
                                    Label {
                                        id: legdir
                                        text: dir
                                        truncationMode: TruncationMode.Elide
                                        width: searchpage.width - iconlist.recsize0 - x
                                        x: Theme.paddingSmall
                                        visible: iconlist.textvis
                                        color: Theme.highlightColor
                                    }
                                    Row {
                                        x: Theme.paddingSmall
                                        Component.onCompleted: {
                                            legfrom.text = fromname + ((fromtrack === undefined || fromtrack === "") ? "" : ", " + fromtrack)
                                            legto.text = destname + ((totrack === undefined || totrack === "") ? "" : ", " + totrack)
                                            legdeptime.text = deptime
                                            legdeprttime.text = depdelay
                                            legarivtime.text = arivtime
                                            legarivrttime.text = arivdelay
                                        }

                                        Row {
                                            width: searchpage.width - iconlist.recsize0 - legdeptime.width - legdeprttime.width - 2 * parent.x
                                            Label {
                                                id: legfromlabel
                                                text: qsTr("From") + ": "
                                                visible: iconlist.textvis
                                                font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                                color: Theme.secondaryHighlightColor
                                            }
                                            Label {
                                                id: legfrom
                                                width: parent.width - legfromlabel.width
                                                truncationMode: TruncationMode.Elide
                                                visible: iconlist.textvis
                                                font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                                opacity: 0.6
                                            }

                                        }
                                        Label {
                                            id: legdeptime
                                            visible: iconlist.textvis
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                            opacity: 0.6
                                        }
                                        Label {
                                            id: legdeprttime
                                            visible: iconlist.textvis
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                        }
                                    }
                                    Row {
                                        x: Theme.paddingSmall
                                        Row {
                                            width: searchpage.width - iconlist.recsize0 - legarivtime.width - legarivrttime.width - 2 * parent.x
                                            Label {
                                                id: legtolabel
                                                text: qsTr("To") + ": "
                                                width: legfromlabel.width
                                                visible: iconlist.textvis
                                                font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                                color: Theme.highlightColor
                                                opacity: 0.6
                                            }
                                            Label {
                                                id: legto
                                                visible: iconlist.textvis
                                                width: parent.width - legtolabel.width
                                                truncationMode: TruncationMode.Elide
                                                font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                                opacity: 0.8
                                            }
                                        }
                                        Label {
                                            id: legarivtime
                                            visible: iconlist.textvis
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                            opacity: 0.8
                                        }
                                        Label {
                                            id: legarivrttime
                                            visible: iconlist.textvis
                                            color: Theme.primaryColor
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: (Theme.fontSizeTiny + Theme.fontSizeSmall) /2
                                        }
                                    }
                                }
                            }
                        }
                        remove:Transition {
                            ParallelAnimation {
                                NumberAnimation { property: "opacity"; to: 0; duration: trip.addanimdur }
                                NumberAnimation { property: "x"; to: 0; duration: trip.addanimdur }
                                NumberAnimation { property: "y"; to: 0; duration: trip.addanimdur }
                            }
                        }
                        add: Transition {
                            ParallelAnimation {
                                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: trip.addanimdur }
                                NumberAnimation { property: "x"; from: 0; duration: trip.addanimdur }
                                NumberAnimation { property: "y"; from: 0; duration: trip.addanimdur }
                            }
                        }

                    }
                }


                MouseArea {
                    id: mouse
                    anchors.fill: parent
                    z: 2
                    onClicked: {
                        iconmodel.clear();
                        statetimer.start();
                    }
                }

                Timer {
                    id: statetimer
                    interval: trip.addanimdur
                    onTriggered: {
                        listView.allstates[index] = (trip.state === "small") ? "big":"small"
                        trip.state = listView.allstates[index]
                        addtimer.start()
                        stop();
                    }
                }

                Timer {
                    id: addtimer
                    interval: trip.addanimdur
                    onTriggered: {
                        iconmodel.clear()
                        iconlist.setupicons()
                        stop();
                    }
                }

                states: [
                    State {
                        name: "small"
                        PropertyChanges {
                            target: trip
                            height: searchpage.height / 5
                            width: listView.width
                        }
                        PropertyChanges {
                            target: iconlist
                            orientation: ListView.Horizontal
                        }
                    },
                    State {
                        name: "big"
                        PropertyChanges {
                            target: trip
                            height: iconlist.height + searchpage.height / 10
                            width: listView.width
                        }

                        PropertyChanges {
                            id: propchange
                            target: iconlist
                            orientation: ListView.Vertical
                            recsize: width/4
                            height: iconlist.numlegs !== -1 ? (spacing + iconlist.recsize)*iconlist.numlegs - spacing : 0
                            textvis: true
                        }
                    }
                ]

                transitions: [
                    Transition {
                        NumberAnimation { target: trip; properties: "width,height"; duration: trip.addanimdur; easing.type: Easing.InOutQuad }
                    }
                ]
            }
            VerticalScrollDecorator {}
        }
    }
}
