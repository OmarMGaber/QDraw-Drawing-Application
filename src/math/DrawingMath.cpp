#include <qdraw/math/DrawingMath.h>
#include <algorithm>
#include <cmath>

namespace qdraw::math {

qreal distance(QPointF a, QPointF b) {
    return std::hypot(b.x() - a.x(), b.y() - a.y());
}

qreal pointToSegmentDistance(QPointF p, QPointF a, QPointF b) {
    QPointF v = b - a;
    QPointF w = p - a;
    qreal c1 = QPointF::dotProduct(w, v);
    if (c1 <= 0) {
        return distance(p, a);
    }
    qreal c2 = QPointF::dotProduct(v, v);
    if (c2 <= c1 || c2 == 0) {
        return distance(p, b);
    }
    qreal t = c1 / c2;
    QPointF projection = a + t * v;
    return distance(p, projection);
}

qreal calculateStrokePadding(qreal strokeWidth) {
    return (strokeWidth * HalfStrokeFactor) + BoundingRectPadding;
}

qreal calculateHitThreshold(qreal radius, qreal strokeWidth) {
    return radius + (strokeWidth * HalfStrokeFactor);
}

qreal calculateDotRadius(qreal strokeWidth) {
    return std::max(MinDotRadius, strokeWidth * HalfStrokeFactor);
}

bool isGestureDragSignificant(QPointF start, QPointF current, qreal minDistance) {
    return distance(start, current) >= minDistance;
}

bool isPointInExpandedBounds(const QRectF& bounds, QPointF point, qreal radius) {
    return bounds.adjusted(-radius, -radius, radius, radius).contains(point);
}

QRectF calculateLineBoundingRect(QPointF start, QPointF end, qreal strokeWidth) {
    qreal minX = std::min(start.x(), end.x());
    qreal maxX = std::max(start.x(), end.x());
    qreal minY = std::min(start.y(), end.y());
    qreal maxY = std::max(start.y(), end.y());
    qreal pad = calculateStrokePadding(strokeWidth);

    return QRectF(minX - pad, minY - pad,
                  (maxX - minX) + 2 * pad,
                  (maxY - minY) + 2 * pad);
}

bool lineIntersectsCircle(QPointF start, QPointF end, qreal strokeWidth, QPointF centre, qreal radius) {
    QRectF bounds = calculateLineBoundingRect(start, end, strokeWidth);
    if (!isPointInExpandedBounds(bounds, centre, radius)) {
        return false;
    }

    qreal threshold = calculateHitThreshold(radius, strokeWidth);
    return pointToSegmentDistance(centre, start, end) <= threshold;
}

QRectF calculateRectangleBoundingRect(const QRectF& rect, qreal strokeWidth) {
    qreal pad = calculateStrokePadding(strokeWidth);
    return rect.adjusted(-pad, -pad, pad, pad);
}

QPointF closestPointOnRect(QPointF point, const QRectF& rect) {
    return QPointF(std::clamp(point.x(), rect.left(), rect.right()),
                   std::clamp(point.y(), rect.top(), rect.bottom()));
}

bool rectangleIntersectsCircle(const QRectF& rect, qreal strokeWidth, bool fillEnabled, QPointF centre, qreal radius) {
    QRectF bounds = calculateRectangleBoundingRect(rect, strokeWidth);
    if (!isPointInExpandedBounds(bounds, centre, radius)) {
        return false;
    }

    qreal threshold = calculateHitThreshold(radius, strokeWidth);

    if (fillEnabled) {
        QPointF clamped = closestPointOnRect(centre, rect);
        return distance(centre, clamped) <= threshold;
    }

    // Outline only: test all 4 segments
    QPointF tl = rect.topLeft();
    QPointF tr = rect.topRight();
    QPointF br = rect.bottomRight();
    QPointF bl = rect.bottomLeft();

    if (pointToSegmentDistance(centre, tl, tr) <= threshold) return true;
    if (pointToSegmentDistance(centre, tr, br) <= threshold) return true;
    if (pointToSegmentDistance(centre, br, bl) <= threshold) return true;
    if (pointToSegmentDistance(centre, bl, tl) <= threshold) return true;

    return false;
}

QRectF calculateEllipseBoundingRect(const QRectF& rect, qreal strokeWidth) {
    qreal pad = calculateStrokePadding(strokeWidth);
    return rect.adjusted(-pad, -pad, pad, pad);
}

QRectF calculateCircleBoundingBox(QPointF start, QPointF end) {
    const qreal dx = end.x() - start.x();
    const qreal dy = end.y() - start.y();
    const qreal side = std::max(std::abs(dx), std::abs(dy));

    const qreal left = (dx >= 0.0) ? start.x() : (start.x() - side);
    const qreal top = (dy >= 0.0) ? start.y() : (start.y() - side);

    return QRectF(left, top, side, side);
}

bool isPointInsideEllipse(QPointF point, const QRectF& rect) {
    qreal a = rect.width() * 0.5;
    qreal b = rect.height() * 0.5;
    if (a <= 0 || b <= 0) {
        return false;
    }
    QPointF ec = rect.center();
    qreal dx = point.x() - ec.x();
    qreal dy = point.y() - ec.y();
    return ((dx * dx) / (a * a) + (dy * dy) / (b * b)) <= NormalizedEllipseThreshold;
}

bool ellipseIntersectsCircle(const QRectF& rect, qreal strokeWidth, bool fillEnabled, QPointF centre, qreal radius) {
    QRectF bounds = calculateEllipseBoundingRect(rect, strokeWidth);
    if (!isPointInExpandedBounds(bounds, centre, radius)) {
        return false;
    }

    qreal threshold = calculateHitThreshold(radius, strokeWidth);
    qreal a = rect.width() * 0.5;
    qreal b = rect.height() * 0.5;
    QPointF ec = rect.center();

    if (a <= 0 || b <= 0) {
        return distance(centre, ec) <= threshold;
    }

    if (fillEnabled && isPointInsideEllipse(centre, rect)) {
        return true;
    }

    // Boundary distance check using polygonal approximation
    QPointF prevPt(ec.x() + a, ec.y());
    for (int i = 1; i <= EllipseSegmentCount; ++i) {
        qreal theta = (2 * Pi * i) / EllipseSegmentCount;
        QPointF currPt(ec.x() + a * std::cos(theta), ec.y() + b * std::sin(theta));
        if (pointToSegmentDistance(centre, prevPt, currPt) <= threshold) {
            return true;
        }
        prevPt = currPt;
    }

    return false;
}

QRectF calculatePointsBoundingRect(const QVector<QPointF>& points, qreal strokeWidth) {
    if (points.isEmpty()) {
        return {};
    }

    qreal minX = points[0].x();
    qreal maxX = points[0].x();
    qreal minY = points[0].y();
    qreal maxY = points[0].y();

    for (const auto& pt : points) {
        if (pt.x() < minX) minX = pt.x();
        if (pt.x() > maxX) maxX = pt.x();
        if (pt.y() < minY) minY = pt.y();
        if (pt.y() > maxY) maxY = pt.y();
    }

    qreal pad = calculateStrokePadding(strokeWidth);
    return QRectF(minX - pad, minY - pad,
                  (maxX - minX) + 2 * pad,
                  (maxY - minY) + 2 * pad);
}

QPainterPath createSmoothPath(const QVector<QPointF>& points) {
    QPainterPath path;
    if (points.isEmpty()) {
        return path;
    }

    path.moveTo(points[0]);
    if (points.size() == 1) {
        return path;
    }

    if (points.size() == 2) {
        path.lineTo(points[1]);
        return path;
    }

    for (int i = 0; i < points.size() - 1; ++i) {
        QPointF p0 = (i == 0) ? (CatmullRomExtrapolationFactor * points[0] - points[1]) : points[i - 1];
        QPointF p1 = points[i];
        QPointF p2 = points[i + 1];
        QPointF p3 = (i + 2 < points.size()) ? points[i + 2] : (CatmullRomExtrapolationFactor * points[i + 1] - points[i]);

        QPointF b1 = p1 + (p2 - p0) / CatmullRomDivisor;
        QPointF b2 = p2 - (p3 - p1) / CatmullRomDivisor;
        path.cubicTo(b1, b2, p2);
    }

    return path;
}

bool strokeIntersectsCircle(const QVector<QPointF>& points, const QRectF& boundingRect, qreal strokeWidth, QPointF centre, qreal radius) {
    if (points.isEmpty()) {
        return false;
    }

    if (!isPointInExpandedBounds(boundingRect, centre, radius)) {
        return false;
    }

    qreal threshold = calculateHitThreshold(radius, strokeWidth);

    if (points.size() == 1) {
        return distance(centre, points[0]) <= threshold;
    }

    for (int i = 0; i < points.size() - 1; ++i) {
        if (pointToSegmentDistance(centre, points[i], points[i + 1]) <= threshold) {
            return true;
        }
    }

    return false;
}

} // namespace qdraw::math
