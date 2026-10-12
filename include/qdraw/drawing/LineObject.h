#pragma once

#include <qdraw/drawing/DrawingObject.h>

/// Committed straight line between two endpoints.
///
/// Hit-tested against a circle using minimum perpendicular or endpoint
/// distance from the circle centre to the line segment.
class LineObject : public DrawingObject {
public:
    LineObject(QPointF start, QPointF end, DrawingStyle style);

    void paint(QPainter& painter) const override;
    QRectF boundingRect() const override;
    bool intersectsCircle(QPointF centre, qreal radius) const override;

    QPointF startPoint() const { return m_start; }
    QPointF endPoint() const { return m_end; }

private:
    QPointF m_start;
    QPointF m_end;
};
