#pragma once

#include <qdraw/config/JsonConfigBase.h>

#include <QColor>
#include <QVariantList>
#include <QVariantMap>

namespace qdraw {

/// Loads and exposes the application color theme embedded as a Qt resource
/// at build-time from config/theme.json.
///
/// Exposes to QML via context property "theme":
///   - Semantic colors: background, surface, surfaceElevated, surfaceHover,
///                      border, borderStrong, textPrimary, textSecondary,
///                      textMuted, textSubtle, accent, accentHover,
///                      controlActive, overlay
///   - palettePresets : QVariantList – list of drawing palette preset colors
class ThemeConfig : public JsonConfigBase {
    Q_OBJECT
    Q_PROPERTY(QVariantMap  theme          READ theme          CONSTANT)
    Q_PROPERTY(QVariantMap  colors         READ colors         CONSTANT)
    Q_PROPERTY(QVariantList palettePresets READ palettePresets CONSTANT)

public:
    explicit ThemeConfig(QObject* parent = nullptr);

    QVariantMap  theme()          const { return m_theme; }
    QVariantMap  colors()         const { return m_colors; }
    QVariantList palettePresets() const { return m_palettePresets; }

    QColor color(const QString& name, const QColor& fallback = {}) const;

    void registerToContext(QQmlContext* context) const override;

protected:
    bool parseJson(const QJsonObject& root) override;

private:
    QVariantMap  m_theme;
    QVariantMap  m_colors;
    QVariantList m_palettePresets;
};

} // namespace qdraw
