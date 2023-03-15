import QtQuick

Window {
    width: 640
    height: 480
    minimumWidth: 640
    minimumHeight: 480
    visible: true
    title: qsTr("PocketBook")

    MainScreen {
        anchors.fill: parent
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
