#pragma once

#include <qdraw/drawing/DrawingObject.h>
#include <QVector>

/// Committed freehand stroke composed of sampled pointer positions.
///
/// Renders via Catmull-Rom spline converted to cubic Bezier curve segments
/// in a QPainterPath for smooth curves passing through all sample points.
/// Hit-tested against a circle by checking distance from the circle centre
/// to each line segment formed by consecutive points.
class FreehandStroke : public DrawingObject {
public:
    /// @param points Sampled gesture points in canvas coordinates.
    /// @param style  Frozen style snapshot at gesture completion time.
    FreehandStroke(QVector<QPointF> points, DrawingStyle style);

    void paint(QPainter& painter) const override;
    QRectF boundingRect() const override;
    bool intersectsCircle(QPointF centre, qreal radius) const override;

    /// Read-only access to sampled points.
    const QVector<QPointF>& points() const { return m_points; }

private:
    QVector<QPointF> m_points;
    QRectF           m_boundingRect;

    void updateBoundingRect();
};
