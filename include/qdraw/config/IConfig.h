#pragma once

#include <QString>

class QQmlContext;

namespace qdraw {

/// Pure abstract interface representing a configuration provider.
class IConfig {
public:
    virtual ~IConfig() = default;

    /// Returns true if the configuration was successfully found and parsed.
    [[nodiscard]] virtual bool isLoaded() const = 0;

    /// Returns detailed error description if loading failed.
    [[nodiscard]] virtual QString errorMessage() const = 0;

    /// Human-readable name of the configuration for logs and dialogs.
    [[nodiscard]] virtual QString configName() const = 0;

    /// Injects configuration properties into the QML engine context.
    virtual void registerToContext(QQmlContext* context) const = 0;
};

} // namespace qdraw
