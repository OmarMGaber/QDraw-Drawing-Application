#include <qdraw/tools/options/ColorOption.h>

ColorOption::ColorOption(QString id, QString label, QColor defaultValue, QObject* parent)
    : ToolOption(parent)
    , m_id(std::move(id))
    , m_label(std::move(label))
    , m_color(defaultValue) {}

void ColorOption::setValue(const QVariant& v) {
    QColor newColor = v.value<QColor>();
    if (!newColor.isValid()) {
        if (v.canConvert<QString>()) {
            newColor = QColor::fromString(v.toString());
        }
    }
    if (newColor.isValid() && newColor != m_color) {
        m_color = newColor;
        emit valueChanged();
    }
}
