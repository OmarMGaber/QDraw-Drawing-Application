#pragma once

#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/drawing/DrawingObject.h>
#include <QUndoCommand>
#include <memory>
#include <vector>

/// Undoable command for removing one or more objects in a single eraser gesture.
///
/// Stores each removed object with its original stacking index.
///
/// REDO order: remove entries in DESCENDING index order so earlier removals
/// do not shift the indices of later entries.
///
/// UNDO order: reinsert entries in ASCENDING index order so each insertion
/// lands at the correct position.
class RemoveObjectsCommand : public QUndoCommand {
public:
    struct Entry {
        int originalIndex = -1;
        std::unique_ptr<DrawingObject> object;
    };

    /// @param entries   Removed objects with their original stacking indices.
    /// @param document  Document the objects were removed from.
    RemoveObjectsCommand(std::vector<Entry> entries, DrawingDocument* document);
    ~RemoveObjectsCommand() override = default;

    /// Remove objects from document in descending index order.
    void redo() override;

    /// Reinsert objects into document in ascending index order.
    void undo() override;

private:
    std::vector<Entry> m_entries;
    DrawingDocument*   m_document = nullptr;
};
