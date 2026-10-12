import QtQuick

Rectangle {
    property color strokeColor: theme.textPrimary

    implicitWidth: 13
    implicitHeight: 9
    width: implicitWidth
    height: implicitHeight
    color: "transparent"
    border.color: strokeColor
    border.width: 1.3
    radius: 4.5
}
