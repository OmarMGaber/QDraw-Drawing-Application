#include <qdraw/config/JsonConfigBase.h>

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

namespace qdraw {

JsonConfigBase::JsonConfigBase(QString resourcePath,
                               QString fallbackPath,
                               QString configName,
                               QObject* parent)
    : QObject(parent)
    , m_resourcePath(std::move(resourcePath))
    , m_fallbackPath(std::move(fallbackPath))
    , m_configName(std::move(configName)) {
}

void JsonConfigBase::setParseError(const QString& errorMsg) {
    m_error = errorMsg;
    m_loaded = false;
    qCritical().noquote() << QStringLiteral("[%1] FATAL: %2").arg(m_configName, m_error);
}

bool JsonConfigBase::load() {
    QString path = m_resourcePath;
    QFile file(path);

    if (!file.exists()) {
        file.setFileName(m_fallbackPath);
        path = m_fallbackPath;
        if (!file.exists()) {
            setParseError(QStringLiteral(
                "Config resource not found.\n"
                "Expected embedded path : %1\n"
                "Fallback filesystem path: %2\n\n"
                "Re-run CMake to embed %3 into the binary."
            ).arg(m_resourcePath, m_fallbackPath, m_configName));
            return false;
        }
        qWarning().noquote() << QStringLiteral("[%1] Resource not found, falling back to %2")
                                    .arg(m_configName, path);
    }

    if (!file.open(QIODevice::ReadOnly)) {
        setParseError(QStringLiteral("Cannot open config file: %1\nError: %2")
                          .arg(path, file.errorString()));
        return false;
    }

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        setParseError(QStringLiteral("Failed to parse %1:\n%2 (offset %3)")
                          .arg(path, parseError.errorString())
                          .arg(parseError.offset));
        return false;
    }

    if (!doc.isObject()) {
        setParseError(QStringLiteral("Config file %1 must contain a root JSON object.").arg(path));
        return false;
    }

    const QJsonObject root = doc.object();
    if (!parseJson(root)) {
        if (m_error.isEmpty()) {
            setParseError(QStringLiteral("Invalid structure in config file %1.").arg(path));
        }
        return false;
    }

    m_loaded = true;
    m_error.clear();
    return true;
}

} // namespace qdraw
