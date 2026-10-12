#include <qdraw/tools/options/BoolOption.h>

BoolOption::BoolOption(QString id, QString label, bool defaultValue, QObject* parent)
    : ToolOption(parent)
    , m_id(std::move(id))
    , m_label(std::move(label))
    , m_value(defaultValue) {}

void BoolOption::setValue(const QVariant& v) {
    bool newVal = v.toBool();
    if (newVal != m_value) {
        m_value = newVal;
        emit valueChanged();
    }
}
