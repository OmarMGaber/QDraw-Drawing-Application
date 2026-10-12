#include <qdraw/tools/CircleTool.h>
#include <qdraw/math/DrawingMath.h>

CircleTool::CircleTool(QObject* parent)
    : EllipseTool(parent) {}

QRectF CircleTool::calculateGeometry(QPointF start, QPointF end) const {
    return qdraw::math::calculateCircleBoundingBox(start, end);
}
