import QtQuick
import QDraw

Rectangle {
    id: root

    implicitWidth: toolRow.implicitWidth + 8
    implicitHeight: 34
    color: theme.surface
    radius: 6
    border.color: theme.border
    border.width: 1

    readonly property var tools: [
        { type: DrawingController.Pen,       name: "Pen",       key: "P" },
        { type: DrawingController.Eraser,    name: "Eraser",    key: "E" },
        { type: DrawingController.Rectangle, name: "Rectangle", key: "R" },
        { type: DrawingController.Ellipse,   name: "Ellipse",   key: "O" },
        { type: DrawingController.Circle,    name: "Circle",    key: "C" },
        { type: DrawingController.Line,      name: "Line",      key: "L" }
    ]

    Row {
        id: toolRow
        anchors.centerIn: parent
        spacing: 2

        Repeater {
            model: root.tools

            delegate: Rectangle {
                id: toolBtn
                required property var modelData

                implicitWidth: contentRow.implicitWidth + 16
                implicitHeight: 26
                radius: 4

                readonly property bool isSelected: controller.activeTool === modelData.type
                readonly property color activeColor: isSelected ? theme.textPrimary : (btnMouse.containsMouse ? theme.textPrimary : theme.textSecondary)

                color: isSelected ? theme.surfaceHover : (btnMouse.containsMouse ? theme.surfaceElevated : "transparent")
                border.color: isSelected ? theme.borderStrong : "transparent"
                border.width: 1

                Behavior on color { ColorAnimation { duration: 100 } }

                Row {
                    id: contentRow
                    anchors.centerIn: parent
                    spacing: 6

                    // Clean Vector Icon (No emojis)
                    Item {
                        width: 14
                        height: 14
                        anchors.verticalCenter: parent.verticalCenter

                        PenIcon {
                            anchors.fill: parent
                            visible: toolBtn.modelData.type === DrawingController.Pen
                            strokeColor: toolBtn.activeColor
                        }

                        EraserIcon {
                            anchors.fill: parent
                            visible: toolBtn.modelData.type === DrawingController.Eraser
                            strokeColor: toolBtn.activeColor
                        }

                        RectangleIcon {
                            anchors.centerIn: parent
                            visible: toolBtn.modelData.type === DrawingController.Rectangle
                            strokeColor: toolBtn.activeColor
                        }

                        EllipseIcon {
                            anchors.centerIn: parent
                            visible: toolBtn.modelData.type === DrawingController.Ellipse
                            strokeColor: toolBtn.activeColor
                        }

                        CircleIcon {
                            anchors.centerIn: parent
                            visible: toolBtn.modelData.type === DrawingController.Circle
                            strokeColor: toolBtn.activeColor
                        }

                        LineIcon {
                            anchors.fill: parent
                            visible: toolBtn.modelData.type === DrawingController.Line
                            strokeColor: toolBtn.activeColor
                        }
                    }

                    Text {
                        text: toolBtn.modelData.name
                        font.pixelSize: 11
                        font.weight: toolBtn.isSelected ? Font.Medium : Font.Normal
                        color: toolBtn.activeColor
                    }

                    Text {
                        text: toolBtn.modelData.key
                        font.pixelSize: 9
                        font.weight: Font.Normal
                        color: toolBtn.isSelected ? theme.textSecondary : theme.textSubtle
                    }
                }

                MouseArea {
                    id: btnMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: controller.selectTool(toolBtn.modelData.type)
                }
            }
        }
    }
}
