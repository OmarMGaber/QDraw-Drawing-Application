#pragma once

#include <QtGlobal>
#include <cmath>

namespace qdraw::math {

// Mathematical constants
constexpr qreal Pi = std::acos(-1.0);

// Geometry and rendering constants
constexpr qreal StrokeFactor = 1;
constexpr qreal HalfStrokeFactor = StrokeFactor / 2.0;
constexpr qreal BoundingRectPadding = 1.0;
constexpr qreal MinDotRadius = 0.5;
constexpr qreal MinimumDragDistance = 1.0;

// Ellipse approximation constants
constexpr int EllipseSegmentCount = 32;
constexpr qreal NormalizedEllipseThreshold = 1.0;

// Catmull-Rom spline curve constants
constexpr qreal CatmullRomDivisor = 6.0;
constexpr qreal CatmullRomExtrapolationFactor = 2.0;

} // namespace qdraw::math
