#pragma once

#include <qdraw/tools/ShapeTool.h>

/// Tool for creating RectangleObject instances via drag gesture.
class RectangleTool : public ShapeTool {
    Q_OBJECT
public:
    explicit RectangleTool(QObject* parent = nullptr);

protected:
    std::unique_ptr<DrawingObject> createObject(QPointF start, QPointF end) const override;
    void drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const override;
};
