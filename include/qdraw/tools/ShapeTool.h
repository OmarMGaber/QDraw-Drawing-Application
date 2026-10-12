#pragma once

#include <qdraw/tools/Tool.h>
#include <qdraw/drawing/DrawingObject.h>

/// Abstract base for single-gesture shape tools.
///
/// Encapsulates the press -> preview -> release lifecycle.
/// Subclasses only implement createObject() and drawPreviewGeometry().
class ShapeTool : public Tool {
    Q_OBJECT
public:
    explicit ShapeTool(QObject* parent = nullptr);

    void pointerPress(QPointF pos) override;
    void pointerMove(QPointF pos) override;
    void pointerRelease(QPointF pos) override;

    void paintPreview(QPainter& painter) const override;
    void deactivate() override;

protected:
    /// Build and return the committed object from the completed gesture.
    virtual std::unique_ptr<DrawingObject> createObject(QPointF start, QPointF end) const = 0;

    /// Render shape outline/fill for the in-progress gesture preview.
    virtual void drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const = 0;

    QPointF startPos() const { return m_start; }
    QPointF currentPos() const { return m_current; }

private:
    QPointF m_start;
    QPointF m_current;
    bool    m_isDragging = false;
};
