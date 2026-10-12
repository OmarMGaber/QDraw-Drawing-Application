#pragma once

#include <qdraw/drawing/DrawingStyle.h>
#include <QPainter>
#include <QPointF>
#include <QRectF>

/// Abstract base for all committed drawing objects.
///
/// Owns geometry (in subclasses) and a frozen DrawingStyle snapshot.
/// Rendering and hit-testing are virtual so document and canvas
/// require no knowledge of concrete types.
///
/// Lifetime: created by a tool, moved into AddObjectCommand,
/// then into DrawingDocument. Always held by std::unique_ptr.
class DrawingObject {
public:
    explicit DrawingObject(DrawingStyle style = {})
        : m_style(style) {}
    virtual ~DrawingObject() = default;

    /// Paint this object. Must apply m_style before drawing
    /// (opacity via QPainter::setOpacity, stroke via QPen, fill via QBrush).
    virtual void paint(QPainter& painter) const = 0;

    /// Axis-aligned bounding rect. Used for coarse hit-testing (O(1) check
    /// before any geometry-aware intersection test).
    virtual QRectF boundingRect() const = 0;

    /// True if this object's geometry intersects a circle centred at centre with radius.
    /// Implementation rule: check bounding rect first, then geometry-aware test.
    virtual bool intersectsCircle(QPointF centre, qreal radius) const = 0;

    /// Read-only access to the frozen style. Useful for testing and diagnostics.
    const DrawingStyle& style() const { return m_style; }

protected:
    DrawingStyle m_style;
};
