#include <QApplication>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>

#include <vector>

#include <qdraw/canvas/DrawingCanvas.h>
#include <qdraw/controller/DrawingController.h>
#include <qdraw/tools/options/ToolOptionModel.h>
#include <qdraw/config/IConfig.h>
#include <qdraw/config/AppConfig.h>
#include <qdraw/config/ThemeConfig.h>

/// Ensures embedded Qt resource bundles are initialized into the resource virtual filesystem.
static void initResources()
{
    Q_INIT_RESOURCE(app_config);
    Q_INIT_RESOURCE(theme_config);
    Q_INIT_RESOURCE(app_assets);
}

namespace {

/// Registers Qt Quick C++ types and enums used across QML.
void registerQmlTypes()
{
    qmlRegisterType<DrawingCanvas>("QDraw", 1, 0, "DrawingCanvas");
    qmlRegisterUncreatableType<DrawingController>("QDraw", 1, 0, "DrawingController",
        QStringLiteral("Access via controller context property"));
    qmlRegisterUncreatableType<ToolOptionModel>("QDraw", 1, 0, "ToolOptionModel",
        QStringLiteral("Access via controller.activeToolOptions"));
    qmlRegisterUncreatableMetaObject(QDrawOptions::staticMetaObject, "QDraw", 1, 0, "OptionType",
        QStringLiteral("OptionType is an enum namespace"));
}

/// Validates that all required configuration interfaces loaded successfully.
/// Displays a native critical error message and returns false if any config is invalid.
bool validateConfigs(const std::vector<const qdraw::IConfig*>& configs)
{
    for (const auto* config : configs) {
        if (!config || !config->isLoaded()) {
            const QString name = config ? config->configName() : QStringLiteral("Unknown");
            const QString err  = config ? config->errorMessage() : QStringLiteral("Null configuration pointer");
            const QString msg  = QStringLiteral(
                "QDraw failed to start because the %1 could not be loaded.\n\n"
                "%2\n\n"
                "Please rebuild the project so that configurations are embedded."
            ).arg(name, err);

            qCritical().noquote() << msg;
            QMessageBox::critical(nullptr, QStringLiteral("QDraw - Startup Error"), msg);
            return false;
        }
    }
    return true;
}

/// Configures and initializes the top-level application UI engine using the provided configuration interfaces.
/// In Qt Quick, this engine instantiates and manages the root application Window defined in Main.qml.
void setupApplicationEngine(
    QQmlApplicationEngine& engine,
    DrawingController& controller,
    const std::vector<const qdraw::IConfig*>& configs)
{
    // Register controller
    engine.rootContext()->setContextProperty(QStringLiteral("controller"), &controller);

    // Register all configurations to the QML context via their IConfig interface
    for (const auto* config : configs) {
        if (config) {
            config->registerToContext(engine.rootContext());
        }
    }

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        qApp,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("QDraw", "Main");
}

/// Resolves and loads the application icon from embedded resources or filesystem paths.
QIcon loadAppIcon(const QString& iconPath)
{
    if (iconPath.isEmpty()) {
        return {};
    }

    // 1. Embedded resource path inside QDraw module
    const QString resourcePath = QStringLiteral(":/qt/qml/QDraw/") + iconPath;
    if (QFile::exists(resourcePath)) {
        return QIcon(resourcePath);
    }

    // 2. Direct resource path if already starting with :/ or qrc:/
    if (iconPath.startsWith(QLatin1String(":/")) || iconPath.startsWith(QLatin1String("qrc:/"))) {
        return QIcon(iconPath);
    }

    // 3. Relative or absolute filesystem path
    if (QFile::exists(iconPath)) {
        return QIcon(iconPath);
    }

    // 4. Relative to application executable directory
    const QString appDirPath = QDir(QCoreApplication::applicationDirPath()).filePath(iconPath);
    if (QFile::exists(appDirPath)) {
        return QIcon(appDirPath);
    }

    return QIcon(iconPath);
}

}

int main(int argc, char *argv[])
{
    initResources();

    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("QDraw"));
    app.setOrganizationName(QStringLiteral("QDraw"));

    qdraw::AppConfig appConfig;
    qdraw::ThemeConfig themeConfig;

    const std::vector<const qdraw::IConfig*> configs = { &appConfig, &themeConfig };
    if (!validateConfigs(configs)) {
        return 1;
    }

    const QIcon appIcon = loadAppIcon(appConfig.iconPath());
    if (!appIcon.isNull()) {
        app.setWindowIcon(appIcon);
    }

    registerQmlTypes();

    DrawingController controller;
    QQmlApplicationEngine engine;
    setupApplicationEngine(engine, controller, configs);

    if (!appIcon.isNull()) {
        for (QObject* rootObj : engine.rootObjects()) {
            if (auto* window = qobject_cast<QQuickWindow*>(rootObj)) {
                window->setIcon(appIcon);
            }
        }
    }

    return app.exec();
}
