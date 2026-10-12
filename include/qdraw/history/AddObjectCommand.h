#pragma once

#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/drawing/DrawingObject.h>
#include <QUndoCommand>
#include <memory>

/// Undoable command for adding a single DrawingObject to the document.
///
/// Owns the object via unique_ptr and transfers ownership to/from the document
/// on redo/undo — no deep copies are ever made.
class AddObjectCommand : public QUndoCommand {
public:
    /// @param object   New object. This command takes ownership.
    /// @param document Document to add it to.
    AddObjectCommand(std::unique_ptr<DrawingObject> object,
                     DrawingDocument* document);
    ~AddObjectCommand() override = default;

    /// Transfer object ownership into the document (append to top).
    /// Records m_insertedIndex for undo's removeAt().
    void redo() override;

    /// Transfer object ownership back from the document.
    void undo() override;

private:
    std::unique_ptr<DrawingObject> m_object;
    DrawingDocument*               m_document = nullptr;
    int                            m_insertedIndex = -1;
};
