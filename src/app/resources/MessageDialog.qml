import QtQuick
import QtQuick.Layouts

Item {
    id: rootItem

    property string error

    function open() {
        loader.active = true
    }

    function close() {
        loader.active = false
    }

    anchors.fill: parent

    Loader {
        id: loader
        anchors.fill: parent
        active: false

        sourceComponent: Item {
            id: overlay

            MouseArea {
                anchors.fill: parent
                preventStealing: true
            }

            Rectangle {
                id: popup
                anchors.centerIn: parent

                width: overlay.width / 2
                height: content.height + 32
                color: "#535353"

                ColumnLayout {
                    id: content
                    anchors.centerIn: parent
                    spacing: 8

                    Text {
                        id: errorText
                        Layout.alignment: Qt.AlignHCenter
                        Layout.maximumWidth: popup.width - 32
                        wrapMode: Text.Wrap
                        text: rootItem.error
                        color: "white"
                    }

                    Rectangle {
                        id: button
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: buttonText.width + 64
                        Layout.preferredHeight: buttonText.height + 8

                        Text {
                            id: buttonText
                            text: "Ok"
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: rootItem.close()
                        }

                        states: [
                            State {
                                when: mouseArea.pressed
                                PropertyChanges {
                                    target: button
                                    color: "#c4c4c4"
                                }
                            },
                            State {
                                when: mouseArea.containsMouse
                                PropertyChanges {
                                    target: button
                                    color: "#f2f2f2"
                                }
                            }
                        ]
                    }
                }
            }
        }
    }
}
