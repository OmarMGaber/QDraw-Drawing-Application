# QML Presentation Layer

## What
The QML Presentation Layer defines the declarative graphical interface, window hierarchy, responsive layout, and user input bindings for QDraw.

## Why
- **Strict presentation-only boundary:** QML is designed for declarative UI composition, layouts, styling, and animations. Allowing QML to directly construct geometry, maintain undo lists, or handle object memory leaks business logic into the view layer. In QDraw, QML communicates with C++ solely through `DrawingController` via `Q_PROPERTY` reads and `Q_INVOKABLE` method invocations.
- **What QML must NEVER do:**
  - Never instantiate or delete `DrawingObject` instances.
  - Never execute geometry calculations or hit-testing algorithms.
  - Never store drawing history stacks or manipulate undo/redo commands.
  - Never hardcode tool-specific property fields in panels.

## How
- `Main.qml` configures the application window, top bar, canvas container, and floating panels.
- Global keyboard shortcuts (`Ctrl+Z`, `Ctrl+Y`, tool selection keys `P`, `E`, `R`, `O`, `C`, `L`) route directly to `controller.undo()`, `controller.redo()`, and `controller.selectTool()`.
- The canvas item `DrawingCanvas` is embedded as a `QQuickPaintedItem` in the central workspace.

## Presentation Architecture

```mermaid
graph TD
    classDef window fill:#090d16,stroke:#334155,stroke-width:2px,color:#f8fafc;
    classDef container fill:#0f172a,stroke:#3b82f6,stroke-width:2px,color:#f8fafc;
    classDef float fill:#1e293b,stroke:#64748b,stroke-width:1px,color:#f8fafc;
    classDef backend fill:#1d4ed8,stroke:#60a5fa,stroke-width:2px,color:#ffffff;

    subgraph QMLPresentation["QML Presentation Layer (Main.qml)"]
        WindowItem["Window (id: window)"]:::window
        Shortcuts["Keyboard Shortcuts\n(Ctrl+Z, Ctrl+Y, P, E, R, O, C, L)"]:::window
        TopBarItem["TopBar (Header, z: 10)"]:::float
        CanvasBox["Rectangle (canvasContainer)"]:::container
        CanvasItem["DrawingCanvas (QQuickPaintedItem)"]:::container
        ToolBarItem["ToolSelector (Floating Pill, z: 20)"]:::float
        PropsItem["PropertiesPanel (Floating Panel, z: 20)"]:::float
    end

    Controller["DrawingController (C++ Context Property)"]:::backend

    WindowItem --> Shortcuts
    WindowItem --> TopBarItem
    WindowItem --> CanvasBox
    CanvasBox --> CanvasItem
    WindowItem --> ToolBarItem
    WindowItem --> PropsItem

    Shortcuts -->|"undo(), redo(), selectTool()"| Controller
    TopBarItem -->|"canUndo, canRedo / undo(), redo()"| Controller
    ToolBarItem -->|"activeTool / selectTool()"| Controller
    PropsItem -->|"activeToolOptions / setOption()"| Controller
    CanvasItem -->|"pointerPressed, moved, released, hovered"| Controller
    Controller -->|"repaintNeeded() -> update()"| CanvasItem
```

## Files

| File | Role |
|---|---|
| `Main.qml` | Top-level window hosting layout structure, canvas view, toolbars, and global shortcuts. |
| `components/` | Subdirectory containing modular UI components (top bar, tool selector, properties panel, delegates). |
| `icons/` | Subdirectory containing standalone vector and canvas-based icon components (`PenIcon`, `EraserIcon`, `LineIcon`, `UndoIcon`, `RedoIcon`, etc.). |

## Boundaries
- QML does **NOT** perform raster painting or Catmull-Rom spline calculations; all rendering is performed by `QPainter` in C++.
- QML does **NOT** own application models or tool registries.
