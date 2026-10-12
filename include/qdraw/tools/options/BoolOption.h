#pragma once

#include <qdraw/tools/options/ToolOption.h>

/// Configurable boolean option.
/// Exposed to QML as a toggle switch.
class BoolOption final : public ToolOption {
    Q_OBJECT
public:
    BoolOption(QString id, QString label, bool defaultValue, QObject* parent = nullptr);

    QString id() const override { return m_id; }
    QString label() const override { return m_label; }
    QDrawOptions::OptionType type() const override { return QDrawOptions::OptionType::Bool; }
    QVariant value() const override { return m_value; }
    void setValue(const QVariant& v) override;

    /// Typed getter used inside Tool subclasses to populate DrawingStyle.
    bool boolean() const { return m_value; }

private:
    QString m_id;
    QString m_label;
    bool    m_value;
};
