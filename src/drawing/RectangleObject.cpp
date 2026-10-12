#include <qdraw/drawing/RectangleObject.h>
#include <qdraw/math/DrawingMath.h>

RectangleObject::RectangleObject(QRectF rect, DrawingStyle style)
    : DrawingObject(style)
    , m_rect(rect.normalized()) {}

void RectangleObject::paint(QPainter& painter) const {
    painter.save();
    painter.setOpacity(m_style.opacity);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(m_style.strokeColor, m_style.strokeWidth, Qt::SolidLine,
             m_style.capStyle, m_style.joinStyle);
    painter.setPen(pen);

    if (m_style.fillEnabled) {
        painter.setBrush(QBrush(m_style.fillColor));
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    painter.drawRect(m_rect);
    painter.restore();
}

QRectF RectangleObject::boundingRect() const {
    return qdraw::math::calculateRectangleBoundingRect(m_rect, m_style.strokeWidth);
}

bool RectangleObject::intersectsCircle(QPointF centre, qreal radius) const {
    return qdraw::math::rectangleIntersectsCircle(m_rect, m_style.strokeWidth, m_style.fillEnabled, centre, radius);
}
