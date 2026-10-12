#pragma once

#include <qdraw/tools/EllipseTool.h>

/// Tool for creating circular EllipseObject instances via 1:1 constrained drag gesture.
class CircleTool : public EllipseTool {
    Q_OBJECT
public:
    explicit CircleTool(QObject* parent = nullptr);

protected:
    QRectF calculateGeometry(QPointF start, QPointF end) const override;
};
