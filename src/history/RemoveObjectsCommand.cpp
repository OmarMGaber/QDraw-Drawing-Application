#include <qdraw/history/RemoveObjectsCommand.h>
#include <algorithm>

RemoveObjectsCommand::RemoveObjectsCommand(std::vector<Entry> entries, DrawingDocument* document)
    : m_entries(std::move(entries))
    , m_document(document) {
    setText(QStringLiteral("Remove Objects"));
    std::sort(m_entries.begin(), m_entries.end(), [](const Entry& a, const Entry& b) {
        return a.originalIndex < b.originalIndex;
    });
}

void RemoveObjectsCommand::redo() {
    if (!m_document) {
        return;
    }
    // Remove descending by index so that earlier indices remain stable
    for (auto it = m_entries.rbegin(); it != m_entries.rend(); ++it) {
        if (!it->object) {
            it->object = m_document->removeAt(it->originalIndex);
        }
    }
}

void RemoveObjectsCommand::undo() {
    if (!m_document) {
        return;
    }
    // Insert ascending by index so that items return to their original stacking positions
    for (auto& entry : m_entries) {
        if (entry.object) {
            m_document->insertAt(entry.originalIndex, std::move(entry.object));
        }
    }
}
