#include <qdraw/drawing/LineObject.h>
#include <qdraw/math/DrawingMath.h>

LineObject::LineObject(QPointF start, QPointF end, DrawingStyle style)
    : DrawingObject(style)
    , m_start(start)
    , m_end(end) {}

void LineObject::paint(QPainter& painter) const {
    painter.save();
    painter.setOpacity(m_style.opacity);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(m_style.strokeColor, m_style.strokeWidth, Qt::SolidLine,
             m_style.capStyle, m_style.joinStyle);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    painter.drawLine(m_start, m_end);
    painter.restore();
}

QRectF LineObject::boundingRect() const {
    return qdraw::math::calculateLineBoundingRect(m_start, m_end, m_style.strokeWidth);
}

bool LineObject::intersectsCircle(QPointF centre, qreal radius) const {
    return qdraw::math::lineIntersectsCircle(m_start, m_end, m_style.strokeWidth, centre, radius);
}
