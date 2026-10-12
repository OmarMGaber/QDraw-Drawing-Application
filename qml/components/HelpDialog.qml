import QtQuick

Item {
    id: root

    anchors.fill: parent
    visible: false
    z: 999

    // Props bound to context properties injected by main.cpp (single source of truth)
    property var shortcutData: shortcutList
    property var infoData: appInfo

    function open() {
        visible = true
        dialogCard.forceActiveFocus()
    }

    function close() {
        visible = false
    }

    // Dismiss on Escape key
    Shortcut {
        sequence: "Escape"
        enabled: root.visible
        onActivated: root.close()
    }

    // Dimmed Backdrop
    Rectangle {
        id: backdrop
        anchors.fill: parent
        color: theme.overlay
        opacity: root.visible ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.close()
        }
    }

    // Modal Content Card
    Rectangle {
        id: dialogCard
        width: Math.min(620, root.width - 32)
        height: Math.min(560, root.height - 32)
        anchors.centerIn: parent
        color: theme.surface
        radius: 8
        border.color: theme.border
        border.width: 1

        // Prevent click-through closing
        MouseArea {
            anchors.fill: parent
            onClicked: {}
        }

        Column {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 14

            // Header Row
            Row {
                width: parent.width
                spacing: 10

                Column {
                    width: parent.width - 32
                    spacing: 2

                    Text {
                        text: "Help & Shortcuts"
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        color: theme.textPrimary
                    }

                    Text {
                        text: "Keyboard shortcut reference and project information"
                        font.pixelSize: 11
                        color: theme.textMuted
                    }
                }

                // Close Button (✕)
                Rectangle {
                    width: 26
                    height: 26
                    radius: 4
                    color: closeMouse.containsMouse ? theme.surfaceHover : "transparent"
                    border.color: closeMouse.containsMouse ? theme.borderStrong : "transparent"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: "✕"
                        font.pixelSize: 12
                        color: closeMouse.containsMouse ? theme.textPrimary : theme.textSecondary
                    }

                    MouseArea {
                        id: closeMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.close()
                    }
                }
            }

            // Project & Author Metadata Card
            Rectangle {
                width: parent.width
                height: metaColumn.implicitHeight + 20
                color: theme.surfaceElevated
                radius: 6
                border.color: theme.border
                border.width: 1

                Column {
                    id: metaColumn
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8

                    Row {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: "Author:"
                            font.pixelSize: 11
                            font.weight: Font.DemiBold
                            color: theme.textSecondary
                        }

                        Text {
                            text: root.infoData.author || "OmarMGaber"
                            font.pixelSize: 11
                            font.weight: Font.Bold
                            color: authorMouse.containsMouse ? theme.accentHover : theme.textPrimary

                            MouseArea {
                                id: authorMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var url = root.infoData.authorUrl || "https://github.com/OmarMGaber";
                                    Qt.openUrlExternally(url);
                                }
                            }
                        }

                        Text {
                            text: "•"
                            font.pixelSize: 11
                            color: theme.textSubtle
                        }

                        Text {
                            text: "GitHub Repository:"
                            font.pixelSize: 11
                            font.weight: Font.DemiBold
                            color: theme.textSecondary
                        }

                        Text {
                            text: root.infoData.githubUrl || "https://github.com/OmarMGaber/QDraw"
                            font.pixelSize: 11
                            font.weight: Font.Medium
                            font.underline: repoMouse.containsMouse
                            color: repoMouse.containsMouse ? theme.accentHover : theme.accent
                            elide: Text.ElideRight
                            width: parent.width - 240

                            MouseArea {
                                id: repoMouse
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var url = root.infoData.githubUrl || "https://github.com/OmarMGaber/QDraw";
                                    Qt.openUrlExternally(url);
                                }
                            }
                        }
                    }
                }
            }

            // Scrollable Shortcuts List
            Rectangle {
                width: parent.width
                height: dialogCard.height - 180
                color: theme.surfaceElevated
                radius: 6
                border.color: theme.border
                border.width: 1
                clip: true

                Flickable {
                    id: flick
                    anchors.fill: parent
                    anchors.margins: 10
                    contentWidth: width
                    contentHeight: shortcutColumn.implicitHeight
                    boundsBehavior: Flickable.StopAtBounds

                    Column {
                        id: shortcutColumn
                        width: parent.width
                        spacing: 6

                        Repeater {
                            model: root.shortcutData

                            delegate: Rectangle {
                                width: shortcutColumn.width
                                height: 38
                                radius: 4
                                color: rowHover.containsMouse ? theme.surfaceHover : theme.surfaceElevated
                                border.color: rowHover.containsMouse ? theme.borderStrong : theme.border
                                border.width: 1

                                Behavior on color { ColorAnimation { duration: 80 } }

                                MouseArea {
                                    id: rowHover
                                    anchors.fill: parent
                                    hoverEnabled: true
                                }

                                Row {
                                    anchors.left: parent.left
                                    anchors.leftMargin: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 10

                                    // Category Tag
                                    Rectangle {
                                        height: 18
                                        width: catText.implicitWidth + 10
                                        radius: 3
                                        anchors.verticalCenter: parent.verticalCenter
                                        color: theme.surfaceHover
                                        border.color: theme.border
                                        border.width: 1

                                        Text {
                                            id: catText
                                            anchors.centerIn: parent
                                            text: modelData.category || "Tool"
                                            font.pixelSize: 9
                                            font.weight: Font.DemiBold
                                            color: theme.textSecondary
                                        }
                                    }

                                    // Name & Description
                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        spacing: 1

                                        Text {
                                            text: modelData.name || ""
                                            font.pixelSize: 11
                                            font.weight: Font.Medium
                                            color: theme.textPrimary
                                        }

                                        Text {
                                            text: modelData.description || ""
                                            font.pixelSize: 9
                                            color: theme.textMuted
                                        }
                                    }
                                }

                                // Key Badge Pill
                                Rectangle {
                                    anchors.right: parent.right
                                    anchors.rightMargin: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    height: 22
                                    width: keyBadgeText.implicitWidth + 14
                                    radius: 4
                                    color: theme.surfaceHover
                                    border.color: theme.borderStrong
                                    border.width: 1

                                    Text {
                                        id: keyBadgeText
                                        anchors.centerIn: parent
                                        text: modelData.displayKey || ""
                                        font.pixelSize: 11
                                        font.weight: Font.Bold
                                        color: theme.textPrimary
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Footer Row
            Row {
                width: parent.width
                spacing: 10

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Tip: Press F1 anytime to toggle this dialog"
                    font.pixelSize: 10
                    color: theme.textSubtle
                }

                Item {
                    width: parent.width - 290
                    height: 1
                }

                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    width: 72
                    height: 26
                    radius: 4
                    color: doneMouse.containsMouse ? theme.surfaceHover : theme.surfaceElevated
                    border.color: doneMouse.containsMouse ? theme.borderStrong : theme.border
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: "Close"
                        font.pixelSize: 11
                        font.weight: Font.Medium
                        color: theme.textPrimary
                    }

                    MouseArea {
                        id: doneMouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: root.close()
                    }
                }
            }
        }
    }
}
