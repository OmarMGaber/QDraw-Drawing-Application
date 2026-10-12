#include <qdraw/history/AddObjectCommand.h>

AddObjectCommand::AddObjectCommand(std::unique_ptr<DrawingObject> object,
                                   DrawingDocument* document)
    : m_object(std::move(object))
    , m_document(document) {
    setText(QStringLiteral("Add Object"));
}

void AddObjectCommand::redo() {
    if (!m_document || !m_object) {
        return;
    }
    m_insertedIndex = m_document->count();
    m_document->add(std::move(m_object));
}

void AddObjectCommand::undo() {
    if (!m_document || m_insertedIndex < 0) {
        return;
    }
    m_object = m_document->removeAt(m_insertedIndex);
}
