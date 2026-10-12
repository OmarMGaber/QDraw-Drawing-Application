import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property string labelText: "Enable"
    property bool   checked: false

    signal toggled(bool newValue)

    implicitWidth: 190
    implicitHeight: 28

    RowLayout {
        anchors.fill: parent

        Text {
            text: root.labelText
            color: theme.textSecondary
            font.pixelSize: 11
            font.weight: Font.Medium
        }

        Item { Layout.fillWidth: true }

        Rectangle {
            id: switchTrack
            width: 32
            height: 18
            radius: 9
            color: root.checked ? theme.controlActive : theme.surfaceHover
            border.color: root.checked ? theme.textPrimary : theme.borderStrong
            border.width: 1

            Behavior on color { ColorAnimation { duration: 100 } }

            Rectangle {
                id: switchThumb
                width: 12
                height: 12
                radius: 6
                y: 2
                x: root.checked ? 17 : 2
                color: root.checked ? theme.surface : theme.textMuted

                Behavior on x { NumberAnimation { duration: 100; easing.type: Easing.OutQuad } }
                Behavior on color { ColorAnimation { duration: 100 } }
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: root.toggled(!root.checked)
            }
        }
    }
}
