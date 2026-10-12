import QtQuick
import QDraw

Rectangle {
    id: root

    implicitWidth: 220
    implicitHeight: 400
    color: theme.surface
    radius: 0
    border.color: "transparent"

    // Clean left border separating panel from canvas
    Rectangle {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 1
        color: theme.border
    }

    Column {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Text {
            text: "PROPERTIES"
            color: theme.textMuted
            font.pixelSize: 10
            font.weight: Font.DemiBold
            font.letterSpacing: 0.8
        }

        Rectangle {
            width: parent.width
            height: 1
            color: theme.border
        }

        Repeater {
            model: controller.activeToolOptions

            delegate: Loader {
                id: optionLoader
                width: parent.width

                required property string optionId
                required property string label
                required property int    type
                required property var    value
                required property real   min
                required property real   max
                required property real   step

                sourceComponent: {
                    switch (type) {
                    case OptionType.Color:
                        return colorPickerComponent
                    case OptionType.Real:
                        return sliderComponent
                    case OptionType.Bool:
                        return toggleComponent
                    default:
                        return null
                    }
                }
            }
        }
    }

    Component {
        id: colorPickerComponent
        ColorPickerButton {
            width: parent ? parent.width : 200
            labelText: parent.label
            selectedColor: parent.value
            onColorSelected: function(newColor) {
                controller.setOption(parent.optionId, newColor)
            }
        }
    }

    Component {
        id: sliderComponent
        LabeledSlider {
            width: parent ? parent.width : 200
            labelText: parent.label
            value: parent.value
            fromValue: parent.min
            toValue: parent.max
            stepSize: parent.step
            onValueModified: function(newVal) {
                controller.setOption(parent.optionId, newVal)
            }
        }
    }

    Component {
        id: toggleComponent
        ToggleSwitch {
            width: parent ? parent.width : 200
            labelText: parent.label
            checked: parent.value
            onToggled: function(newChecked) {
                controller.setOption(parent.optionId, newChecked)
            }
        }
    }
}
