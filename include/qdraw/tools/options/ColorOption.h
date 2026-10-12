#pragma once

#include <qdraw/tools/options/ToolOption.h>

/// Configurable color option.
/// Exposed to QML as a color picker button with dialog.
class ColorOption final : public ToolOption {
    Q_OBJECT
public:
    ColorOption(QString id, QString label, QColor defaultValue, QObject* parent = nullptr);

    QString id() const override { return m_id; }
    QString label() const override { return m_label; }
    QDrawOptions::OptionType type() const override { return QDrawOptions::OptionType::Color; }
    QVariant value() const override { return m_color; }
    void setValue(const QVariant& v) override;

    /// Typed getter used inside Tool subclasses to populate DrawingStyle.
    QColor color() const { return m_color; }

private:
    QString m_id;
    QString m_label;
    QColor  m_color;
};
