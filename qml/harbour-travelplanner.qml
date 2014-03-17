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

ApplicationWindow
{
    id: mainWindow
    initialPage: Component { MainPage { } }
    cover: Qt.resolvedUrl("cover/CoverPage.qml")

    property bool avail: false
    property string from: ""
    property string to: ""
    property string fromid: ""
    property string toid: ""
    property string time: ""
    property string date: ""
    property string changetime: "5"
    property string errmsg: strerr
    property string response: ""
    property int database: 1

    property int searched: 1

    function getsettings() {
        var language = DBjs.getsetting("language")
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
    }

    function getsetting(setting) {
        var value = DBjs.getsetting(setting)
        if (setting === "language") {
            if (value === null) {
                setsetting(setting, stren)
                value = stren
                lang = stren
            }
            return value
        } else if (setting === "changetime") {
            if (value === null) {
                setsetting(setting, "5")
                value = "5"
                changetime = "5"
            }
            return value
        }

    }
    function setsetting(setting, value) {
        console.log("Setting " + setting + " : " + value)
        if (setting === "language") {
            lang = value;
        } else if (setting === "changetime") {
            changetime = value
        }
        DBjs.setsetting(setting, value)
    }

    //Language
    property string lang: stren
    property string strsv: "Svenska"
    property string stren: "English"

    //All strings
    property string strappname: lang === stren ? "Travelplanner" : "Reseplaneraren"
    property string strsearch: lang === stren ? "Search" : "Sök"
    property string strfrom: lang === stren ? "From" : "Från"
    property string strto: lang === stren ? "To" : "Till"
    property string strdate: lang === stren ? "Date" : "Datum"
    property string strtime: lang === stren ? "Time" : "Tid"
    property string strnow: lang === stren ? "Now" : "Nu"
    property string strfavourites: lang === stren ? "Favourites" : "Favoriter"
    property string strnofavourites: lang === stren ? "No favourites" : "Inga favoriter"
    property string strmovetotop: lang === stren ? "Move to top" : "Flytta överst"
    property string strdelete: lang === stren ? "Delete" : "Ta bort"

    property string strchangedir: lang === stren ? "Change direction" : "Byt riktning"
    property string strsettings: lang === stren ? "Settings" : "Inställningar"
    property string strlanguage: lang === stren ? "Language" : "Språk"
    property string strchangetime: lang === stren ? "Exchange time" : "Bytestid"
    property string strminute: lang === stren ? "minute" : "minut"
    property string strminutes: lang === stren ? "minutes" : "minuter"
    property string strabout: lang === stren ? "About" : "Info"
    property string strcreated: lang === stren ? "Created by Sebastian Ivarsson" : "Skapad av Sebastian Ivarsson"
    property string strapi: lang === stren ? "Uses the" : "Använder"
    property string strvasttrafik: lang === stren ? "Västtrafik API" : "Västtrafiks API"
    property string strgit: lang === stren ? "The source code is available on" : "Källkoden finns tillgänglig på"
    property string strgpl: lang === stren ? "Licensed under" : "Licensierat under"
    property string strsearchres: lang === stren ? "Search Results" : "Sökresultat"
    property string straddfavourite: lang === stren ? "Save as favourite" : "Spara som favorit"
    property string strerr: lang === stren ? "Search failed:\n" : "Sökningen misslyckades:\n"
    property string strcovererr: lang === stren ? "Search failed" : "Sökningen\nmisslyckades"
}


