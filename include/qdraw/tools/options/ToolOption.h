#pragma once

#include <QColor>
#include <QObject>
#include <QString>
#include <QVariant>

namespace QDrawOptions {
    Q_NAMESPACE
    enum class OptionType { Color, Real, Bool };
    Q_ENUM_NS(OptionType)
}

/// Abstract base for a single configurable tool option.
///
/// Carries a unique id, human-readable label, an OptionType discriminator for QML
/// control selection, and a current QVariant value.
/// Subclasses provide type-specific metadata (e.g. min/max/step for RealOption).
///
/// Owned exclusively by the Tool that registered it.
/// Must NOT be stored in DrawingObjects — use DrawingStyle for frozen snapshots.
class ToolOption : public QObject {
    Q_OBJECT
public:
    explicit ToolOption(QObject* parent = nullptr)
        : QObject(parent) {}
    ~ToolOption() override = default;

    /// Unique key used by DrawingController::setOption() to route writes.
    virtual QString id() const = 0;

    /// Human-readable label shown in PropertiesPanel.
    virtual QString label() const = 0;

    /// Discriminator used by QML Loader to choose the correct control delegate.
    virtual QDrawOptions::OptionType type() const = 0;

    /// Current value exposed as a QVariant via ToolOptionModel's Value role.
    virtual QVariant value() const = 0;

    /// Update the current value.
    virtual void setValue(const QVariant& v) = 0;

signals:
    /// Emitted after setValue(). ToolOptionModel connects to this to emit
    /// granular dataChanged() for only the affected row.
    void valueChanged();
};
