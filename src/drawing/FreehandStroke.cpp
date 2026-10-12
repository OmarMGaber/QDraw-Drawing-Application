#include <qdraw/drawing/FreehandStroke.h>
#include <qdraw/math/DrawingMath.h>

FreehandStroke::FreehandStroke(QVector<QPointF> points, DrawingStyle style)
    : DrawingObject(style)
    , m_points(std::move(points)) {
    updateBoundingRect();
}

void FreehandStroke::updateBoundingRect() {
    m_boundingRect = qdraw::math::calculatePointsBoundingRect(m_points, m_style.strokeWidth);
}

void FreehandStroke::paint(QPainter& painter) const {
    if (m_points.isEmpty()) {
        return;
    }

    painter.save();
    painter.setOpacity(m_style.opacity);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (m_points.size() == 1) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_style.strokeColor);
        qreal r = qdraw::math::calculateDotRadius(m_style.strokeWidth);
        painter.drawEllipse(m_points[0], r, r);
        painter.restore();
        return;
    }

    QPen pen(m_style.strokeColor, m_style.strokeWidth, Qt::SolidLine,
             m_style.capStyle, m_style.joinStyle);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    if (m_points.size() == 2) {
        painter.drawLine(m_points[0], m_points[1]);
        painter.restore();
        return;
    }

    painter.drawPath(qdraw::math::createSmoothPath(m_points));
    painter.restore();
}

QRectF FreehandStroke::boundingRect() const {
    return m_boundingRect;
}

bool FreehandStroke::intersectsCircle(QPointF centre, qreal radius) const {
    return qdraw::math::strokeIntersectsCircle(m_points, m_boundingRect, m_style.strokeWidth, centre, radius);
}
