import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: root

    property string labelText: "Option"
    property real   value: 0.0
    property real   fromValue: 0.0
    property real   toValue: 100.0
    property real   stepSize: 1.0

    signal valueModified(real newValue)

    implicitWidth: 190
    implicitHeight: 40

    Column {
        anchors.fill: parent
        spacing: 4

        RowLayout {
            width: parent.width

            Text {
                text: root.labelText
                color: theme.textSecondary
                font.pixelSize: 11
                font.weight: Font.Medium
            }

            Item { Layout.fillWidth: true }

            Text {
                text: root.stepSize < 0.1 ? root.value.toFixed(2) : (root.stepSize < 1.0 ? root.value.toFixed(1) : Math.round(root.value).toString())
                color: theme.textPrimary
                font.pixelSize: 11
            }
        }

        Slider {
            id: slider
            width: parent.width
            height: 18
            from: root.fromValue
            to: root.toValue
            stepSize: root.stepSize
            value: root.value

            onMoved: {
                root.valueModified(slider.value)
            }

            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 190
                implicitHeight: 3
                width: slider.availableWidth
                height: 3
                radius: 1.5
                color: theme.surfaceHover

                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    color: theme.textSubtle
                    radius: 1.5
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 12
                implicitHeight: 12
                radius: 6
                color: slider.pressed ? theme.controlActive : (slider.hovered ? theme.textPrimary : theme.controlActive)
                border.color: theme.borderStrong
                border.width: 1
            }
        }
    }
}
