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

Page {
    id: aboutpage
    

    Item {
        anchors.fill: parent        
        Column {
            anchors.centerIn: parent
            Item {
                height: aboutpage.width/4
                width: parent.width
                Image {
                    id: logo
                    width: 86
                    height: 86
                    anchors.centerIn: parent
                    source: "/usr/share/icons/hicolor/86x86/apps/harbour-travelplanner.png"
                }
            }
            spacing: Theme.paddingLarge

            Text {
                color: Theme.highlightColor;
                text: 'Created by Sebastian Ivarsson, '
            }
            Text {
                color: Theme.highlightColor;
                text: 'uses the <html><style type="text/css"></style><a href="http://labs.vasttrafik.se">Västtrafik API</a></html>'
                onLinkActivated: Qt.openUrlExternally(link)
                linkColor: "white"
            }
            Text {
                color: Theme.highlightColor;
                text: 'The source code is available on <html><style type="text/css"></style><a href="http://github.com/sebiva/travelplanner">github</a></html>'
                onLinkActivated: Qt.openUrlExternally(link)
                linkColor: "white"
            }
            Text {
                color: Theme.highlightColor;
                text: 'This program is licensed under <html><style type="text/css"></style><a href="http://gnu.org/licenses/licenses.html">GPL v3</a></html>'
                onLinkActivated: Qt.openUrlExternally(link)
                linkColor: "white"
            }
        }
    }           

}
