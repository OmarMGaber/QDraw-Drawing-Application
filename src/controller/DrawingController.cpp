#include <qdraw/controller/DrawingController.h>
#include <qdraw/canvas/DrawingCanvas.h>

DrawingController::DrawingController(QObject* parent)
    : QObject(parent)
    , m_penTool(std::make_unique<PenTool>(this))
    , m_eraserTool(std::make_unique<EraserTool>(this))
    , m_rectangleTool(std::make_unique<RectangleTool>(this))
    , m_ellipseTool(std::make_unique<EllipseTool>(this))
    , m_circleTool(std::make_unique<CircleTool>(this))
    , m_lineTool(std::make_unique<LineTool>(this)) {
    // Provide document and undo stack context to all tools
    m_penTool->setContext(&m_document, &m_undoStack);
    m_eraserTool->setContext(&m_document, &m_undoStack);
    m_rectangleTool->setContext(&m_document, &m_undoStack);
    m_ellipseTool->setContext(&m_document, &m_undoStack);
    m_circleTool->setContext(&m_document, &m_undoStack);
    m_lineTool->setContext(&m_document, &m_undoStack);

    connect(&m_document, &DrawingDocument::changed, this, &DrawingController::onDocumentChanged);
    connect(&m_undoStack, &QUndoStack::canUndoChanged, this, &DrawingController::historyChanged);
    connect(&m_undoStack, &QUndoStack::canRedoChanged, this, &DrawingController::historyChanged);

    selectTool(Pen);
}

void DrawingController::selectTool(ToolType tool) {
    if (m_activeTool) {
        m_activeTool->deactivate();
        disconnect(m_activeTool, &Tool::previewChanged, this, &DrawingController::onPreviewChanged);
    }

    m_activeToolType = tool;

    switch (tool) {
    case Pen:
        m_activeTool = m_penTool.get();
        break;
    case Eraser:
        m_activeTool = m_eraserTool.get();
        break;
    case Rectangle:
        m_activeTool = m_rectangleTool.get();
        break;
    case Ellipse:
        m_activeTool = m_ellipseTool.get();
        break;
    case Circle:
        m_activeTool = m_circleTool.get();
        break;
    case Line:
        m_activeTool = m_lineTool.get();
        break;
    }

    wireActiveTool();
}

void DrawingController::wireActiveTool() {
    if (!m_activeTool) {
        return;
    }

    connect(m_activeTool, &Tool::previewChanged, this, &DrawingController::onPreviewChanged);
    m_optionModel.populate(m_activeTool->options());
    m_activeTool->activate();

    emit activeToolChanged();
    emit repaintNeeded();
}

void DrawingController::setOption(const QString& id, const QVariant& value) {
    if (m_activeTool) {
        m_activeTool->setOptionValue(id, value);
    }
}

void DrawingController::undo() {
    if (m_undoStack.canUndo()) {
        m_undoStack.undo();
    }
}

void DrawingController::redo() {
    if (m_undoStack.canRedo()) {
        m_undoStack.redo();
    }
}

void DrawingController::setCanvas(DrawingCanvas* canvas) {
    if (canvas) {
        canvas->setController(this);
    }
}

void DrawingController::onPointerPressed(QPointF pos) {
    if (m_activeTool) {
        m_activeTool->pointerPress(pos);
    }
}

void DrawingController::onPointerMoved(QPointF pos) {
    if (m_activeTool) {
        m_activeTool->pointerMove(pos);
    }
}

void DrawingController::onPointerReleased(QPointF pos) {
    if (m_activeTool) {
        m_activeTool->pointerRelease(pos);
    }
}

void DrawingController::onPointerHovered(QPointF pos) {
    if (m_activeTool) {
        m_activeTool->pointerHover(pos);
    }
}

void DrawingController::onDocumentChanged() {
    emit repaintNeeded();
    emit historyChanged();
}

void DrawingController::onPreviewChanged() {
    emit repaintNeeded();
}
