cmake_minimum_required(VERSION 3.16)

project(QDraw VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Quick Widgets)

qt_standard_project_setup(REQUIRES 6.5)

# Core Static Library
add_library(qdraw_core STATIC
    include/qdraw/canvas/DrawingCanvas.h
    include/qdraw/controller/DrawingController.h
    include/qdraw/drawing/DrawingDocument.h
    include/qdraw/drawing/DrawingObject.h
    include/qdraw/drawing/DrawingStyle.h
    include/qdraw/drawing/EllipseObject.h
    include/qdraw/drawing/FreehandStroke.h
    include/qdraw/drawing/LineObject.h
    include/qdraw/drawing/RectangleObject.h
    include/qdraw/history/AddObjectCommand.h
    include/qdraw/history/RemoveObjectsCommand.h
    include/qdraw/math/DrawingMath.h
    include/qdraw/math/MathConstants.h
    include/qdraw/tools/EraserTool.h
    include/qdraw/tools/EllipseTool.h
    include/qdraw/tools/CircleTool.h
    include/qdraw/tools/LineTool.h
    include/qdraw/tools/PenTool.h
    include/qdraw/tools/RectangleTool.h
    include/qdraw/tools/ShapeTool.h
    include/qdraw/tools/Tool.h
    include/qdraw/tools/options/BoolOption.h
    include/qdraw/tools/options/ColorOption.h
    include/qdraw/tools/options/RealOption.h
    include/qdraw/tools/options/ToolOption.h
    include/qdraw/tools/options/ToolOptionModel.h
    include/qdraw/config/IConfig.h
    include/qdraw/config/JsonConfigBase.h
    include/qdraw/config/AppConfig.h
    include/qdraw/config/ThemeConfig.h

    src/canvas/DrawingCanvas.cpp
    src/controller/DrawingController.cpp
    src/drawing/DrawingDocument.cpp
    src/drawing/EllipseObject.cpp
    src/drawing/FreehandStroke.cpp
    src/drawing/LineObject.cpp
    src/drawing/RectangleObject.cpp
    src/history/AddObjectCommand.cpp
    src/history/RemoveObjectsCommand.cpp
    src/math/DrawingMath.cpp
    src/tools/EraserTool.cpp
    src/tools/EllipseTool.cpp
    src/tools/CircleTool.cpp
    src/tools/LineTool.cpp
    src/tools/PenTool.cpp
    src/tools/RectangleTool.cpp
    src/tools/ShapeTool.cpp
    src/tools/Tool.cpp
    src/tools/options/BoolOption.cpp
    src/tools/options/ColorOption.cpp
    src/tools/options/RealOption.cpp
    src/tools/options/ToolOptionModel.cpp
    src/config/JsonConfigBase.cpp
    src/config/AppConfig.cpp
    src/config/ThemeConfig.cpp
)

target_include_directories(qdraw_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(qdraw_core PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Widgets
)

# Application Executable
qt_add_executable(appQDraw
    src/main.cpp
)

# Embed config/app.json as a Qt resource inside the QML module
qt_add_resources(appQDraw "app_config"
    PREFIX "/qt/qml/QDraw"
    FILES
        config/app.json
)

# Embed config/theme.json as a Qt resource inside the QML module
qt_add_resources(appQDraw "theme_config"
    PREFIX "/qt/qml/QDraw"
    FILES
        config/theme.json
)

# Embed assets/brand/logo.png as a Qt resource inside the QML module
qt_add_resources(appQDraw "app_assets"
    PREFIX "/qt/qml/QDraw"
    FILES
        assets/brand/logo.png
)

qt_add_qml_module(appQDraw
    URI QDraw
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/components/TopBar.qml
        qml/components/ToolSelector.qml
        qml/components/PropertiesPanel.qml
        qml/components/LabeledSlider.qml
        qml/components/ColorPickerButton.qml
        qml/components/ToggleSwitch.qml
        qml/components/HelpDialog.qml
        qml/icons/PenIcon.qml
        qml/icons/EraserIcon.qml
        qml/icons/HelpIcon.qml
        qml/icons/LineIcon.qml
        qml/icons/UndoIcon.qml
        qml/icons/RedoIcon.qml
        qml/icons/RectangleIcon.qml
        qml/icons/EllipseIcon.qml
        qml/icons/CircleIcon.qml
)

set_target_properties(appQDraw PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQDraw PRIVATE
    qdraw_core
    Qt6::Quick
    Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appQDraw
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
