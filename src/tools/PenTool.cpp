#include <qdraw/tools/PenTool.h>
#include <qdraw/drawing/FreehandStroke.h>
#include <qdraw/history/AddObjectCommand.h>
#include <qdraw/math/DrawingMath.h>
#include <qdraw/tools/options/ColorOption.h>
#include <qdraw/tools/options/RealOption.h>
#include <QPainterPath>

PenTool::PenTool(QObject* parent)
    : Tool(parent) {
    registerOption(std::make_unique<ColorOption>(QStringLiteral("strokeColor"), QStringLiteral("Color"), Qt::black));
    registerOption(std::make_unique<RealOption>(QStringLiteral("strokeWidth"), QStringLiteral("Width"), 2.0, 0.5, 50.0, 0.5));
    registerOption(std::make_unique<RealOption>(QStringLiteral("opacity"), QStringLiteral("Opacity"), 1.0, 0.05, 1.0, 0.05));
}

void PenTool::pointerPress(QPointF pos) {
    m_isDrawing = true;
    m_points.clear();
    m_points.append(pos);
    emit previewChanged();
}

void PenTool::pointerMove(QPointF pos) {
    if (!m_isDrawing) {
        return;
    }
    m_points.append(pos);
    emit previewChanged();
}

void PenTool::pointerRelease(QPointF pos) {
    if (!m_isDrawing) {
        return;
    }
    m_isDrawing = false;
    if (m_points.isEmpty() || m_points.last() != pos) {
        m_points.append(pos);
    }

    if (!m_points.isEmpty() && document() && undoStack()) {
        DrawingStyle style;
        style.strokeColor = colorOption(QStringLiteral("strokeColor"));
        style.strokeWidth = realOption(QStringLiteral("strokeWidth"));
        style.opacity     = realOption(QStringLiteral("opacity"));
        style.capStyle    = Qt::RoundCap;
        style.joinStyle   = Qt::RoundJoin;

        auto stroke = std::make_unique<FreehandStroke>(m_points, style);
        undoStack()->push(new AddObjectCommand(std::move(stroke), document()));
    }

    m_points.clear();
    emit previewChanged();
}

void PenTool::paintPreview(QPainter& painter) const {
    if (!m_isDrawing || m_points.isEmpty()) {
        return;
    }

    painter.save();
    painter.setOpacity(realOption(QStringLiteral("opacity")));
    painter.setRenderHint(QPainter::Antialiasing, true);

    qreal width = realOption(QStringLiteral("strokeWidth"));
    QColor color = colorOption(QStringLiteral("strokeColor"));

    if (m_points.size() == 1) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        qreal r = qdraw::math::calculateDotRadius(width);
        painter.drawEllipse(m_points[0], r, r);
        painter.restore();
        return;
    }

    QPen pen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    if (m_points.size() == 2) {
        painter.drawLine(m_points[0], m_points[1]);
        painter.restore();
        return;
    }

    painter.drawPath(qdraw::math::createSmoothPath(m_points));
    painter.restore();
}

void PenTool::deactivate() {
    m_isDrawing = false;
    if (!m_points.isEmpty()) {
        m_points.clear();
        emit previewChanged();
    }
}
