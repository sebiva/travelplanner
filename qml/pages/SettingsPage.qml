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
        title: qsTr("Settings")
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

            //Now uses LANG variable instead

            ComboBox {
                id: backendpicker
                label: qsTr("Backend")
                width: parent.width
                Component.onCompleted: {
                    var back = mainWindow.getsetting("backend")
                    var i = 0;
                    while(backendpicker.currentItem !== null) {
                        if (backendpicker.currentItem.text === back) {
                            return;
                        }
                        currentIndex = ++i;
                    }
                }

                menu: ContextMenu {
                    MenuItem {text: "Västtrafik"; onClicked: mainWindow.setsetting("backend", text)}
                    MenuItem {text: "SL"; onClicked: mainWindow.setsetting("backend", text)}
                }
            }


            //Doesn't work properly with Västtrafik...

//            ComboBox {
//                id: changetimepicker
//                label: mainWindow.strchangetime
//                width: parent.width
//                property string val: value
//                Component.onCompleted: {
//                    var time = mainWindow.getsetting("changetime")
//                    console.log("TIEMME" + time)
//                    var i = 0;
//                    while(changetimepicker.currentItem !== null) {
//                        if (changetimepicker.currentItem.text.split(" ")[0] === time) {
//                            return;
//                        }
//                        currentIndex = ++i;
//                    }
//                }


//                menu: ContextMenu {
//                    onActivated: mainWindow.setsetting("changetime", index)
//                    MenuItem {text: "0 " + mainWindow.strminutes; }
//                    MenuItem {text: "1 " + mainWindow.strminute; }
//                    MenuItem {text: "2 " + mainWindow.strminutes; }
//                    MenuItem {text: "3 " + mainWindow.strminutes; }
//                    MenuItem {text: "4 " + mainWindow.strminutes; }
//                    MenuItem {text: "5 " + mainWindow.strminutes; }
//                    MenuItem {text: "6 " + mainWindow.strminutes; }
//                    MenuItem {text: "7 " + mainWindow.strminutes; }
//                }
//            }
            Column {
                width: parent.width
                Label {
                    color: Theme.highlightColor;
                    text: qsTr("About")
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.paddingLarge
                    opacity: 0.5
                }

                Text {
                    color: Theme.primaryColor;
                    text: qsTr("Created by Sebastian Ivarsson")
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: qsTr("Uses the") + ' <html><style type="text/css"></style><a href="http://labs.vasttrafik.se">' + qsTr("Västtrafik API") + '</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: qsTr("The source code is available on") + ' <html><style type="text/css"></style><a href="http://github.com/sebiva/travelplanner">github</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
                Text {
                    color: Theme.primaryColor;
                    text: qsTr("Licensed under") + ' <html><style type="text/css"></style><a href="http://gnu.org/licenses/licenses.html">GPL v3</a></html>'
                    onLinkActivated: Qt.openUrlExternally(link)
                    linkColor: Theme.highlightColor
                    x: Theme.paddingLarge
                    width: parent.width
                }
            }
        }
    }           

}
