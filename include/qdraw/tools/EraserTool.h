#pragma once

#include <qdraw/tools/Tool.h>
#include <QSet>

/// Circular whole-object eraser tool.
///
/// Tracks hover movements to display a circular cursor overlay.
/// During drag gestures, tests intersection against all document objects,
/// deduplicating hits within the gesture, and commits a single
/// RemoveObjectsCommand upon pointer release.
class EraserTool : public Tool {
    Q_OBJECT
public:
    explicit EraserTool(QObject* parent = nullptr);

    void pointerPress(QPointF pos) override;
    void pointerMove(QPointF pos) override;
    void pointerRelease(QPointF pos) override;
    void pointerHover(QPointF pos) override;

    void paintPreview(QPainter& painter) const override;
    void deactivate() override;

private:
    void checkHits(QPointF pos);
    void commitHits();

    QPointF             m_cursorPos;
    bool                m_hasCursor = false;
    bool                m_isErasing = false;
    QSet<DrawingObject*> m_hitObjects;
};
