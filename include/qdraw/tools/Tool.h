#pragma once

#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/tools/options/ToolOption.h>
#include <QColor>
#include <QHash>
#include <QObject>
#include <QPainter>
#include <QPointF>
#include <QUndoStack>
#include <QVector>
#include <memory>
#include <vector>

/// Abstract base for all drawing tools.
///
/// Owns the option registry declared in subclass constructors via registerOption().
/// The registry order is the display order in PropertiesPanel.
///
/// Each tool permanently retains its own option values across tool switches.
/// Receives DrawingDocument and QUndoStack pointers from DrawingController
/// for object hit-testing and command commits.
class Tool : public QObject {
    Q_OBJECT
public:
    explicit Tool(QObject* parent = nullptr);
    ~Tool() override = default;

    /// Set execution context. Tool does not own either pointer.
    void setContext(DrawingDocument* document, QUndoStack* undoStack);

    /// All registered options in declaration order. Used by DrawingController
    /// to populate ToolOptionModel on tool activation.
    QVector<ToolOption*> options() const;

    /// Set an option value by id. Called by DrawingController on behalf of QML.
    void setOptionValue(const QString& id, const QVariant& value);

    /// Paint the in-progress gesture preview.
    /// Called by DrawingCanvas::paint() after all committed objects.
    /// Default implementation is a no-op — override only when the tool has a preview.
    virtual void paintPreview(QPainter& painter) const { Q_UNUSED(painter); }

    /// Called when this tool becomes active. Override for state/cursor setup.
    virtual void activate() {}

    /// Called when this tool is deactivated. Override for preview/state cleanup.
    virtual void deactivate() {}

    virtual void pointerPress(QPointF pos) = 0;
    virtual void pointerMove(QPointF pos) = 0;
    virtual void pointerRelease(QPointF pos) = 0;

    /// Called on hover movement (no button pressed). EraserTool overrides to update
    /// the cursor circle without accumulating gesture points.
    virtual void pointerHover(QPointF pos) { Q_UNUSED(pos); }

signals:
    /// Emitted inside pointerMove/pointerHover when the preview visual changes.
    /// DrawingController connects this to repaintNeeded() -> DrawingCanvas::update().
    void previewChanged();

    /// Emitted after setOptionValue().
    void optionValueChanged(const QString& id);

protected:
    void registerOption(std::unique_ptr<ToolOption> opt);

    QColor colorOption(const QString& id) const;
    qreal  realOption(const QString& id) const;
    bool   boolOption(const QString& id) const;

    DrawingDocument* document() const { return m_document; }
    QUndoStack*      undoStack() const { return m_undoStack; }

private:
    std::vector<std::unique_ptr<ToolOption>> m_options;
    QHash<QString, ToolOption*>              m_optionIndex;

    DrawingDocument* m_document = nullptr;
    QUndoStack*      m_undoStack = nullptr;
};
