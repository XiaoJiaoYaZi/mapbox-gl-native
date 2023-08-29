import QtQuick
import QtQuick.Controls
import mbglquick

Rectangle {
    id:root
    Keys.enabled: true
    focus: true
    readonly property int iconSize: 48
    MapBoxItem{
        id:mapItem
        anchors.fill: parent
    }

    ImageButton{
        x:parent.width-2*iconSize
        y:400
        width: iconSize
        height: iconSize
        iconsource: "/mbglquick/platform/qt/quick/images/zoomout.svg"
        onClicked:mapItem.zoomIn()
    }

    ImageButton{
        x:parent.width-2*iconSize
        y:400 + 1.5*iconSize
        width: iconSize
        height: iconSize
        iconsource: "/mbglquick/platform/qt/quick/images/zoomin.svg"
        onClicked:mapItem.zoomOut()
    }

    ComboBox{
        id:styleCombox
        x:parent.width - 2* iconSize
        y: 400 + 3*iconSize
        width: iconSize
        height: iconSize

        model: ListModel{
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/streets.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/outdoor.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/light.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/dark.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/satelite.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/satelightstreet.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/NAV_DAY.svg"}
            ListElement{ iconPath:"/mbglquick/platform/qt/quick/images/NAV_NIGHT.svg"}
        }

        delegate: ItemDelegate{
            width: styleCombox.width
            height: styleCombox.height
            contentItem: Image {
                source: iconPath
            }
        }
        indicator: Item {
            id: name
        }

        contentItem:Item {
            width: styleCombox.width
            height: styleCombox.height
            Image {
                anchors.fill: parent
                source: styleCombox.currentValue
            }
        }
        onActivated: (index)=>{
            mapItem.setStyle(index)
        }
    }
}
