import QtQuick
import QtQuick.Layouts

// debug rect snippet
//Rectangle { anchors.fill: parent; color: "#30ff0000" }

Window {
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("PocketBook")

    Item {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            Rectangle {
                id: topBar
                color: "#d3d3d3"
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
                Layout.rightMargin: 8
                Layout.leftMargin: 8
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
                            Layout.preferredWidth: 200
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

                    Rectangle {
                        width: stateText.width + 8
                        height: stateText.height + 8
                        anchors.centerIn: parent
                        visible: stateText.text.length > 0
                        Text {
                            id: stateText
                            anchors.centerIn: parent
                            text: progressState
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            controller.processFile(index)
                        }
                    }
                }
            }
        }
    }

    MessageDialog {
        id: dialog
    }

    Connections {
        target: controller

        function onErrorOccured(error) {
            dialog.error = error
            dialog.open()
        }
    }
}
