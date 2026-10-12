import QtQuick

Rectangle {
    property color strokeColor: theme.textPrimary

    implicitWidth: 11
    implicitHeight: 11
    width: implicitWidth
    height: implicitHeight
    color: "transparent"
    border.color: strokeColor
    border.width: 1.3
    radius: 5.5
}
