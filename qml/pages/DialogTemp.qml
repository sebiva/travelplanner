import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: dialog
    property string val
    canAccept: true
    acceptDestination: Qt.resolvedUrl("MainPage.qml")




    SilicaFlickable {
        anchors.fill: parent
        Column {
            width: parent.width
            DialogHeader {
                acceptText: "Accept"
                opacity: 0.5
            }
            Label {
                text: "Hello"
            }
        }
    }
}
