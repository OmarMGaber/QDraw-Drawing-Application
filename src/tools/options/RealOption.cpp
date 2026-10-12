#include <qdraw/tools/options/RealOption.h>
#include <algorithm>

RealOption::RealOption(QString id, QString label, qreal defaultValue,
                       qreal min, qreal max, qreal step,
                       QObject* parent)
    : ToolOption(parent)
    , m_id(std::move(id))
    , m_label(std::move(label))
    , m_value(std::clamp(defaultValue, min, max))
    , m_min(min)
    , m_max(max)
    , m_step(step) {}

void RealOption::setValue(const QVariant& v) {
    bool ok = false;
    qreal newVal = v.toReal(&ok);
    if (ok) {
        newVal = std::clamp(newVal, m_min, m_max);
        if (newVal != m_value) {
            m_value = newVal;
            emit valueChanged();
        }
    }
}
