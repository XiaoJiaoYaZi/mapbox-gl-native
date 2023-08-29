import QtQuick

Rectangle{
    property url iconsource: ""
    signal clicked()
    radius: 8
    Image {
        source: iconsource
        sourceSize.width: area.pressed ?  parent.width-4 : parent.width
        sourceSize.height: area.pressed ? parent.height-4 : parent.height
        anchors.centerIn: parent
    }
    MouseArea{
        id: area
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
