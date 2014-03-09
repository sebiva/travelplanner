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
    id: settingspage
    PageHeader {
        title: mainWindow.strsettings
    }

    Item {
        anchors.fill: parent        
        Column {
            width: parent.width
            anchors.centerIn: parent
            Item {
                height: settingspage.width/4
                width: parent.width
                Image {
                    id: logo
                    width: 86
                    height: 86
                    anchors.centerIn: parent
                    source: "/usr/share/icons/hicolor/86x86/apps/harbour-travelplanner.png"
                }
            }

            ComboBox {
                id: languagepicker
                label: mainWindow.strlanguage
                width: parent.width
                Component.onCompleted: {
                    var lang = mainWindow.getlanguage()
                    var i = 0;
                    while(languagepicker.currentItem !== null) {
                        if (languagepicker.currentItem.text === lang) {
                            return;
                        }
                        currentIndex = ++i;
                    }
                }

                menu: ContextMenu {
                    MenuItem {text: mainWindow.stren; onClicked: mainWindow.setlanguage(text)}
                    MenuItem {text: mainWindow.strsv; onClicked: mainWindow.setlanguage(text)}
                }
            }
            Column {
                width: parent.width
                Label {
                    color: Theme.highlightColor;
                    text: mainWindow.strabout
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.paddingLarge
                    opacity: 0.5
                }

                Text {
                    color: Theme.primaryColor;
                    text: mainWindow.strcreated
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: mainWindow.strapi + ' <html><style type="text/css"></style><a href="http://labs.vasttrafik.se">' + mainWindow.strvasttrafik + '</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: mainWindow.strgit + ' <html><style type="text/css"></style><a href="http://github.com/sebiva/travelplanner">github</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: mainWindow.strgpl + ' <html><style type="text/css"></style><a href="http://gnu.org/licenses/licenses.html">GPL v3</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
            }
        }
    }           

}
