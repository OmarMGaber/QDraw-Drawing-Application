# QML Icons Subsystem

## What
The QML Icons Subsystem provides standalone, reusable vector and canvas-based icon components for application toolbars and controls.

## Why
- **Encapsulated rendering:** Canvas-based rendering logic, coordinate geometry, and stroke configurations are self-contained.
- **Reactive color binding:** Each icon exposes a `property color strokeColor`. Canvas icons automatically call `requestPaint()` when `strokeColor` changes without requiring external `Connections` objects.
- **Clean composition:** Host components (such as `ToolSelector` and `TopBar`) instantiate icons declaratively without cluttering layout definitions with imperative 2D canvas drawing code.

## Files

| File | Type | Role |
|---|---|---|
| `PenIcon.qml` | `Canvas` | Vector pen nib icon for the freehand pen tool. |
| `EraserIcon.qml` | `Canvas` | Geometric eraser block icon with divider stroke. |
| `LineIcon.qml` | `Canvas` | Clean diagonal stroke icon for the line tool. |
| `UndoIcon.qml` | `Canvas` | Curved circular arc with arrow head for undo action. |
| `RedoIcon.qml` | `Canvas` | Curved circular arc with arrow head for redo action. |
| `RectangleIcon.qml` | `Rectangle` | 1px border outline for rectangle tool. |
| `EllipseIcon.qml` | `Rectangle` | 1px border ellipse for ellipse tool. |
| `CircleIcon.qml` | `Rectangle` | Proportional circular border icon for circle tool. |
| `HelpIcon.qml` | `Canvas` | Vector question mark icon for help & shortcut reference dialog. |
