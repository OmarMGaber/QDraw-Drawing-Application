#include <qdraw/config/ThemeConfig.h>

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlContext>

namespace qdraw {

static constexpr char kResourcePath[] = ":/qt/qml/QDraw/config/theme.json";
static constexpr char kFallbackPath[]  = "config/theme.json";

ThemeConfig::ThemeConfig(QObject* parent)
    : JsonConfigBase(kResourcePath, kFallbackPath, QStringLiteral("ThemeConfig"), parent) {
    load();
}

QColor ThemeConfig::color(const QString& name, const QColor& fallback) const {
    const auto it = m_colors.find(name);
    if (it != m_colors.end() && it.value().canConvert<QString>()) {
        const QColor c(it.value().toString());
        if (c.isValid()) {
            return c;
        }
    }
    return fallback;
}

bool ThemeConfig::parseJson(const QJsonObject& root) {
    if (!root.contains(QStringLiteral("theme"))) {
        setParseError(QStringLiteral("Theme config is missing required key \"theme\"."));
        return false;
    }

    m_colors = root.value(QStringLiteral("theme")).toObject().toVariantMap();

    if (root.contains(QStringLiteral("palettePresets"))) {
        m_palettePresets = root.value(QStringLiteral("palettePresets")).toArray().toVariantList();
    }

    m_theme = m_colors;
    m_theme.insert(QStringLiteral("palettePresets"), m_palettePresets);
    m_theme.insert(QStringLiteral("colors"), m_colors);

    qInfo() << "[ThemeConfig] Loaded" << m_colors.size() << "theme tokens.";
    return true;
}

void ThemeConfig::registerToContext(QQmlContext* context) const {
    if (context) {
        context->setContextProperty(QStringLiteral("theme"), m_theme);
    }
}

} // namespace qdraw
