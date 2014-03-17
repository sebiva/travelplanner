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
import "../search.js" as Searchjs

CoverBackground {
    id: coverpage
    property bool avail: mainWindow.avail
    property bool error: false
    property string from: mainWindow.from
    property string to: mainWindow.to
    property int searched: mainWindow.searched
    property string lang: mainWindow.lang
    property string err: mainWindow.errmsg
    onErrChanged: search(false)
    onLangChanged: search(false)
    onSearchedChanged: search(false)
    onAvailChanged: search(false)
    onFromChanged: search(false)
    onToChanged: search(false)
    function search(now) {
        console.log("SEARCHING FROM COVER " + avail)
        listmodel.clear();
        if (mainWindow.avail) {

            console.log("DURTIME " + Searchjs.duration(mainWindow.time, Searchjs.getcurrenttime(),mainWindow.date, Searchjs.getcurrentdate()));
            if (!now || (Searchjs.duration(mainWindow.time, Searchjs.getcurrenttime(),mainWindow.date, Searchjs.getcurrentdate()) === "0min")) {
                console.log("IFFFFFFF" + mainWindow.response.substring(0,200));
                Searchjs.setuplist(Searchjs.parse(mainWindow.response),listmodel);
            } else {
                console.log("ELSEEEEEE" + avail)
                triplist.searching = true;
                mainWindow.time = Searchjs.getcurrenttime();
                mainWindow.date = Searchjs.getcurrentdate();
                Searchjs.sendrequest(mainWindow.fromid, mainWindow.toid, mainWindow.date, mainWindow.time, triplist.doneloading, listmodel, mainWindow.changetime);
            }
        } else {
            console.log("HELLO " + mainWindow.lang + "::" + mainWindow.errmsg + "::" + mainWindow.strcovererr + "::" + mainWindow.errmsg === mainWindow.strerr)
            placeholdertext.text = mainWindow.errmsg === mainWindow.strerr ? mainWindow.strappname : mainWindow.strappname + "\n" + mainWindow.strcovererr;
        }
    }
    Column {
        anchors.centerIn: parent
        width: parent.width
        Item {
            height: coverpage.height/3
            width: parent.width
            opacity: !mainWindow.avail || error ? 1 : 0.3

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
            visible: !mainWindow.avail || error
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }


    Component.onCompleted: search()

    Column {
        anchors.fill: parent
        Label {id: from; x: Theme.paddingSmall; visible: mainWindow.avail; text: mainWindow.from; font.pixelSize: Theme.fontSizeTiny; width: parent.width; truncationMode: TruncationMode.Elide; color: Theme.secondaryHighlightColor}
        Label {id: to; x: Theme.paddingSmall; visible: mainWindow.avail; text: mainWindow.to; font.pixelSize: Theme.fontSizeTiny; width: parent.width; truncationMode: TruncationMode.Elide; color: Theme.highlightColor}

        ListView {
            id: triplist
            width: parent.width
            height: parent.height - from.height - to.height
            clip: true

            property bool searching: false

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
                visible: mainWindow.avail && !error
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

                Component.onCompleted: Searchjs.addicons(iconmodel, index)

                Column {
                    width: parent.width
                    Row {
                        height: triplist.height / 6
                        Label {
                            id: deptimelabel
                            text: deptime + Searchjs.delay(deptime, deprttime, depdate, deprtdate)
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: arivtimelabel
                            text: arivtime + Searchjs.delay(arivtime, arivrttime, arivdate, arivrtdate)
                            width: coverpage.width / 3
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: Theme.fontSizeTiny
                        }
                        Label {
                            id: durlabel
                            text: Searchjs.duration(deptime, arivtime, depdate, arivdate)
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
                                        color: color1
                                        opacity: 1
                                        Label {
                                            id: recttext
                                            text: name
                                            color: color2
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
        enabled: mainWindow.avail
        CoverAction {
            //Update the search for the current time
            iconSource: "image://theme/icon-cover-timer"
            onTriggered: coverpage.search(true);
        }
        CoverAction {
            //Update the search for the original or last used time
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: coverpage.search(false);
        }
    }
}


