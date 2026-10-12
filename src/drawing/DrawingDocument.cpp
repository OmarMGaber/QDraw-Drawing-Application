#include <qdraw/drawing/DrawingDocument.h>

DrawingDocument::DrawingDocument(QObject* parent)
    : QObject(parent) {}

void DrawingDocument::add(std::unique_ptr<DrawingObject> object) {
    if (!object) {
        return;
    }
    m_objects.push_back(std::move(object));
    emit changed();
}

std::unique_ptr<DrawingObject> DrawingDocument::removeAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_objects.size())) {
        return nullptr;
    }
    auto it = m_objects.begin() + index;
    std::unique_ptr<DrawingObject> removed = std::move(*it);
    m_objects.erase(it);
    emit changed();
    return removed;
}

void DrawingDocument::insertAt(int index, std::unique_ptr<DrawingObject> object) {
    if (!object) {
        return;
    }
    int clampedIndex = std::clamp(index, 0, static_cast<int>(m_objects.size()));
    m_objects.insert(m_objects.begin() + clampedIndex, std::move(object));
    emit changed();
}

int DrawingDocument::count() const {
    return static_cast<int>(m_objects.size());
}

DrawingObject* DrawingDocument::at(int index) const {
    if (index < 0 || index >= static_cast<int>(m_objects.size())) {
        return nullptr;
    }
    return m_objects[static_cast<size_t>(index)].get();
}

int DrawingDocument::indexOf(const DrawingObject* object) const {
    for (size_t i = 0; i < m_objects.size(); ++i) {
        if (m_objects[i].get() == object) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

QVector<DrawingObject*> DrawingDocument::objects() const {
    QVector<DrawingObject*> list;
    list.reserve(static_cast<qsizetype>(m_objects.size()));
    for (const auto& obj : m_objects) {
        list.append(obj.get());
    }
    return list;
}

void DrawingDocument::clear() {
    if (m_objects.empty()) {
        return;
    }
    m_objects.clear();
    emit changed();
}
