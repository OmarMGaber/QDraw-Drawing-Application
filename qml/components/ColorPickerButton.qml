import QtQuick
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
    id: root

    property string labelText: "Color"
    property color  selectedColor: theme.palettePresets && theme.palettePresets.length > 0 ? theme.palettePresets[0] : "black"

    signal colorSelected(color newColor)

    implicitWidth: 190
    implicitHeight: 52

    readonly property var palettePresets: theme.palettePresets || []

    ColorDialog {
        id: colorDialog
        title: "Choose " + root.labelText
        selectedColor: root.selectedColor
        onAccepted: {
            root.colorSelected(colorDialog.selectedColor)
        }
    }

    Column {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            width: parent.width

            Text {
                text: root.labelText
                color: theme.textSecondary
                font.pixelSize: 11
                font.weight: Font.Medium
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 32
                height: 18
                radius: 3
                color: root.selectedColor
                border.color: swatchBtnMouse.containsMouse ? theme.textMuted : theme.borderStrong
                border.width: 1

                MouseArea {
                    id: swatchBtnMouse
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onClicked: colorDialog.open()
                }
            }
        }

        Row {
            spacing: 6
            width: parent.width

            Repeater {
                model: root.palettePresets
                delegate: Rectangle {
                    required property var modelData
                    width: 16
                    height: 16
                    radius: 3
                    color: modelData

                    readonly property bool isCurrent: root.selectedColor === modelData
                    border.color: isCurrent ? theme.textPrimary : (presetMouse.containsMouse ? theme.textSubtle : theme.border)
                    border.width: isCurrent ? 2 : 1

                    MouseArea {
                        id: presetMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.colorSelected(parent.modelData)
                    }
                }
            }
        }
    }
}
