#include <qdraw/tools/EraserTool.h>
#include <qdraw/history/RemoveObjectsCommand.h>
#include <qdraw/tools/options/RealOption.h>

EraserTool::EraserTool(QObject* parent)
    : Tool(parent) {
    registerOption(std::make_unique<RealOption>(QStringLiteral("radius"), QStringLiteral("Eraser Size"), 20.0, 5.0, 100.0, 1.0));
}

void EraserTool::pointerHover(QPointF pos) {
    m_cursorPos = pos;
    m_hasCursor = true;
    emit previewChanged();
}

void EraserTool::pointerPress(QPointF pos) {
    m_cursorPos = pos;
    m_hasCursor = true;
    m_isErasing = true;
    m_hitObjects.clear();
    checkHits(pos);
    emit previewChanged();
}

void EraserTool::pointerMove(QPointF pos) {
    m_cursorPos = pos;
    m_hasCursor = true;
    if (m_isErasing) {
        checkHits(pos);
    }
    emit previewChanged();
}

void EraserTool::pointerRelease(QPointF pos) {
    m_cursorPos = pos;
    m_hasCursor = true;
    if (m_isErasing) {
        checkHits(pos);
        m_isErasing = false;
        commitHits();
    }
    emit previewChanged();
}

void EraserTool::checkHits(QPointF pos) {
    if (!document()) {
        return;
    }
    qreal r = realOption(QStringLiteral("radius"));
    const auto objects = document()->objects();
    for (DrawingObject* obj : objects) {
        if (!m_hitObjects.contains(obj) && obj->intersectsCircle(pos, r)) {
            m_hitObjects.insert(obj);
        }
    }
}

void EraserTool::commitHits() {
    if (m_hitObjects.isEmpty() || !document() || !undoStack()) {
        m_hitObjects.clear();
        return;
    }

    std::vector<RemoveObjectsCommand::Entry> entries;
    const auto objects = document()->objects();
    for (int i = 0; i < objects.size(); ++i) {
        if (m_hitObjects.contains(objects[i])) {
            entries.push_back({ i, nullptr });
        }
    }

    if (!entries.empty()) {
        undoStack()->push(new RemoveObjectsCommand(std::move(entries), document()));
    }
    m_hitObjects.clear();
}

void EraserTool::paintPreview(QPainter& painter) const {
    if (!m_hasCursor) {
        return;
    }

    qreal r = realOption(QStringLiteral("radius"));
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Subtle highlighted fill when active erasing vs hovering
    if (m_isErasing) {
        painter.setBrush(QColor(239, 68, 68, 45)); // Soft red tint
        painter.setPen(QPen(QColor(220, 38, 38, 220), 1.5, Qt::SolidLine));
    } else {
        painter.setBrush(QColor(59, 130, 246, 25)); // Soft blue/neutral tint
        painter.setPen(QPen(QColor(71, 85, 105, 200), 1.2, Qt::DashLine));
    }

    painter.drawEllipse(m_cursorPos, r, r);
    painter.restore();
}

void EraserTool::deactivate() {
    m_hasCursor = false;
    m_isErasing = false;
    m_hitObjects.clear();
    emit previewChanged();
}
