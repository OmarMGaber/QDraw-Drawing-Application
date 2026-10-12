# Canvas Subsystem

## What
The Canvas Subsystem integrates the C++ rendering engine with Qt Quick via `QQuickPaintedItem`, handling viewport repainting and converting raw pointer and hover events into clean signals.

## Why
- **Signals over direct controller coupling:** If `DrawingCanvas` held concrete dependencies on tools or history logic, testing the rendering item in isolation would require mocking the entire application state. By emitting standard Qt signals (`pointerPressed`, `pointerMoved`, `pointerReleased`, `pointerHovered`), the canvas remains a pure input/render view. The controller subscribes to these signals, achieving clean inversion of control and loose coupling.
- **Hover events for the eraser cursor:** Standard mouse move events only fire while a mouse button is actively held down. To render an intuitive circular cursor indicator preview before the user clicks to erase, hover events (`setAcceptHoverEvents(true)`) are required. This allows `EraserTool` to track pointer coordinates smoothly during passive movement without triggering accidental object deletions.

## How
- `DrawingCanvas` subclasses `QQuickPaintedItem` and overrides `paint(QPainter*)`.
- The paint sequence executes in three distinct layers:
  1. Solid canvas background fill (`Qt::white`).
  2. Committed objects iterated from `DrawingDocument::objects()` via `DrawingController::document()`.
  3. Interactive gesture preview rendered via `activeTool()->paintPreview()`.
- When the document changes or tool previews update, `DrawingController` emits `repaintNeeded()`, causing the canvas to schedule an asynchronous paint pass via `update()`.

## Architecture Diagram

```mermaid
classDiagram
    class QQuickPaintedItem
    class DrawingCanvas {
        -DrawingController* m_controller
        +controller() DrawingController*
        +setController(DrawingController* controller) void
        +paint(QPainter* painter) void
        #mousePressEvent(QMouseEvent* e) void
        #mouseMoveEvent(QMouseEvent* e) void
        #mouseReleaseEvent(QMouseEvent* e) void
        #hoverMoveEvent(QHoverEvent* e) void
        +pointerPressed(QPointF pos)
        +pointerMoved(QPointF pos)
        +pointerReleased(QPointF pos)
        +pointerHovered(QPointF pos)
        +controllerChanged()
    }

    class DrawingController {
        +document() DrawingDocument*
        +activeTool() Tool*
        +onPointerPressed(QPointF) void
        +onPointerMoved(QPointF) void
        +onPointerReleased(QPointF) void
        +onPointerHovered(QPointF) void
        +repaintNeeded()
    }

    QQuickPaintedItem <|-- DrawingCanvas
    DrawingCanvas o--> DrawingController : receives repaintNeeded() & dispatches signals
```

### Rendering & Event Flow

```mermaid
graph TD
    classDef item fill:#1e293b,stroke:#3b82f6,stroke-width:2px,color:#f8fafc;
    classDef flow fill:#0f172a,stroke:#64748b,stroke-width:1px,color:#94a3b8;
    classDef ctrl fill:#1d4ed8,stroke:#60a5fa,stroke-width:2px,color:#ffffff;

    subgraph InputProcessing["Input Event Pipeline"]
        Mouse["Mouse & Hover Events\n(Press, Move, Release, Hover)"]:::flow
        CanvasItem["DrawingCanvas\n(QQuickPaintedItem)"]:::item
        Signals["Qt Signals\n(pointerPressed, pointerMoved, etc.)"]:::flow
    end

    Controller["DrawingController"]:::ctrl

    subgraph PaintPipeline["DrawingCanvas::paint() 3-Layer Pipeline"]
        L1["Layer 1: Background Fill\npainter->fillRect(boundingRect, Qt::white)"]:::flow
        L2["Layer 2: Committed Geometry\nfor obj in document->objects(): obj->paint()"]:::flow
        L3["Layer 3: Gesture Preview\nactiveTool->paintPreview(painter)"]:::flow
    end

    Mouse --> CanvasItem
    CanvasItem --> Signals
    Signals --> Controller
    Controller -->|"repaintNeeded() -> update()"| CanvasItem
    CanvasItem --> L1
    L1 --> L2
    L2 --> L3
```

## Files

| File | Role |
|---|---|
| `DrawingCanvas.h` / `.cpp` | `QQuickPaintedItem` providing viewport rendering, hover tracking, and input event signals. |

## Boundaries
- This subsystem does **NOT** interpret what gestures mean; clicking or dragging is forwarded blindly as coordinates.
- This subsystem does **NOT** own drawing objects, undo stacks, or tool state.
