import QtQuick
import QtQuick.Window
import QtQuick.Controls

ApplicationWindow {
    id: rootWindow
    width: 800
    height: 600
    MapBoxView{
        anchors.fill: parent
    }

    Component.onCompleted: rootWindow.show()
}
