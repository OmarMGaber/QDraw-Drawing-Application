#pragma once

#include <qdraw/math/MathConstants.h>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QVector>

namespace qdraw::math {

// --- Basic Geometric Primitives ---

/// Euclidean distance between two points.
qreal distance(QPointF a, QPointF b);

/// Minimum distance from point @p p to line segment [@p a, @p b].
qreal pointToSegmentDistance(QPointF p, QPointF a, QPointF b);

/// Stroke padding added to geometry boundaries for stroke width and antialiasing.
qreal calculateStrokePadding(qreal strokeWidth);

/// Hit test distance threshold given probe radius and stroke width.
qreal calculateHitThreshold(qreal radius, qreal strokeWidth);

/// Minimum radius for drawing a single-point dot.
qreal calculateDotRadius(qreal strokeWidth);

/// Checks if gesture drag vector between start and current point exceeds minimum distance threshold.
bool isGestureDragSignificant(QPointF start, QPointF current, qreal minDistance = MinimumDragDistance);

/// Checks if point is inside bounding box expanded by probe radius.
bool isPointInExpandedBounds(const QRectF& bounds, QPointF point, qreal radius);

// --- Line Object Math ---

/// Bounding rectangle for a straight line segment, padded by stroke width.
QRectF calculateLineBoundingRect(QPointF start, QPointF end, qreal strokeWidth);

/// Hit test: checks if circle (centre, radius) intersects line segment [@p start, @p end].
bool lineIntersectsCircle(QPointF start, QPointF end, qreal strokeWidth, QPointF centre, qreal radius);

// --- Rectangle Object Math ---

/// Bounding rectangle for an axis-aligned rectangle, padded by stroke width.
QRectF calculateRectangleBoundingRect(const QRectF& rect, qreal strokeWidth);

/// Returns the closest point on or inside an axis-aligned rectangle to a given test point.
QPointF closestPointOnRect(QPointF point, const QRectF& rect);

/// Hit test: checks if circle (centre, radius) intersects rectangle (outline or filled interior).
bool rectangleIntersectsCircle(const QRectF& rect, qreal strokeWidth, bool fillEnabled, QPointF centre, qreal radius);

// --- Ellipse & Circle Object Math ---

/// Bounding rectangle for an ellipse, padded by stroke width.
QRectF calculateEllipseBoundingRect(const QRectF& rect, qreal strokeWidth);

/// Bounding box for a circle gesture constrained to a 1:1 aspect ratio square.
QRectF calculateCircleBoundingBox(QPointF start, QPointF end);

/// Checks if a test point lies inside or on the normalized boundary of an ellipse inscribed in @p rect.
bool isPointInsideEllipse(QPointF point, const QRectF& rect);

/// Hit test: checks if circle (centre, radius) intersects ellipse (outline or filled interior).
bool ellipseIntersectsCircle(const QRectF& rect, qreal strokeWidth, bool fillEnabled, QPointF centre, qreal radius);

// --- Freehand Stroke & Curve Math ---

/// Bounding rectangle enclosing a set of 2D points, padded by stroke width.
QRectF calculatePointsBoundingRect(const QVector<QPointF>& points, qreal strokeWidth);

/// Constructs a smooth QPainterPath from sampled points using Catmull-Rom to cubic Bezier conversion.
QPainterPath createSmoothPath(const QVector<QPointF>& points);

/// Hit test: checks if circle (centre, radius) intersects freehand polyline/points.
bool strokeIntersectsCircle(const QVector<QPointF>& points, const QRectF& boundingRect, qreal strokeWidth, QPointF centre, qreal radius);

} // namespace qdraw::math
