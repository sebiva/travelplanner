/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0
import "../search.js" as Searchjs

CoverBackground {
    id: coverpage
    property bool avail: mainWindow.avail
    property bool error: false
    property string from: mainWindow.from
    onAvailChanged: search(false)
    onFromChanged: search(false)
    function search(now) {
        console.log("SEARCHING FROM COVER " + avail)
        listmodel.clear();
        if (mainWindow.avail) {

            console.log("DURTIME " + Searchjs.duration(mainWindow.time, Searchjs.getcurrenttime(),mainWindow.date, Searchjs.getcurrentdate()));
            if (!now || (Searchjs.duration(mainWindow.time, Searchjs.getcurrenttime(),mainWindow.date, Searchjs.getcurrentdate()) === "")) {
                console.log("IFFFFFFF" + mainWindow.response.substring(0,200));
                Searchjs.setuplist(Searchjs.parse(mainWindow.response),listmodel);
            } else {
                console.log("ELSEEEEEE" + avail)
                triplist.searching = true;
                mainWindow.time = Searchjs.getcurrenttime();
                mainWindow.date = Searchjs.getcurrentdate();
                Searchjs.sendrequest(mainWindow.fromid, mainWindow.toid, mainWindow.date, mainWindow.time, triplist.doneloading, listmodel);
            }
        } else {
            placeholdertext.text = mainWindow.errmsg === "" ? "Travelplanner" : mainWindow.errmsg;
        }
    }

    Label {
        id: placeholdertext
        visible: !mainWindow.avail || error
        anchors.centerIn: parent
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
            iconSource: "image://theme/icon-cover-timer"
            onTriggered: coverpage.search(true);
        }
        CoverAction {
            iconSource: "image://theme/icon-cover-refresh"
            onTriggered: coverpage.search(false);
        }
    }
}


