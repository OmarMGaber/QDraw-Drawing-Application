import QtQuick
import QDraw

Rectangle {
    id: root

    implicitWidth: 800
    implicitHeight: 46
    color: theme.surface
    border.color: theme.border
    border.width: 1

    signal helpRequested()

    // Left Branding & Help
    Row {
        anchors.left: parent.left
        anchors.leftMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        spacing: 8

        Image {
            id: brandLogo
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            source: (appInfo && appInfo.icon) ? ("qrc:/qt/qml/QDraw/" + appInfo.icon) : ""
            fillMode: Image.PreserveAspectFit
            smooth: true
            mipmap: true
            visible: status === Image.Ready
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: (appInfo && appInfo.name) ? appInfo.name : "QDraw"
            font.pixelSize: 13
            font.weight: Font.DemiBold
            color: theme.textPrimary
        }

        Rectangle {
            width: 1
            height: 16
            anchors.verticalCenter: parent.verticalCenter
            color: theme.border
        }

        // Help Button
        Rectangle {
            id: helpBtn
            anchors.verticalCenter: parent.verticalCenter
            width: 66
            height: 26
            radius: 4
            color: helpMouse.containsMouse ? theme.surfaceHover : theme.surfaceElevated
            border.color: helpMouse.containsMouse ? theme.borderStrong : theme.border
            border.width: 1

            Behavior on color { ColorAnimation { duration: 100 } }

            Row {
                anchors.centerIn: parent
                spacing: 5

                HelpIcon {
                    anchors.verticalCenter: parent.verticalCenter
                    strokeColor: helpMouse.containsMouse ? theme.textPrimary : theme.textSecondary
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Help"
                    font.pixelSize: 11
                    font.weight: Font.Medium
                    color: helpMouse.containsMouse ? theme.textPrimary : theme.textSecondary
                }
            }

            MouseArea {
                id: helpMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.helpRequested()
            }
        }
    }

    // Center Tool Selector
    ToolSelector {
        id: centerTools
        anchors.centerIn: parent
    }

    // Right History Controls
    Row {
        anchors.right: parent.right
        anchors.rightMargin: 16
        anchors.verticalCenter: parent.verticalCenter
        spacing: 6

        // Undo Button
        Rectangle {
            id: undoBtn
            width: 76
            height: 28
            radius: 4
            color: undoMouse.containsMouse && controller.canUndo ? theme.surfaceHover : (controller.canUndo ? theme.surfaceElevated : theme.background)
            border.color: controller.canUndo ? theme.borderStrong : theme.border
            border.width: 1
            opacity: controller.canUndo ? 1.0 : 0.4

            Behavior on color { ColorAnimation { duration: 100 } }

            Row {
                anchors.centerIn: parent
                spacing: 5

                // Vector Undo Arrow (No emoji)
                UndoIcon {
                    anchors.verticalCenter: parent.verticalCenter
                    strokeColor: controller.canUndo ? theme.textPrimary : theme.textMuted
                }

                Text {
                    text: "Undo"
                    font.pixelSize: 11
                    font.weight: Font.Medium
                    color: controller.canUndo ? theme.textPrimary : theme.textMuted
                }
            }

            MouseArea {
                id: undoMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: controller.canUndo ? Qt.PointingHandCursor : Qt.ArrowCursor
                enabled: controller.canUndo
                onClicked: controller.undo()
            }
        }

        // Redo Button
        Rectangle {
            id: redoBtn
            width: 76
            height: 28
            radius: 4
            color: redoMouse.containsMouse && controller.canRedo ? theme.surfaceHover : (controller.canRedo ? theme.surfaceElevated : theme.background)
            border.color: controller.canRedo ? theme.borderStrong : theme.border
            border.width: 1
            opacity: controller.canRedo ? 1.0 : 0.4

            Behavior on color { ColorAnimation { duration: 100 } }

            Row {
                anchors.centerIn: parent
                spacing: 5

                // Vector Redo Arrow (No emoji)
                RedoIcon {
                    anchors.verticalCenter: parent.verticalCenter
                    strokeColor: controller.canRedo ? theme.textPrimary : theme.textMuted
                }

                Text {
                    text: "Redo"
                    font.pixelSize: 11
                    font.weight: Font.Medium
                    color: controller.canRedo ? theme.textPrimary : theme.textMuted
                }
            }

            MouseArea {
                id: redoMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: controller.canRedo ? Qt.PointingHandCursor : Qt.ArrowCursor
                enabled: controller.canRedo
                onClicked: controller.redo()
            }
        }
    }
}
