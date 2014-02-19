import QtQuick 2.0
import Sailfish.Silica 1.0

import "../search.js" as Searchjs


Page {
    id: searchpage
    property variant vals: [from, to, date, time]
    property string from
    property string to
    property string date
    property string time


    property string name

    property string fromid : "9021014001200000"
    property string toid : "9021014001960000"

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: col.height
        Component.onCompleted:  {
            console.log(date, time)
            loadwaittimer.start()
            //Searchjs.sendrequest(fromid, toid, date, time, textarea.doneloading)
        }
        Timer {
            id: loadwaittimer
            interval: 10
            onTriggered: Searchjs.sendrequest(fromid, toid, date, time, textarea.doneloading)
        }

        Column {
            id: col
            //anchors.fill: parent
            width: parent.width
            PageHeader {
                title: "Results"
            }

            Button {
                id: btn
                height: 100
                text: "search"
                onClicked: {
                    console.log(date, time)
                }
            }

            TextArea {
                id: textarea
                width: parent.width
                height: implicitHeight
                text: "hej"
                function doneloading(answer) {
                    console.log(answer.substring(0,500));
                    textarea.text = answer;
                }

            }

            BusyIndicator {
                running: true;
                width: parent.width / 3
                height: width
                anchors.horizontalCenter:  parent.horizontalCenter
                size: BusyIndicatorSize.Large
            }


        }
    }
    //    SilicaListView {
    //        id: listView
    //        model: 4

    //        anchors.fill: parent
    //        header: PageHeader {
    //            id: head
    //            title: "Search Results"
    //        }
    //        delegate: BackgroundItem {
    //            id: delegate
    //            function doneloading(answer) {
    //                console.log(answer.substring(0,500));
    //                label.text = answer;
    //            }
    //            Label {
    //                id: label
    //                x: Theme.paddingLarge
    //                text: vals[index]
    //                anchors.verticalCenter: parent.verticalCenter
    //                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
    //            }
    //            onClicked: {
    //                console.log("Clicked " + index, vals[index])
    //                Searchjs.sendrequest(fromid, toid, date, time, delegate.doneloading)
    //            }
    //        }
    //        VerticalScrollDecorator {}
    //    }



}
