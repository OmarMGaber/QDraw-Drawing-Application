#pragma once

#include <QHoverEvent>
#include <QMouseEvent>
#include <QPointF>
#include <QQuickPaintedItem>
#include <qdraw/controller/DrawingController.h>

class DrawingController;

/// QQuickPaintedItem responsible for rendering canvas content and forwarding pointer input.
///
/// Rendering order inside paint():
///   1. White background fill
///   2. Committed objects via DrawingDocument (accessed through DrawingController)
///   3. In-progress preview: controller->activeTool()->paintPreview(*painter)
///
/// Does NOT own objects, tools, or history.
/// Does NOT call tool or controller methods directly — emits signals for all events.
/// Canvas repaints when it receives repaintNeeded() from DrawingController.
class DrawingCanvas : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(DrawingController* controller READ controller
                                             WRITE setController
                                             NOTIFY controllerChanged)
public:
    explicit DrawingCanvas(QQuickItem* parent = nullptr);
    ~DrawingCanvas() override = default;

    DrawingController* controller() const { return m_controller; }
    void setController(DrawingController* controller);

    void paint(QPainter* painter) override;

signals:
    void pointerPressed(QPointF pos);
    void pointerMoved(QPointF pos);
    void pointerReleased(QPointF pos);
    void pointerHovered(QPointF pos);
    void controllerChanged();

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void hoverMoveEvent(QHoverEvent* e) override;

private:
    DrawingController* m_controller = nullptr;
};
