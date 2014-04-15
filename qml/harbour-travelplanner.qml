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
import "pages"
import "database.js" as DBjs

//import searcher 1.0

ApplicationWindow
{
    id: mainWindow
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

//    property bool avail: false
//    property string from: ""
//    property string to: ""
//    property string fromid: ""
//    property string toid: ""
//    property string time: ""
//    property string date: ""
    property string changetime: "5"
    property string errmsg: qsTr("Search failed:") + "\n"
    property int verDB: 0
//    property string timeofsearch: ""
//    property string dateofsearch: ""
    property string backend: "Västtrafik"

//    property int searched: 1

    function incDB() {
        if (verDB===0) {
            verDB = 1;
        } else {
            verDB = (verDB+2) % 4
        }
    }

//    Search {
//        id: searcher
//    }

    function getsettings() {
        var language = searchx.getlanguage()
        if (language !== null) {
            lang = language
        } else {
            lang = stren
        }
        var time = DBjs.getsetting("changetime")
        if (time !== null) {
            changetime = time
        } else {
            changetime = "5"
        }
        var back = DBjs.getsetting("backend")
        if (back !== null) {
            backend = back
        } else {
            backend = "Västtrafik"
        }
        searchx.setbackend(backend); //Set the backend properly
    }

    function getsetting(setting) {
        var value = ""
        if (setting === "language") {
            value = searchx.getlanguage()
            if (value === null) {
                setsetting(setting, stren)
                value = stren
            }
            console.log("language " + value)

        } else if (setting === "changetime") {
            value = DBjs.getsetting(setting)
            if (value === null) {
                setsetting(setting, "5")
                value = "5"
            }
        } else if (setting === "backend") {
            value = DBjs.getsetting(setting)
            if (value === null) {
                setsetting(setting, "Västtrafik")
                value = "Västtrafik"
            }
        }
        return value
    }
    function setsetting(setting, value) {
        if (setting === "language") {
            lang = value;
        } else if (setting === "changetime") {
            changetime = value
        } else if (setting === "backend") {
            backend = value
            searchx.setbackend(backend);
        }
        DBjs.setsetting(setting, value)
        console.log("Database changed")
        incDB()
    }

    //Language
    property string lang: stren
    property string strsv: "Svenska"
    property string stren: "English"
}


