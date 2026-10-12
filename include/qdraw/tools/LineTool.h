#pragma once

#include <qdraw/tools/ShapeTool.h>

/// Tool for creating LineObject instances via drag gesture.
class LineTool : public ShapeTool {
    Q_OBJECT
public:
    explicit LineTool(QObject* parent = nullptr);

protected:
    std::unique_ptr<DrawingObject> createObject(QPointF start, QPointF end) const override;
    void drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const override;
};
