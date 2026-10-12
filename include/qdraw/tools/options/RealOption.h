#pragma once

#include <qdraw/tools/options/ToolOption.h>

/// Configurable floating-point option with min/max bounds and step size.
/// Exposed to QML as a labeled slider.
class RealOption final : public ToolOption {
    Q_OBJECT
public:
    RealOption(QString id, QString label, qreal defaultValue,
               qreal min, qreal max, qreal step = 0.1,
               QObject* parent = nullptr);

    QString id() const override { return m_id; }
    QString label() const override { return m_label; }
    QDrawOptions::OptionType type() const override { return QDrawOptions::OptionType::Real; }
    QVariant value() const override { return m_value; }
    void setValue(const QVariant& v) override;

    /// Typed getter used inside Tool subclasses to populate DrawingStyle.
    qreal real() const { return m_value; }
    qreal min()  const { return m_min; }
    qreal max()  const { return m_max; }
    qreal step() const { return m_step; }

private:
    QString m_id;
    QString m_label;
    qreal   m_value;
    qreal   m_min;
    qreal   m_max;
    qreal   m_step;
};
