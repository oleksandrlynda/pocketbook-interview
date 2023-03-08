import QtQuick
import QtQuick.Layouts

// debug rect snippet
//Rectangle { anchors.fill: parent; color: "#30ff0000" }

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    ColumnLayout {
        anchors.fill: parent
        Rectangle {
            id: topBar
            color: "lightgrey"
            Layout.fillWidth: true
            Layout.preferredHeight: 64

            Text {
                anchors.centerIn: parent
                text: controller.imagesPath
                font.bold: true
            }
        }

        ListView {
            id: filterView

            Layout.rightMargin: 8
            Layout.preferredHeight: 32
            implicitWidth: contentWidth
            Layout.alignment: Qt.AlignRight

            orientation: ListView.Horizontal
            spacing: 8

            model: ListModel {
                ListElement { filterName: "All" }
                ListElement { filterName: "bmp" }
                ListElement { filterName: "png" }
                ListElement { filterName: "barch" }
            }

            delegate: Rectangle {
                height: filterView.height
                width: filterLabel.width + 16
                color: "#d3d3d3"
                border {
                    width: 4
                    color: filterView.currentIndex === index ? "#a3c3f3" : "#d3d3d3"
                }

                Text {
                    id: filterLabel
                    anchors.centerIn: parent
                    text: filterName
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        filterView.currentIndex = index
                        controller.filter(filterName)
                    }
                }
            }
        }

        ListView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 8
            spacing: 8

            clip: true
            model: controller.model

            delegate: Rectangle {
                width: view.width
                height: 32
                color: "#f3f3f3"

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 8

                    Text {
                        text: name
                    }

                    Item {
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: 150
                        Layout.fillHeight: true

                        Text {
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: suffix
                        }

                        Text {
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: size
                        }
                    }
                }
            }
        }
    }
}
