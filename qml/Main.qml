import QtQuick
import QtQuick.Window
import QtQml
import QDraw
import "components"

Window {
    id: window
    width: 1100
    height: 750
    minimumWidth: 800
    minimumHeight: 550
    visible: true
    title: "QDraw"
    color: theme.background

    readonly property var rootController: controller
    property real appScale: 1.0

    function triggerAction(actionId) {
        switch (actionId) {
        case "undo":
            controller.undo()
            break
        case "redo":
            controller.redo()
            break
        case "tool_pen":
            controller.selectTool(DrawingController.Pen)
            break
        case "tool_eraser":
            controller.selectTool(DrawingController.Eraser)
            break
        case "tool_rectangle":
            controller.selectTool(DrawingController.Rectangle)
            break
        case "tool_ellipse":
            controller.selectTool(DrawingController.Ellipse)
            break
        case "tool_circle":
            controller.selectTool(DrawingController.Circle)
            break
        case "tool_line":
            controller.selectTool(DrawingController.Line)
            break
        case "zoom_in":
            window.appScale = Math.min(+(window.appScale + 0.1).toFixed(1), 3.0)
            break
        case "zoom_out":
            window.appScale = Math.max(+(window.appScale - 0.1).toFixed(1), 0.5)
            break
        case "zoom_reset":
            window.appScale = 1.0
            break
        case "help":
            helpDialog.open()
            break
        }
    }

    // Shortcuts driven directly from config/app.json (single source of truth)
    // shortcutList is guaranteed to be populated — app won't start without it.
    Instantiator {
        id: shortcutInstantiator
        model: shortcutList
        delegate: Shortcut {
            sequences: modelData.sequences
            context: Qt.ApplicationShortcut
            onActivated: window.triggerAction(modelData.id)
        }
    }

    Item {
        id: appRoot
        width: window.width / window.appScale
        height: window.height / window.appScale
        scale: window.appScale
        transformOrigin: Item.TopLeft

        // Top Header Bar with integrated ToolSelector & Help button
        TopBar {
            id: topBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 46
            z: 10
            onHelpRequested: helpDialog.open()
        }

        // Docked Properties Panel
        PropertiesPanel {
            id: propertiesPanel
            anchors.top: topBar.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 220
            z: 10
        }

        // Canvas Workspace
        Rectangle {
            id: canvasContainer
            anchors.top: topBar.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: propertiesPanel.left
            color: theme.background

            // Drawing Canvas
            DrawingCanvas {
                id: canvas
                anchors.fill: parent
                controller: window.rootController
            }
        }

        // Help & Shortcuts Modal Dialog
        HelpDialog {
            id: helpDialog
            z: 100
        }
    }
}

