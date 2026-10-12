#include <qdraw/canvas/DrawingCanvas.h>
#include <QCursor>
#include <QPainter>

DrawingCanvas::DrawingCanvas(QQuickItem* parent)
    : QQuickPaintedItem(parent) {
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAntialiasing(true);
    setCursor(QCursor(Qt::CrossCursor));
}

void DrawingCanvas::setController(DrawingController* controller) {
    if (m_controller == controller) {
        return;
    }

    if (m_controller) {
        disconnect(this, nullptr, m_controller, nullptr);
        disconnect(m_controller, nullptr, this, nullptr);
    }

    m_controller = controller;

    if (m_controller) {
        connect(this, &DrawingCanvas::pointerPressed, m_controller, &DrawingController::onPointerPressed);
        connect(this, &DrawingCanvas::pointerMoved, m_controller, &DrawingController::onPointerMoved);
        connect(this, &DrawingCanvas::pointerReleased, m_controller, &DrawingController::onPointerReleased);
        connect(this, &DrawingCanvas::pointerHovered, m_controller, &DrawingController::onPointerHovered);
        connect(m_controller, &DrawingController::repaintNeeded, this, [this]() {
            this->update();
        });
    }

    emit controllerChanged();
    update();
}

void DrawingCanvas::paint(QPainter* painter) {
    if (!painter) {
        return;
    }

    // 1. Clean background fill
    painter->fillRect(boundingRect(), Qt::white);

    // 2. Render all committed objects in stacking order
    if (m_controller && m_controller->document()) {
        const auto objects = m_controller->document()->objects();
        for (const DrawingObject* obj : objects) {
            if (obj) {
                obj->paint(*painter);
            }
        }
    }

    // 3. Render active tool preview overlay
    if (m_controller && m_controller->activeTool()) {
        m_controller->activeTool()->paintPreview(*painter);
    }
}

void DrawingCanvas::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        emit pointerPressed(e->position());
        e->accept();
    } else {
        QQuickPaintedItem::mousePressEvent(e);
    }
}

void DrawingCanvas::mouseMoveEvent(QMouseEvent* e) {
    emit pointerMoved(e->position());
    e->accept();
}

void DrawingCanvas::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        emit pointerReleased(e->position());
        e->accept();
    } else {
        QQuickPaintedItem::mouseReleaseEvent(e);
    }
}

void DrawingCanvas::hoverMoveEvent(QHoverEvent* e) {
    emit pointerHovered(e->position());
    e->accept();
}
