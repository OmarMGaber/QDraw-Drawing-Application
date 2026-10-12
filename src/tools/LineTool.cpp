#include <qdraw/tools/LineTool.h>
#include <qdraw/drawing/LineObject.h>
#include <qdraw/tools/options/ColorOption.h>
#include <qdraw/tools/options/RealOption.h>

LineTool::LineTool(QObject* parent)
    : ShapeTool(parent) {
    registerOption(std::make_unique<ColorOption>(QStringLiteral("strokeColor"), QStringLiteral("Line Color"), Qt::black));
    registerOption(std::make_unique<RealOption>(QStringLiteral("strokeWidth"), QStringLiteral("Line Width"), 2.0, 0.5, 50.0, 0.5));
    registerOption(std::make_unique<RealOption>(QStringLiteral("opacity"), QStringLiteral("Opacity"), 1.0, 0.05, 1.0, 0.05));
}

std::unique_ptr<DrawingObject> LineTool::createObject(QPointF start, QPointF end) const {
    DrawingStyle style;
    style.strokeColor = colorOption(QStringLiteral("strokeColor"));
    style.strokeWidth = realOption(QStringLiteral("strokeWidth"));
    style.opacity     = realOption(QStringLiteral("opacity"));
    style.capStyle    = Qt::RoundCap;
    style.joinStyle   = Qt::RoundJoin;

    return std::make_unique<LineObject>(start, end, style);
}

void LineTool::drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const {
    painter.drawLine(start, end);
}
