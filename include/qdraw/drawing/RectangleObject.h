#pragma once

#include <qdraw/drawing/DrawingObject.h>

/// Committed rectangle defined by a normalized bounding box.
///
/// Supports stroke and optional fill.
/// Hit-tested against a circle: checks if circle overlaps the outline or,
/// if fill is enabled, the interior.
class RectangleObject : public DrawingObject {
public:
    RectangleObject(QRectF rect, DrawingStyle style);

    void paint(QPainter& painter) const override;
    QRectF boundingRect() const override;
    bool intersectsCircle(QPointF centre, qreal radius) const override;

    QRectF rect() const { return m_rect; }

private:
    QRectF m_rect;
};
