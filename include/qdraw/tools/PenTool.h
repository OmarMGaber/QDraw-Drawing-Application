#pragma once

#include <qdraw/tools/Tool.h>
#include <QVector>

/// Freehand drawing tool.
///
/// Collects sampled pointer positions, renders an in-progress Catmull-Rom spline preview,
/// and commits a FreehandStroke object via AddObjectCommand on pointer release.
class PenTool : public Tool {
    Q_OBJECT
public:
    explicit PenTool(QObject* parent = nullptr);

    void pointerPress(QPointF pos) override;
    void pointerMove(QPointF pos) override;
    void pointerRelease(QPointF pos) override;

    void paintPreview(QPainter& painter) const override;
    void deactivate() override;

private:
    QVector<QPointF> m_points;
    bool             m_isDrawing = false;
};
