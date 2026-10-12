#pragma once

#include <qdraw/tools/ShapeTool.h>

/// Tool for creating EllipseObject instances via drag gesture.
class EllipseTool : public ShapeTool {
    Q_OBJECT
public:
    explicit EllipseTool(QObject* parent = nullptr);

protected:
    std::unique_ptr<DrawingObject> createObject(QPointF start, QPointF end) const override;
    void drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const override;

    /// Computes the bounding rectangle from gesture start and end coordinates.
    /// Defaults to the unconstrained normalized rectangle; overridden by CircleTool.
    virtual QRectF calculateGeometry(QPointF start, QPointF end) const;
};
