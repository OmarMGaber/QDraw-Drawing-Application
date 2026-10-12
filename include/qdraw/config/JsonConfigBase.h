#pragma once

#include <qdraw/config/IConfig.h>

#include <QJsonObject>
#include <QObject>
#include <QString>

namespace qdraw {

/// Base class implementing the Template Method pattern for JSON-backed configs.
///
/// Handles the common workflow:
///   1. Resource and source-tree fallback path resolution
///   2. File opening and reading
///   3. JSON parsing and error reporting
///   4. Root JSON object validation
///   5. Delegating schema extraction to parseJson(root) in subclasses
class JsonConfigBase : public QObject, public IConfig {
    Q_OBJECT
    Q_PROPERTY(bool    isLoaded     READ isLoaded     CONSTANT)
    Q_PROPERTY(QString errorMessage READ errorMessage CONSTANT)

public:
    explicit JsonConfigBase(QString resourcePath,
                            QString fallbackPath,
                            QString configName,
                            QObject* parent = nullptr);

    [[nodiscard]] bool isLoaded() const override { return m_loaded; }
    [[nodiscard]] QString errorMessage() const override { return m_error; }
    [[nodiscard]] QString configName() const override { return m_configName; }

    /// Template method: executes the common loading, file reading, and JSON parsing pipeline.
    bool load();

protected:
    /// Subclasses must implement this to extract their specific properties from the root JSON object.
    /// Returns true on success, or false (setting an error message via setParseError) on failure.
    virtual bool parseJson(const QJsonObject& root) = 0;

    void setParseError(const QString& errorMsg);

private:
    QString m_resourcePath;
    QString m_fallbackPath;
    QString m_configName;
    bool    m_loaded = false;
    QString m_error;
};

} // namespace qdraw
