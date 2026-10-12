# Controller Subsystem

## What
The Controller Subsystem orchestrates the application backend, providing the single unified interface between Qt Quick (QML) and C++ business logic.

## Why
- **Single activation entry point (`selectTool`):** Routing tool changes through a single method guarantees that tool deactivation, lifecycle cleanup, signal disconnection, model repopulation, and repaint requests happen atomically. If QML or shortcuts could instantiate or swap tools arbitrarily, state synchronization between the canvas, properties panel, and tools would become fragmented.
- **Centralized signal routing and wiring:** `DrawingController` acts as a coordinator. It connects canvas input signals to active tool methods, document change signals to canvas repaints, and tool option modifications to QML models. Neither the canvas nor the tools need direct knowledge of one another.

## How
- `DrawingController` owns all concrete tools as persistent `std::unique_ptr` instances (preserving their configured options across switches), along with the `DrawingDocument`, `QUndoStack`, and `ToolOptionModel`.
- Exposed properties (`activeTool`, `activeToolOptions`, `canUndo`, `canRedo`) allow QML views to update reactively.
- When `selectTool(tool)` is invoked:
  1. The outgoing tool is deactivated and its `previewChanged()` signal disconnected.
  2. The active tool pointer shifts to the chosen tool.
  3. `ToolOptionModel::populate()` repopulates available options for that tool.
  4. The incoming tool is activated and wired to trigger canvas repaints on preview updates.
  5. `activeToolChanged()` and `repaintNeeded()` are emitted.

## Class Architecture

```mermaid
classDiagram
    class QObject
    class DrawingController {
        -ToolType m_activeToolType
        -Tool* m_activeTool
        -DrawingDocument m_document
        -QUndoStack m_undoStack
        -ToolOptionModel m_optionModel
        -unique_ptr~PenTool~ m_penTool
        -unique_ptr~EraserTool~ m_eraserTool
        -unique_ptr~RectangleTool~ m_rectangleTool
        -unique_ptr~EllipseTool~ m_ellipseTool
        -unique_ptr~CircleTool~ m_circleTool
        -unique_ptr~LineTool~ m_lineTool
        +activeToolType() ToolType
        +activeToolOptions() ToolOptionModel*
        +canUndo() bool
        +canRedo() bool
        +document() DrawingDocument*
        +activeTool() Tool*
        +selectTool(ToolType tool) void
        +setOption(QString id, QVariant value) void
        +undo() void
        +redo() void
        +setCanvas(DrawingCanvas* canvas) void
        +onPointerPressed(QPointF pos) void
        +onPointerMoved(QPointF pos) void
        +onPointerReleased(QPointF pos) void
        +onPointerHovered(QPointF pos) void
    }

    class DrawingDocument {
        -vector~unique_ptr~DrawingObject~~ m_objects
        +add(unique_ptr~DrawingObject~) void
        +removeAt(int index) unique_ptr~DrawingObject~
        +objects() QVector~DrawingObject*~
        +changed() void
    }

    class QUndoStack {
        +push(QUndoCommand*) void
        +undo() void
        +redo() void
        +canUndo() bool
        +canRedo() bool
    }

    class ToolOptionModel {
        -QVector~ToolOption*~ m_options
        +populate(QVector~ToolOption*~) void
        +rowCount() int
        +data() QVariant
    }

    class Tool {
        <<abstract>>
        +setContext(DrawingDocument*, QUndoStack*) void
        +options() QVector~ToolOption*~
        +setOptionValue(QString, QVariant) void
        +paintPreview(QPainter&) void
        +pointerPress(QPointF) void
        +pointerMove(QPointF) void
        +pointerRelease(QPointF) void
        +pointerHover(QPointF) void
    }

    QObject <|-- DrawingController
    DrawingController *-- DrawingDocument : owns
    DrawingController *-- QUndoStack : owns
    DrawingController *-- ToolOptionModel : owns
    DrawingController *-- Tool : owns concrete instances
    DrawingController o--> Tool : routes gestures to activeTool
```

## Files

| File | Role |
|---|---|
| `DrawingController.h` / `.cpp` | Central coordinator exposing QML-invokable methods, owning backend state, and routing canvas signals. |

## Boundaries
- The controller does **NOT** perform low-level raster painting; that belongs in `DrawingCanvas` and `DrawingObject`.
- The controller does **NOT** render UI controls or manage window layouts; that belongs in QML.
