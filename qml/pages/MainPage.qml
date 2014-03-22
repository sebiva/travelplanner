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
import "../database.js" as DBjs
import "../time.js" as Timejs

Dialog {
    id: maindialog
    property string time
    property string date
    property string from
    property string to
    property string fromid:"null"
    property string toid: "null"
    property bool typing: fromtext.typing || totext.typing
    property int animtime: 500

    canAccept: (fromid !== "null") && (toid !== "null") && !typing; // && (from !== "") && (to !== "");
    acceptDestination: canAccept ? Qt.resolvedUrl("SearchPage.qml") : null
    onAcceptPendingChanged: {
        console.log("fore searchfunc")
        searchfunc()
    }

    property bool fromready: false
    property bool toready: false

    Component.onCompleted: {
        DBjs.setup();
        mainWindow.getsettings()
        var lastsearch = DBjs.getlastsearch();
        if (lastsearch === 0) {
            return;
        }
        fromid = lastsearch.fromid
        toid = lastsearch.toid
        fromtext.text = lastsearch.from
        totext.text = lastsearch.to
        from = lastsearch.from
        to = lastsearch.to

    }
    function searchfunc() {
        if (fromready && toready) { //&& (from !== "") && (to !== "")) {
            DBjs.setlastsearch(fromid,toid,from,to)
            acceptDestinationInstance.from = from//text.text
            acceptDestinationInstance.to = to//text.text
            acceptDestinationInstance.fromid = fromid
            acceptDestinationInstance.toid = toid
            acceptDestinationInstance.time = timepicker.value// === "Select") ? Searchjs.getcurrenttime() : timepicker.value
            acceptDestinationInstance.date = datepicker.value//(datepicker.value === "Select") ? Searchjs.getcurrentdate() : datepicker.value
            mainWindow.searched = mainWindow.searched + 1 % 2
        }
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
                    text: mainWindow.strsettings
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                    }
                }

                MenuItem {
                    enabled: fromready || toready
                    text: mainWindow.strchangedir
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
                    acceptText: canAccept ? mainWindow.strsearch : mainWindow.strappname
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
                        if(txt.focus && txt.text.length > 2 && !autochang && typing) {
                            console.log("Getting name " + txt.text.length)
                            getsuggestions(txt.text);
                        }    
                    }
                    autochang = false
                }
                property bool autochang: false
                property bool chosen: false

                Row {
                    id: fromrow
                    width: column.width
                    height: fromtext.height
                    visible: !maindialog.typing || fromtext.typing
                    TextField {
                        property bool typing: false
                        id: fromtext
                        width: fromrow.width - height
                        onTextChanged: {
                            //console.log("changed fromtext : " + text + ":auto" + column.autochang + ":cli" + buttonfrom.clicked + ":chos" + column.chosen);
                            if (column.state !== "cleared" && column.active) {
                                column.textchang(true)
                            }

//                            if (buttonfrom.clicked) {
//                                buttonfrom.clicked = false
//                                text = ""
//                                return
//                            }
//                            if (column.autochang) {
//                                column.autochang = false
//                                return
//                            }
//                            if (column.chosen) {
//                                return
//                            }

                            //if (!column.autochang && !buttonfrom.clicked) {
                                column.textchang(true)
                            //}
                        }
                        placeholderText: mainWindow.strfrom
                        EnterKey.onClicked: ;//
                        onClicked: {
                            //column.chosen = false
                            column.active = true
                            column.state = "typing"
                            typing = true
                            if (text.length < 3) {
                                searchmodel.clear()
                            } else {
                                //console.log("clicked fromtext :" + text)
                                column.textchang(true)
                            }
                            //buttonfrom.clicked = false
                            Qt.inputMethod.show();
                        }
                        onFocusChanged: {
                            searchmodel.clear();
                            //console.log("Focus changed " + activeFocus + "btncli:" + buttonfrom.clicked + "chs:" + column.chosen)
//                            if (!activeFocus && buttonfrom.clicked && !column.chosen) {
//                                focus = true
//                                console.log("No focus")
//                                Qt.inputMethod.show()
//                            }
                            if (column.active) {
                                column.state = "typing"
                                focus = true
                                Qt.inputMethod.show()
                                typing = true
                                return
                            }

                            typing = focus;
                            text = maindialog.from
                            // Fix double triggerings of onTextChanged
//                            if (maindialog.from === "") {
//                                console.log("in if")
//                                buttonfrom.clicked = true
//                                column.autochang = true
//                            }
//                            text = maindialog.from;
                        }
                    }
                    BackgroundItem {
                        id: buttonfrom
                        width: maindialog.width - fromtext.width
                        height: fromtext.height - 2 * Theme.paddingLarge
                        highlighted: iconfrom.highlighted
                        property  bool clicked: false
                        IconButton {
                            id: iconfrom
                            z: 0
                            icon.source: "image://theme/icon-m-clear"
                            anchors.centerIn: parent
                            width: fromtext.height / 2
                            onClicked: buttonfrom.clikked()

                        }
                        function clikked() {
                            //console.log("Clearing : " + fromtext.text)
                            //column.autochang = true
                            searchmodel.clear()
                            //clicked = true
                            //searchlist.currentIndex = -1
                            column.state = "cleared"
                            fromready = false
                            maindialog.from = ""
                            maindialog.fromid = "null"
                            //console.log("Now Clearing : " + fromtext.text)
                            fromtext.focus = false
                            fromtext.text = "" //MAGI!!!!!!!!!!!!!!!!!!
                            //console.log("Cleared : " + fromtext.text + ":")
                        }
                        onClicked: clikked()
                    }

                }
                property bool active: false
                property string state: ""
                // typing, cleared, chosen
                Row {
                    id: torow
                    width: parent.width
                    visible: !maindialog.typing || totext.typing
                    TextField {
                        property bool typing: false
                        id: totext
                        width: parent.width - height
                        onTextChanged: {
                            console.log("STATE IN TEXT: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                            if (column.state === "cleared" && column.active) {
                                if (!activeFocus) {
                                    //focus = true
                                }
                                return
                            }
                            if (!column.active) {
                                return
                            }

//                            if (buttonto.clicked) {
//                                buttonto.clicked = false
//                                text = ""
//                                return
//                            }
//                            if (column.autochang) {
//                                column.autochang = false
//                                return
//                            }
//                            if (column.chosen) {
//                                return
//                            }
                            column.textchang(false);
                        }

                        placeholderText: mainWindow.strto
                        EnterKey.onClicked: ; //accept();
                        anchors.topMargin: Theme.paddingMedium
                        onClicked: {
                            console.log("STATE IN onClicked: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                            column.active = true
                            column.state = "typing"
                            //column.chosen = false
                            typing = true
                            //console.log("To" + text.length)
                            if(text.length < 3) {
                                searchmodel.clear()
                            } else {
                                column.textchang(false)
                            }
                            Qt.inputMethod.show();
                        }
                        onFocusChanged: {
                            searchmodel.clear();
                            console.log("STATE IN FOCUS CHANGED: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                            if (column.active) {
                                console.log("STATE IN if on FOCUS: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                                column.state = "typing"
                                focus = true
                                Qt.inputMethod.show()
                                typing = true
                                return
                            }

                            typing = focus;
                            text = maindialog.to;
//                            if (!activeFocus && (buttonto.clicked || !column.autochang) && !column.chosen) {
//                                buttonto.clicked = false
//                                focus = true
//                                Qt.inputMethod.show()
//                            }

//                            // Fix double triggerings of onTextChanged
//                            if (maindialog.to === "") {
//                                console.log("in if")
//                                buttonto.clicked = true
//                                column.autochang = true
//                            }

                        }
                    }
                    BackgroundItem {
                        id: buttonto
                        width: maindialog.width - totext.width
                        height: totext.height - 2 * Theme.paddingLarge
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
                            console.log("STATE IN X: "+ column.state + " FOCUS: " + focus + " ACTIVE: " + column.active)
                            //column.autochang = true
                            searchmodel.clear()
                            //clicked = true
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
                            value : Timejs.getcurrentdate() // new Date().getDate() + "/" + (new Date().getMonth()+1) + "/" + new Date().getFullYear()
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

                            label: mainWindow.strdate
                            onClicked: openDateDialog()
                            opacity: 0.8
                        }

                        ValueButton {
                            id: timepicker
                            value : Timejs.getcurrenttime()

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
                            label: mainWindow.strtime
                            onClicked: openTimeDialog()
                            opacity: 0.8
                        }
                    }
                    Button {
                        text: mainWindow.strnow
                        width: parent.width / 4
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            datepicker.value = Timejs.getcurrentdate()
                            timepicker.value = Timejs.getcurrenttime()
                        }
                    }
                }

            }

            SilicaListView {
                id: searchlist
                width: parent.width
                anchors.top: column.bottom
                height: maindialog.typing && (searchmodel.count > 0) ? maindialog.height - column.height : 0
                spacing: Theme.paddingMedium
                clip: true
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
                        //console.log("clicked: " + name)
                        column.state = "chosen"
                        column.active = false
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
                    totext.typing = false
                }
            }
        }

        Item {
            height: maindialog.height - search.height; width: parent.width;
            visible: favlist.empty && !maindialog.typing;
            Label{
                anchors.centerIn: parent;
                text: mainWindow.strnofavourites
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
                    //console.log("UPDATING: " + res[i].fromid + res[i].to + favlist.count);
                    favmodel.append(res[i]);
                }
            }

            Component.onCompleted: updatefavs()
            onDatabaseChanged: updatefavs()

            visible: !maindialog.typing && !empty
            header: Label {
                text: mainWindow.strfavourites
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
                            text: mainWindow.strdelete
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
                            text: mainWindow.strmovetotop
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
                                text: mainWindow.strfrom + ": "
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
                                text: mainWindow.strto + ":"
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


