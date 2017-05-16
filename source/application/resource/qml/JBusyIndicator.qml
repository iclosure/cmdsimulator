import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import QtQuick.Window 2.2

Rectangle {
    id: root

    width: 300
    height: 300
    color: 'transparent'

    BusyIndicator {
        id: busyIndicator
        anchors.fill: parent
        running: parent.visible
    }
}
