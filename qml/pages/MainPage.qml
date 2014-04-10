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

import searcher 1.0
import timehelp 1.0


Dialog {
    id: maindialog
    property string from
    property string to
    property string fromid:"null"
    property string toid: "null"
    property bool fromready: false
    property bool toready: false
    property bool typing: fromtext.typing || totext.typing
    property int animtime: 500

    canAccept: (fromid !== "null") && (toid !== "null") && !typing;
    acceptDestination: canAccept ? Qt.resolvedUrl("SearchPage.qml") : null

    onAccepted: {
        searcher.setfromid(fromid)
        searcher.settoid(toid)
        searcher.setfrom(from)
        searcher.setto(to)
        searcher.setdate(datepicker.value)
        searcher.settime(timepicker.value)
        searcher.setdateofsearch(timehelp.getcurrentdate())
        searcher.settimeofsearch(timehelp.getcurrenttime())
    }


    Search {
        id: searcher
        onReady: console.log("Ready signal received in MainPage");
        //TODO: TEST
        onSearching: {
            console.log("MainPage, onSearchnig")
        }
        onStopsready: {
            console.log("Mainpage, stops ready")
            searchmodel.clear()
            var i = 0
            while(i < searcher.getnumstops()) {
                var data = searcher.getstop(i).split("#")
                searchmodel.append({name: data[0], nmbr: data[1] } )
                i++
            }
        }
    }
    Timehelp {
        id: timehelp
    }


    Component.onCompleted: {

        DBjs.setup()
        mainWindow.getsettings()
        var lastsearch = DBjs.getlastsearch()
        if (lastsearch === 0) {
            return;
        }
        fromid = lastsearch.fromid
        toid = lastsearch.toid
        fromtext.text = lastsearch.from
        totext.text = lastsearch.to
        from = lastsearch.from
        to = lastsearch.to
        fromready = true
        toready = true
        console.log("Loaded MainPage\n")
    }

    Column {
        anchors.fill: parent
        SilicaFlickable {
            id: search
            width: parent.width
            property real height0: topheader.height + fromrow.height + torow.height + timerow.height
            height: maindialog.typing ? maindialog.height : height0
            clip: true
            y: 0

            contentHeight: height
            PullDownMenu {
                visible: !fromtext.typing && !totext.typing

                MenuItem {
                    text: qsTr("Settings")
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                    }
                }

                MenuItem {
                    enabled: fromready || toready
                    text: qsTr("Change direction")
                    IconButton {
                        visible: parent.enabled
                        anchors.right: parent.right
                        icon.source: "image://theme/icon-m-shuffle"
                    }
                    onClicked: {
                        var temp = from
                        from = to
                        to = temp
                        var temptext = fromtext.text
                        fromtext.text = totext.text
                        totext.text = temptext
                        var tempbool = fromready
                        fromready = toready
                        toready = tempbool
                        var tempid = fromid
                        fromid = toid
                        toid = tempid
                    }
                }
            }

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
                    acceptText: canAccept ? qsTr("Search") : qsTr("Travelplanner")
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
                    searcher.getstops(text)
                }

                function textchang(from) {
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
                        if(txt.focus && txt.text.length > 2 && typing) {
                            console.log("Getting name " + txt.text.length)
                            getsuggestions(txt.text);
                        }    
                    }
                }

                property bool active: false
                property string state: ""
                // typing, cleared, chosen


                Row {
                    id: fromrow
                    width: column.width
                    visible: !maindialog.typing || fromtext.typing
                    TextField {
                        property bool typing: false
                        id: fromtext
                        width: fromrow.width - height
                        y: Theme.paddingLarge
                        onTextChanged: {
                            if (column.state !== "cleared" && column.active) {
                                column.textchang(true)
                            }
                        }
                        placeholderText: qsTr("From")
                        onClicked: {
                            column.active = true
                            if (text.length < 3) {
                                searchmodel.clear()
                            } else if (column.state !== "typing"){
                                column.textchang(true)
                            }
                            column.state = "typing"
                            typing = true
                            Qt.inputMethod.show();
                        }
                        onFocusChanged: {
                            searchmodel.clear();
                            if (column.active) {
                                column.state = "typing"
                                focus = true
                                Qt.inputMethod.show()
                                typing = true
                                return
                            }
                            typing = focus;
                            text = maindialog.from
                        }
                    }
                    BackgroundItem {
                        id: buttonfrom
                        width: maindialog.width - fromtext.width
                        height: parent.height
                        highlighted: iconfrom.highlighted
                        property  bool clicked: false
                        IconButton {
                            id: iconfrom
                            icon.source: "image://theme/icon-m-clear"
                            anchors.centerIn: parent
                            width: fromtext.height / 2
                            onClicked: buttonfrom.clikked()
                        }
                        function clikked() {
                            searchmodel.clear()
                            column.state = "cleared"
                            fromready = false
                            maindialog.from = ""
                            maindialog.fromid = "null"
                            fromtext.focus = false
                            fromtext.text = ""
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
                        id: totext
                        width: parent.width - height
                        y: Theme.paddingLarge
                        onTextChanged: {
                            if (column.state !== "cleared" && column.active) {
                                column.textchang(false);
                            }
                        }
                        placeholderText: qsTr("To")
                        onClicked: {
                            column.active = true
                            if(text.length < 3) {
                                searchmodel.clear()
                            } else if (column.state !== "typing") {
                                column.textchang(false)
                            }
                            column.state = "typing"
                            typing = true
                            Qt.inputMethod.show();
                        }
                        onFocusChanged: {
                            searchmodel.clear();
                            if (column.active) {
                                column.state = "typing"
                                focus = true
                                Qt.inputMethod.show()
                                typing = true
                                return
                            }
                            typing = focus;
                            text = maindialog.to;
                        }
                    }
                    BackgroundItem {
                        id: buttonto
                        width: maindialog.width - totext.width
                        height: parent.height
                        highlighted: iconto.highlighted
                        property bool clicked: false
                        IconButton {
                            id: iconto
                            icon.source: "image://theme/icon-m-clear"
                            anchors.centerIn: parent
                            width: fromtext.height / 2
                            onClicked: buttonto.clikked()
                        }
                        function clikked() {
                            searchmodel.clear()
                            column.state = "cleared"
                            toready = false
                            maindialog.to = ""
                            maindialog.toid = "null"
                            totext.focus = false
                            totext.text = ""
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
                            value : timehelp.getcurrentdatestr()
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

                            label: qsTr("Date")
                            onClicked: openDateDialog()
                            opacity: 0.8
                        }

                        ValueButton {
                            id: timepicker
                            value : timehelp.getcurrenttimestr()

                            function openTimeDialog() {
                                var dialog = pageStack.push("Sailfish.Silica.TimePickerDialog", {
                                                                hourMode: DateTime.TwentyFourHours,
                                                                hour: value.split(":")[0],
                                                                minute: value.split(":")[1]
                                                            })

                                dialog.accepted.connect(function() {
                                    value = dialog.timeText
                                })
                            }
                            label: qsTr("Time")
                            onClicked: openTimeDialog()
                            opacity: 0.8
                        }
                    }
                    Button {
                        text: qsTr("Now")
                        width: parent.width / 4
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            datepicker.value = timehelp.getcurrentdatestr()
                            timepicker.value = timehelp.getcurrenttimestr()
                        }
                    }
                }

            }

            SilicaListView {
                id: searchlist
                width: parent.width
                anchors.top: column.bottom
                height: maindialog.typing && (searchmodel.count > 0) ? maindialog.height - column.height : 0
                //spacing: Theme.paddingMedium
                clip: true
                currentIndex: -1
                z: 5
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
                delegate:
                    BackgroundItem {
                    id: searchdelegate
                    width: parent.width
                    height: searchtext.height + 2 * Theme.paddingLarge
                    Label {
                        id: searchtext
                        width: parent.width- 2 * Theme.paddingLarge
                        truncationMode: TruncationMode.Elide
                        text: name
                        anchors.centerIn: parent
                        x: Theme.paddingLarge
                        color: searchdelegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                    onClicked: {
                        console.log("STATE IN SELECT: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                        column.state = "chosen"
                        column.active = false
                        if (searchlist.from) {
                            fromready = true
                            fromtext.typing = false;
                            maindialog.fromid = nmbr;
                            maindialog.from = name;
                            fromtext.text = name
                        } else {
                            toready = true
                            totext.typing = false;
                            maindialog.toid = nmbr;
                            maindialog.to = name
                            totext.text = name
                        }
                        searchmodel.clear()
                    }
                }
            }
            BackgroundItem {
                id: fillerfrom
                width: fromtext.typing ? maindialog.width : 0
                anchors.top: searchlist.bottom
                anchors.bottom: parent.bottom
                highlightedColor: "transparent"
                onClicked: {
                    console.log("STATE IN FILLER: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                    column.active = false
                    column.state = ""
                    fromtext.typing = false
                }
            }
            BackgroundItem {
                id: fillerto
                width: totext.typing ? maindialog.width : 0
                anchors.top: searchlist.bottom
                anchors.bottom: parent.bottom
                highlightedColor: "transparent"
                onClicked: {
                    console.log("STATE IN FILLER: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                    column.active = false
                    column.state = ""
                    totext.typing = false
                }
            }
        }

        Item {
            height: maindialog.height - search.height; width: parent.width;
            visible: favlist.empty && !maindialog.typing;
            Label{
                anchors.centerIn: parent;
                text: qsTr("No favourites")
                color: Theme.highlightColor;
                Component.onCompleted: favlist.updatefavs()
            }
        }
        SilicaListView {
            id: favlist
            property bool empty: true

            property int database: mainWindow.database
            function updatefavs() {
                var res = DBjs.getfaves();
                favmodel.clear();
                if (res === 0) {
                    empty = true;
                    return;
                } else {
                    empty = false;
                }
                console.log("Updating favourites " + empty)

                for(var i=0;i<res.length;i++) {
                    favmodel.append(res[i]);
                }
            }

            Component.onCompleted: updatefavs()
            onDatabaseChanged: updatefavs()

            visible: !maindialog.typing && !empty
            header: Label {
                text: qsTr("Favourites")
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
                    fromtext.text = maindialog.from
                    totext.text = maindialog.to
                }
            }

            ListModel {
                id: favmodel
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
                        maindialog.from = anifromtext.text
                        maindialog.to = anitotext.text
                        maindialog.fromid = fromid;
                        maindialog.toid = toid;
                        if(movetimer.running) {
                            return;
                        }
                        fromready = true
                        toready = true
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
                    }


                    menu: ContextMenu {
                        MenuItem {
                            text: qsTr("Delete")
                            onClicked: {
                                console.log(favmodel.count + " " + index + " uahoeutna hej");
                                DBjs.remfav(fid, tid);

                                favmodel.remove(index);
                                if (favmodel.count === 0) {
                                    favlist.empty = true
                                }
                            }
                        }
                        MenuItem {
                            text: qsTr("Move to top")
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
                                text: qsTr("From") + ": "
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
                                text: qsTr("To") + ":"
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

            clip: true
            spacing: Theme.paddingMedium
            height: parent.height - search.height
            //contentHeight: childrenRect.height
            model: favmodel
            delegate: favdelegate
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
                    y: topheader.height

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
                    y: topheader.height + fromtext.height
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


