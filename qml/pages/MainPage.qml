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
import "../database.js" as DBjs

Dialog {
    id: maindialog
    property string time
    property string date
    property string from
    property string to
    property string fromid: "null"
    property string toid: "null"
    property bool typing: fromtext.typing || totext.typing
    property int animtime: 500
    canAccept: (fromid !== "null") && (toid !== "null") && !typing;
    acceptDestination: canAccept ? Qt.resolvedUrl("SearchPage.qml") : null
    onAcceptPendingChanged: {
        console.log("fore searchfunc")
        searchfunc()
    }

    property bool fromready: false
    property bool toready: false

    Component.onCompleted: DBjs.setup();

    function searchfunc() {
        acceptDestinationInstance.from = fromtext.text
        acceptDestinationInstance.to = totext.text
        acceptDestinationInstance.fromid = fromid
        acceptDestinationInstance.toid = toid
        acceptDestinationInstance.time = timepicker.value// === "Select") ? Searchjs.getcurrenttime() : timepicker.value
        acceptDestinationInstance.date = datepicker.value//(datepicker.value === "Select") ? Searchjs.getcurrentdate() : datepicker.value
    }

    Column {
        anchors.fill: parent
        SilicaFlickable {
            id: search
            width: parent.width
            property real height0: topheader.height + fromrow.height + torow.height + timerow.height
            height: maindialog.typing ? maindialog.height : height0
            clip: true

            contentHeight: height
            PullDownMenu {
                visible: !fromtext.typing && !totext.typing
                MenuItem {
                    text: "Change direction"
                    IconButton {
                        anchors.right: parent.right
                        icon.source: "image://theme/icon-m-shuffle"
                    }
                    onClicked: {
                        var temp = fromtext.text
                        fromtext.text = totext.text
                        totext.text = temp
                        temp = fromready
                        fromready = toready
                        toready = temp
                        temp = fromid
                        fromid = toid
                        toid = temp
                    }
                }

                MenuItem {
                    text: "Update favourites"
                    IconButton {
                        anchors.right: parent.right
                        icon.source: "image://theme/icon-m-developer-mode"
                    }

                    onClicked: {
                        var res = DBjs.getfaves(); //pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                        favmodel.clear();
                        for(var i=0;i<res.length;i++) {
                            //console.log("UPDATING: " + res[i].fromid + res[i].to + favlist.count);

                            favmodel.append(res[i]);
                        }

                    }
                }
            }



            // Place our content in a Column.  The PageHeader is always placed at the top
            // of the page, followed by our content.

            Column {
                id: column

                x: Theme.paddingSmall
                width: parent.width


                move: Transition {
                    NumberAnimation {
                        properties: "y"
                        easing.type: Easing.InOutQuad
                        duration: maindialog.animtime
                    }
                }
                add: Transition {
                    NumberAnimation {
                        properties: "y"
                        easing.type: Easing.InOutQuad
                        duration: maindialog.animtime
                    }
                }

                DialogHeader {
                    id: topheader
                    acceptText: canAccept ? "Search" : "TravelPlanner"
                    opacity: canAccept ? 1 : 0.8
                    visible: !maindialog.typing
                }
                Rectangle {
                    id: spacer
                    width: parent.width
                    height: Theme.paddingMedium
                    opacity: 0
                    visible: maindialog.typing
                }

                function getsuggestions(text) {
                    Searchjs.sendnamerequest(text,searchlist.filllist);
                }

                function textchang(from) {
                    //console.log("textchanged : from/to" + from)
                    var txt = from ? fromtext : totext;
                    searchmodel.clear();
                    if (txt.text !== "") {

                        if(from) {
                            fromready = true;
                            searchlist.from = true
                        } else {
                            toready = true;
                            searchlist.from = false;
                        }
                        if(txt.focus && txt.text.length > 2) {
                            getsuggestions(txt.text);
                        }
                    }
                }

                Row {
                    id: fromrow
                    width: parent.width
                    visible: !maindialog.typing || fromtext.typing
                    TextField {
                        property bool typing: false
                        property bool cleared: false
                        id: fromtext
                        width: parent.width - height
                        onTextChanged: column.textchang(true);
                        placeholderText: "from"
                        EnterKey.onClicked: totext.focus = true
                        onFocusChanged: {
                            searchmodel.clear();
                            typing = focus;
                            text = maindialog.from;
                        }
                    }
                    BackgroundItem {
                        id: buttonfrom
                        width: iconfrom.width * 2
                        highlighted: iconfrom.highlighted
                        IconButton {
                            id: iconfrom
                            z: 0
                            visible: !maindialog.typing
                            icon.source: "image://theme/icon-m-clear"
                            anchors.centerIn: parent
                            width: fromtext.height / 2
                            onClicked: buttonfrom.clikked()

                        }
                        function clikked() {
                            fromtext.text = ""
                            maindialog.from = ""
                            maindialog.fromid = "null"
                        }

                        onClicked: clikked()
                    }
                }

                Row {
                    id: torow
                    width: parent.width
                    visible: !maindialog.typing || totext.typing
                    TextField {
                        property bool typing: false
                        property bool cleared: false
                        id: totext

                        width: parent.width - height
                        onTextChanged: column.textchang(false);
                        EnterKey.onClicked: accept();
                        placeholderText: "to"
                        anchors.topMargin: Theme.paddingMedium
                        onActiveFocusChanged: { focusChanged(focus) }
                        onFocusChanged: {
                            searchmodel.clear();
                            if (!cleared) {
                                typing = focus;
                                text = maindialog.to;
                            }


                        }

                    }
                    BackgroundItem {
                        id: buttonto
                        width: iconto.width * 2
                        highlighted: iconto.highlighted
                        IconButton {
                            id: iconto
                            visible: !maindialog.typing
                            icon.source: "image://theme/icon-m-clear"
                            anchors.centerIn: parent
                            width: fromtext.height / 2
                            onClicked: buttonto.clikked()
                        }
                        function clikked() {
                            //totext.cleared = true
                            totext.text = ""
                            maindialog.to = ""
                            maindialog.toid = "null"
                        }
                        onClicked: clikked()
                    }
                }

                Row {
                    id: timerow
                    visible: !maindialog.typing
                    spacing: Theme.paddingMedium

                    height: datepicker.height + timepicker.height
                    width: parent.width
                    Column {

                        width: 2 * parent.width / 3

                        ValueButton {
                            id: datepicker
                            value : Searchjs.getcurrentdate() // new Date().getDate() + "/" + (new Date().getMonth()+1) + "/" + new Date().getFullYear()
                            function openDateDialog() {
                                var dialog = pageStack.push("Sailfish.Silica.DatePickerDialog", { date: new Date(value.split("-")[0],  (parseInt(value.split("-")[1])-1), value.split("-")[2] )})

                                dialog.accepted.connect(function() {
                                    var month = dialog.month; var day = dialog.day;
                                    if (month < 10) {
                                        month = "0" + month;
                                    }
                                    if (day < 10) {
                                        day = "0" + day;
                                    }

                                    value = dialog.year + "-" + month + "-" + day
                                })
                            }

                            label: "Date"
                            onClicked: openDateDialog()
                            opacity: 0.8
                        }

                        ValueButton {
                            id: timepicker
                            value : Searchjs.getcurrenttime()

                            function openTimeDialog() {
                                var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog", {
                                                                hourMode: DateTime.TwentyFourHours,
                                                                hour: value.split(":")[0],
                                                                minute: value.split(":")[0]
                                                            })

                                dialog.accepted.connect(function() {
                                    value = dialog.timeText
                                })
                            }

                            label: "Time"
                            onClicked: openTimeDialog()
                            opacity: 0.8
                        }
                    }
                    Button {
                        text: "now"
                        width: parent.width / 4
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            datepicker.value = Searchjs.getcurrentdate()//new Date().getDate() + "/" + (new Date().getMonth()+1) + "/" + new Date().getFullYear()
                            timepicker.value = Searchjs.getcurrenttime()
                        }
                    }
                }

            }
            ListView {
                id: searchlist
                width: parent.width
                anchors.top: column.bottom
                height: maindialog.typing ? maindialog.height - column.height : 0
                spacing: Theme.paddingMedium
                currentIndex: -1
                property bool from: false

                function filllist(stops) {

                    searchmodel.clear();
                    var i = 0;
                    while(stops[i] !== undefined) {
                        searchmodel.append({name: stops[i].name, nmbr: stops[i].id});
                        i++;
                    }
                }
                model: ListModel {id: searchmodel}
                delegate: ListItem {
                    id: searchdelegate
                    x: Theme.paddingSmall
                    Label {
                        id: searchtext
                        width: parent.width
                        text: name
                    }
                    onClicked: {
                        //console.log("clicked: " + name)
                        if (searchlist.from) {
                            fromtext.typing = false;
                            maindialog.fromid = nmbr;
                            maindialog.from = name;
                        } else {
                            totext.typing = false;
                            maindialog.toid = nmbr;
                            maindialog.to = name
                        }
                        searchmodel.clear()
                       // console.log(searchmodel.count)
                    }
                }
            }
        }
        SilicaListView {
            id: favlist
            property bool empty: true

            property int database: mainWindow.database
            function updatefavs() {
                var res = DBjs.getfaves();
                favmodel.clear();
                for(var i=0;i<res.length;i++) {
                    //console.log("UPDATING: " + res[i].fromid + res[i].to + favlist.count);
                    favmodel.append(res[i]);
                }
            }

            Component.onCompleted: updatefavs()
            onDatabaseChanged: updatefavs()

            visible: !maindialog.typing
            header: Label {
                text: "Favourites"
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingLarge
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.highlightColor
            }

            Timer {
                id: movetimer
                interval: 400
                onTriggered: {
                    anifromtext.state = "done"
                    anitotext.state = "done"
                    maindialog.from = anifromtext.text
                    maindialog.to = anitotext.text
                    fromtext.text = maindialog.from
                    totext.text = maindialog.to
                }
            }

            ListModel {
                id: favmodel
                //ListElement {from: "f"; to: "u"; fromid: "oeu"; toid: "oeuaao"}
            }
            Component {
                id: favdelegate

                ListItem {
                    id: favlistitem
                    property string fid: fromid
                    property string tid: toid
                    onClicked: {
                        anifromtext.text = favfromtext.text;
                        anitotext.text = favtotext.text;
                        if(movetimer.running) {
                            return;
                        }
                        anifromtext.opacity = 1;
                        anitotext.opacity = 1;

                        var cordfrom = favfromtext.mapToItem(null, favfromtext.x, favfromtext.y);
                        anifromtext.x = cordfrom.x - favfromheadtext.width;
                        anifromtext.y = cordfrom.y;
                        var cordto = favtotext.mapToItem(null, favtotext.x, favtotext.y);
                        anitotext.x = cordto.x - favtoheadtext.width;
                        anitotext.y = cordto.y;

                        anifromtext.state = "start";
                        anitotext.state = "start";
                        movetimer.start();
                        maindialog.fromid = fromid;
                        maindialog.toid = toid;
                    }


                    menu: ContextMenu {
                        MenuItem {
                            text: "delete"
                            onClicked: { DBjs.remfav(fid, tid); favmodel.remove(index) }
                        }
                        MenuItem {
                            text: "move to top"
                            onClicked: {
                                favmodel.move(index, 0, 1);
                                DBjs.movetotop(fid, tid, favfromtext.text, favtotext.text);
                            }
                        }
                    }
                    Column {
                        id: faventry
                        Row {
                            x: Theme.paddingSmall
                            y: Theme.paddingSmall
                            Label {
                                id: favfromheadtext
                                x: Theme.paddingMedium
                                text: "From: "
                                color: Theme.secondaryHighlightColor
                            }

                            Label {
                                id: favfromtext
                                x: Theme.paddingMedium
                                text: from
                                font.pixelSize: Theme.fontSizeMedium
                                opacity: 0.6
                            }
                        }
                        Row {
                            x: Theme.paddingSmall
                            y: Theme.paddingSmall
                            Label {
                                id: favtoheadtext
                                x: Theme.paddingMedium
                                text: "To:"
                                color: Theme.highlightColor
                                width: favfromheadtext.width
                                opacity: 0.6
                            }
                            Label {
                                id: favtotext
                                x: Theme.paddingMedium
                                text: to
                                font.pixelSize: Theme.fontSizeMedium
                                opacity: 0.8
                            }
                        }
                    }
                }


            }
            ListModel {
                id: favemptymodel
            }
            Component {
                id: favemptydelegate
                Label {text: "No favourites yet"}
            }

            clip: true
            spacing: Theme.paddingMedium
            height: parent.height - search.height
            //contentHeight: childrenRect.height
            model: empty ? favmodel : favemptymodel
            delegate: empty ? favdelegate : favemptydelegate
            width: parent.width
            VerticalScrollDecorator{}
        }

    }

    Label {
        id: anifromtext
        opacity: 0

        states: [
            State {
                name: "start"
                PropertyChanges {
                    target: anifromtext
                    x: fromtext.x + Theme.paddingLarge + Theme.paddingSmall
                    y: topheader.height//fromtext.y + Theme.paddingSmall

                }
            }, State {
                name: "done"
                PropertyChanges {
                    target: anifromtext
                    opacity: 0
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { id: anifrommove; properties: "x,y"; easing.type: Easing.InOutQuad; duration: 450 }
            }, Transition {
                NumberAnimation { id: anifromopac; property: "opacity"; duration: 300}
            }
        ]
    }
    Label {
        id: anitotext
        opacity: 0

        states: [
            State {
                name: "start"
                PropertyChanges {
                    target: anitotext
                    x: totext.x + Theme.paddingLarge + Theme.paddingSmall
                    y: topheader.height + fromtext.height//totext.y + Theme.paddingSmall
                }
            }, State {
                name: "done"
                PropertyChanges {
                    target: anitotext
                    opacity: 0
                }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { id: anitomove; properties: "x,y"; easing.type: Easing.InOutQuad; duration: 450 }
            }, Transition {
                NumberAnimation { id: anitoopac; property: "opacity"; duration: 300}
            }
        ]
    }
}


