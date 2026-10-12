#pragma once

#include <qdraw/drawing/DrawingObject.h>
#include <QObject>
#include <QVector>
#include <memory>
#include <vector>

/// Owns and orders all committed DrawingObject instances.
///
/// The sole source of truth for drawn content.
/// DrawingCanvas reads objects via DrawingController::document() during paint().
/// Commands mutate the document via add / removeAt / insertAt.
///
/// Does NOT render, hit-test, preview, or manage history.
class DrawingDocument : public QObject {
    Q_OBJECT
public:
    explicit DrawingDocument(QObject* parent = nullptr);
    ~DrawingDocument() override = default;

    /// Append object to the top of the stacking order. Takes ownership.
    /// Emits changed().
    void add(std::unique_ptr<DrawingObject> object);

    /// Remove and return the object at index, transferring ownership to caller.
    /// Used by RemoveObjectsCommand::redo(). Emits changed().
    std::unique_ptr<DrawingObject> removeAt(int index);

    /// Insert object at a specific stacking index.
    /// Used by RemoveObjectsCommand::undo() to restore original stacking order.
    /// Emits changed().
    void insertAt(int index, std::unique_ptr<DrawingObject> object);

    /// Number of committed objects.
    int count() const;

    /// Object at index. Non-owning.
    DrawingObject* at(int index) const;

    /// Find index of object. Returns -1 if not found.
    int indexOf(const DrawingObject* object) const;

    /// Non-owning view of all objects in stacking order.
    /// Called by DrawingCanvas::paint() on every frame via controller->document().
    QVector<DrawingObject*> objects() const;

    /// Remove all objects from document.
    void clear();

signals:
    /// Emitted after any structural change.
    /// DrawingController::onDocumentChanged() connects this to repaintNeeded().
    void changed();

private:
    std::vector<std::unique_ptr<DrawingObject>> m_objects;
};
