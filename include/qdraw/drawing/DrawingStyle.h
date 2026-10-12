#pragma once

#include <QColor>
#include <QPainter>

/// Immutable rendering snapshot stored in every committed DrawingObject.
///
/// Created by a tool's commit step by reading its live ToolOption registry
/// at the moment the user completes a gesture. Never modified after construction.
///
/// Contains only what QPainter needs — no UI metadata (labels, min/max, type tags).
/// ToolOption is mutable, UI-aware, and owned by a tool.
/// DrawingStyle is frozen, render-only, and owned by a DrawingObject.
struct DrawingStyle {
    QColor           strokeColor = Qt::black;
    QColor           fillColor   = Qt::transparent;
    qreal            strokeWidth = 2.0;
    qreal            opacity     = 1.0;
    bool             fillEnabled = false;
    Qt::PenCapStyle  capStyle    = Qt::RoundCap;
    Qt::PenJoinStyle joinStyle   = Qt::RoundJoin;
};
