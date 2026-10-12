#include <qdraw/tools/ShapeTool.h>
#include <qdraw/history/AddObjectCommand.h>
#include <qdraw/math/DrawingMath.h>

ShapeTool::ShapeTool(QObject* parent)
    : Tool(parent) {}

void ShapeTool::pointerPress(QPointF pos) {
    m_isDragging = true;
    m_start = pos;
    m_current = pos;
    emit previewChanged();
}

void ShapeTool::pointerMove(QPointF pos) {
    if (!m_isDragging) {
        return;
    }
    m_current = pos;
    emit previewChanged();
}

void ShapeTool::pointerRelease(QPointF pos) {
    if (!m_isDragging) {
        return;
    }
    m_isDragging = false;
    m_current = pos;

    // Discard zero-length gestures
    if (qdraw::math::isGestureDragSignificant(m_start, m_current) && document() && undoStack()) {
        auto obj = createObject(m_start, m_current);
        if (obj) {
            undoStack()->push(new AddObjectCommand(std::move(obj), document()));
        }
    }

    emit previewChanged();
}

void ShapeTool::paintPreview(QPainter& painter) const {
    if (!m_isDragging) {
        return;
    }

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setOpacity(realOption(QStringLiteral("opacity")));

    QPen pen(colorOption(QStringLiteral("strokeColor")),
             realOption(QStringLiteral("strokeWidth")),
             Qt::DashLine,
             Qt::RoundCap,
             Qt::RoundJoin);
    painter.setPen(pen);

    if (boolOption(QStringLiteral("fillEnabled"))) {
        QColor fill = colorOption(QStringLiteral("fillColor"));
        painter.setBrush(QBrush(fill));
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    drawPreviewGeometry(painter, m_start, m_current);
    painter.restore();
}

void ShapeTool::deactivate() {
    if (m_isDragging) {
        m_isDragging = false;
        emit previewChanged();
    }
}
