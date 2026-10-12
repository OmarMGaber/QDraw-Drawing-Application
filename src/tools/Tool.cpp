#include <qdraw/tools/Tool.h>
#include <qdraw/tools/options/BoolOption.h>
#include <qdraw/tools/options/ColorOption.h>
#include <qdraw/tools/options/RealOption.h>

Tool::Tool(QObject* parent)
    : QObject(parent) {}

void Tool::setContext(DrawingDocument* document, QUndoStack* undoStack) {
    m_document = document;
    m_undoStack = undoStack;
}

QVector<ToolOption*> Tool::options() const {
    QVector<ToolOption*> list;
    list.reserve(static_cast<qsizetype>(m_options.size()));
    for (const auto& opt : m_options) {
        list.append(opt.get());
    }
    return list;
}

void Tool::setOptionValue(const QString& id, const QVariant& value) {
    auto it = m_optionIndex.find(id);
    if (it != m_optionIndex.end() && *it) {
        (*it)->setValue(value);
        emit optionValueChanged(id);
    }
}

void Tool::registerOption(std::unique_ptr<ToolOption> opt) {
    if (!opt) {
        return;
    }
    QString id = opt->id();
    m_optionIndex.insert(id, opt.get());
    m_options.push_back(std::move(opt));
}

QColor Tool::colorOption(const QString& id) const {
    auto it = m_optionIndex.find(id);
    if (it != m_optionIndex.end()) {
        if (auto* co = qobject_cast<ColorOption*>(*it)) {
            return co->color();
        }
    }
    return Qt::black;
}

qreal Tool::realOption(const QString& id) const {
    auto it = m_optionIndex.find(id);
    if (it != m_optionIndex.end()) {
        if (auto* ro = qobject_cast<RealOption*>(*it)) {
            return ro->real();
        }
    }
    return 0.0;
}

bool Tool::boolOption(const QString& id) const {
    auto it = m_optionIndex.find(id);
    if (it != m_optionIndex.end()) {
        if (auto* bo = qobject_cast<BoolOption*>(*it)) {
            return bo->boolean();
        }
    }
    return false;
}
