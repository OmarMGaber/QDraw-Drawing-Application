#include <QTest>
#include <QUndoStack>
#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/drawing/RectangleObject.h>
#include <qdraw/history/AddObjectCommand.h>
#include <qdraw/history/RemoveObjectsCommand.h>

class TestHistory : public QObject {
    Q_OBJECT

private slots:
    void testAddUndoRedo() {
        DrawingDocument doc;
        QUndoStack stack;

        DrawingStyle style;
        auto rect = std::make_unique<RectangleObject>(QRectF(0, 0, 50, 50), style);
        stack.push(new AddObjectCommand(std::move(rect), &doc));

        QCOMPARE(doc.count(), 1);
        QVERIFY(stack.canUndo());
        QVERIFY(!stack.canRedo());

        stack.undo();
        QCOMPARE(doc.count(), 0);
        QVERIFY(!stack.canUndo());
        QVERIFY(stack.canRedo());

        stack.redo();
        QCOMPARE(doc.count(), 1);
        QVERIFY(stack.canUndo());
    }

    void testRemoveObjectsDescendingRedoAscendingUndo() {
        DrawingDocument doc;
        QUndoStack stack;
        DrawingStyle style;

        // Add 5 distinct rectangles with known coordinates
        for (int i = 0; i < 5; ++i) {
            auto rect = std::make_unique<RectangleObject>(QRectF(i * 10, i * 10, 10, 10), style);
            doc.add(std::move(rect));
        }
        QCOMPARE(doc.count(), 5);

        // Store pointers to check object identity
        DrawingObject* obj1 = doc.at(1);
        DrawingObject* obj3 = doc.at(3);

        // Prepare removal entries for indices 1 and 3
        std::vector<RemoveObjectsCommand::Entry> entries;
        entries.push_back({ 1, nullptr });
        entries.push_back({ 3, nullptr });

        stack.push(new RemoveObjectsCommand(std::move(entries), &doc));

        // After removal: doc should have 3 items left
        QCOMPARE(doc.count(), 3);

        // Undo: should restore 5 items in original positions
        stack.undo();
        QCOMPARE(doc.count(), 5);
        QCOMPARE(doc.at(1), obj1);
        QCOMPARE(doc.at(3), obj3);

        // Redo: should remove them again cleanly
        stack.redo();
        QCOMPARE(doc.count(), 3);
    }

    void testBranchDiscard() {
        DrawingDocument doc;
        QUndoStack stack;
        DrawingStyle style;

        stack.push(new AddObjectCommand(std::make_unique<RectangleObject>(QRectF(0, 0, 10, 10), style), &doc));
        stack.push(new AddObjectCommand(std::make_unique<RectangleObject>(QRectF(10, 10, 10, 10), style), &doc));
        QCOMPARE(doc.count(), 2);

        stack.undo();
        QCOMPARE(doc.count(), 1);
        QVERIFY(stack.canRedo());

        // Perform new edit
        stack.push(new AddObjectCommand(std::make_unique<RectangleObject>(QRectF(20, 20, 10, 10), style), &doc));
        QCOMPARE(doc.count(), 2);
        // Old redo branch must be discarded
        QVERIFY(!stack.canRedo());
    }
};

QTEST_MAIN(TestHistory)
#include "tst_history.moc"
