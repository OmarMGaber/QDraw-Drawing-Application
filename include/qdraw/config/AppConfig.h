#pragma once

#include <qdraw/config/JsonConfigBase.h>

#include <QVariantList>
#include <QVariantMap>

namespace qdraw {

/// Loads and exposes the application configuration embedded as a Qt resource
/// at build-time from config/app.json.
///
/// Exposes to QML via context properties:
///   - appInfo     : QVariantMap   – name, author, authorUrl, githubUrl, icon
///   - shortcutList: QVariantList  – list of shortcut objects for display & binding
class AppConfig : public JsonConfigBase {
    Q_OBJECT
    Q_PROPERTY(QVariantList shortcuts READ shortcuts CONSTANT)
    Q_PROPERTY(QVariantMap  appInfo   READ appInfo   CONSTANT)
    Q_PROPERTY(QString      iconPath  READ iconPath  CONSTANT)

public:
    explicit AppConfig(QObject* parent = nullptr);

    QVariantList shortcuts() const { return m_shortcuts; }
    QVariantMap  appInfo()   const { return m_appInfo; }
    QString      iconPath()  const;

    void registerToContext(QQmlContext* context) const override;

protected:
    bool parseJson(const QJsonObject& root) override;

private:
    QVariantList m_shortcuts;
    QVariantMap  m_appInfo;
};

} // namespace qdraw
