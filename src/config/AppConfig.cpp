#include <qdraw/config/AppConfig.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlContext>

namespace qdraw {

static constexpr char kResourcePath[] = ":/qt/qml/QDraw/config/app.json";
static constexpr char kFallbackPath[]  = "config/app.json";

AppConfig::AppConfig(QObject* parent)
    : JsonConfigBase(kResourcePath, kFallbackPath, QStringLiteral("AppConfig"), parent) {
    load();
}

bool AppConfig::parseJson(const QJsonObject& root) {
    if (!root.contains(QStringLiteral("app")) || !root.contains(QStringLiteral("shortcuts"))) {
        setParseError(QStringLiteral(
            "App config is missing required keys: \"app\" and \"shortcuts\"."
        ));
        return false;
    }

    m_appInfo   = root.value(QStringLiteral("app")).toObject().toVariantMap();
    m_shortcuts = root.value(QStringLiteral("shortcuts")).toArray().toVariantList();

    qInfo() << "[AppConfig] Loaded" << m_shortcuts.size() << "shortcuts.";
    return true;
}

void AppConfig::registerToContext(QQmlContext* context) const {
    if (context) {
        context->setContextProperty(QStringLiteral("shortcutList"), m_shortcuts);
        context->setContextProperty(QStringLiteral("appInfo"),      m_appInfo);
    }
}

QString AppConfig::iconPath() const {
    return m_appInfo.value(QStringLiteral("icon")).toString();
}

} // namespace qdraw
