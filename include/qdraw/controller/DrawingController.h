#pragma once

#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/tools/Tool.h>
#include <qdraw/tools/PenTool.h>
#include <qdraw/tools/EraserTool.h>
#include <qdraw/tools/RectangleTool.h>
#include <qdraw/tools/EllipseTool.h>
#include <qdraw/tools/CircleTool.h>
#include <qdraw/tools/LineTool.h>
#include <qdraw/tools/options/ToolOptionModel.h>
#include <QObject>
#include <QUndoStack>
#include <QString>
#include <QVariant>
#include <QPointF>
#include <memory>

class DrawingCanvas;

/// Central application controller exposed to QML.
///
/// Owns the tools, drawing document, undo stack, and option model.
/// Routes canvas pointer signals to the currently active tool.
/// Coordinates repainting triggers when document geometry or preview state changes.
class DrawingController : public QObject {
    Q_OBJECT

public:
    enum ToolType { Pen, Eraser, Rectangle, Ellipse, Circle, Line };
    Q_ENUM(ToolType)

    Q_PROPERTY(ToolType         activeTool        READ activeToolType   NOTIFY activeToolChanged)
    Q_PROPERTY(ToolOptionModel* activeToolOptions READ activeToolOptions NOTIFY activeToolChanged)
    Q_PROPERTY(bool             canUndo           READ canUndo          NOTIFY historyChanged)
    Q_PROPERTY(bool             canRedo           READ canRedo          NOTIFY historyChanged)

    explicit DrawingController(QObject* parent = nullptr);
    ~DrawingController() override = default;

    ToolType activeToolType() const { return m_activeToolType; }
    ToolOptionModel* activeToolOptions() { return &m_optionModel; }
    bool canUndo() const { return m_undoStack.canUndo(); }
    bool canRedo() const { return m_undoStack.canRedo(); }

    /// Returns non-owning pointer to the document.
    const DrawingDocument* document() const { return &m_document; }

    /// Returns non-owning pointer to the active tool.
    Tool* activeTool() const { return m_activeTool; }

public slots:
    /// Activates a new tool, updating tool options and signal subscriptions.
    Q_INVOKABLE void selectTool(ToolType tool);

    /// Updates a configuration option on the currently active tool.
    Q_INVOKABLE void setOption(const QString& id, const QVariant& value);

    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();

    /// Wires canvas signals and establishes repaint triggers.
    void setCanvas(DrawingCanvas* canvas);

    void onPointerPressed(QPointF pos);
    void onPointerMoved(QPointF pos);
    void onPointerReleased(QPointF pos);
    void onPointerHovered(QPointF pos);
    void onDocumentChanged();
    void onPreviewChanged();

signals:
    void activeToolChanged();
    void historyChanged();
    void repaintNeeded();

private:
    void wireActiveTool();

    std::unique_ptr<PenTool>       m_penTool;
    std::unique_ptr<EraserTool>    m_eraserTool;
    std::unique_ptr<RectangleTool> m_rectangleTool;
    std::unique_ptr<EllipseTool>   m_ellipseTool;
    std::unique_ptr<CircleTool>    m_circleTool;
    std::unique_ptr<LineTool>      m_lineTool;

    Tool*              m_activeTool = nullptr;
    ToolType           m_activeToolType = Pen;

    DrawingDocument    m_document;
    QUndoStack         m_undoStack;
    ToolOptionModel    m_optionModel;
};
