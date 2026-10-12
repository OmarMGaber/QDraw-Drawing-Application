#include <qdraw/tools/RectangleTool.h>
#include <qdraw/drawing/RectangleObject.h>
#include <qdraw/tools/options/BoolOption.h>
#include <qdraw/tools/options/ColorOption.h>
#include <qdraw/tools/options/RealOption.h>

RectangleTool::RectangleTool(QObject* parent)
    : ShapeTool(parent) {
    registerOption(std::make_unique<ColorOption>(QStringLiteral("strokeColor"), QStringLiteral("Stroke Color"), Qt::black));
    registerOption(std::make_unique<RealOption>(QStringLiteral("strokeWidth"), QStringLiteral("Stroke Width"), 2.0, 0.5, 50.0, 0.5));
    registerOption(std::make_unique<RealOption>(QStringLiteral("opacity"), QStringLiteral("Opacity"), 1.0, 0.05, 1.0, 0.05));
    registerOption(std::make_unique<BoolOption>(QStringLiteral("fillEnabled"), QStringLiteral("Fill"), false));
    registerOption(std::make_unique<ColorOption>(QStringLiteral("fillColor"), QStringLiteral("Fill Color"), Qt::white));
}

std::unique_ptr<DrawingObject> RectangleTool::createObject(QPointF start, QPointF end) const {
    DrawingStyle style;
    style.strokeColor = colorOption(QStringLiteral("strokeColor"));
    style.strokeWidth = realOption(QStringLiteral("strokeWidth"));
    style.opacity     = realOption(QStringLiteral("opacity"));
    style.fillEnabled = boolOption(QStringLiteral("fillEnabled"));
    style.fillColor   = colorOption(QStringLiteral("fillColor"));
    style.capStyle    = Qt::SquareCap;
    style.joinStyle   = Qt::MiterJoin;

    return std::make_unique<RectangleObject>(QRectF(start, end).normalized(), style);
}

void RectangleTool::drawPreviewGeometry(QPainter& painter, QPointF start, QPointF end) const {
    painter.drawRect(QRectF(start, end).normalized());
}
