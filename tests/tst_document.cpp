#include <QTest>
#include <qdraw/drawing/DrawingDocument.h>
#include <qdraw/drawing/RectangleObject.h>

class TestDocument : public QObject {
    Q_OBJECT

private slots:
    void testAddAndCount() {
        DrawingDocument doc;
        QCOMPARE(doc.count(), 0);

        int changedSignalCount = 0;
        connect(&doc, &DrawingDocument::changed, [&]() {
            ++changedSignalCount;
        });

        DrawingStyle style;
        auto rect = std::make_unique<RectangleObject>(QRectF(0, 0, 100, 100), style);
        doc.add(std::move(rect));

        QCOMPARE(doc.count(), 1);
        QCOMPARE(changedSignalCount, 1);
        QVERIFY(doc.at(0) != nullptr);
    }

    void testRemoveAndInsert() {
        DrawingDocument doc;
        DrawingStyle style;

        doc.add(std::make_unique<RectangleObject>(QRectF(0, 0, 10, 10), style));
        doc.add(std::make_unique<RectangleObject>(QRectF(10, 10, 10, 10), style));
        doc.add(std::make_unique<RectangleObject>(QRectF(20, 20, 10, 10), style));
        QCOMPARE(doc.count(), 3);

        auto removed = doc.removeAt(1);
        QVERIFY(removed != nullptr);
        QCOMPARE(doc.count(), 2);

        doc.insertAt(1, std::move(removed));
        QCOMPARE(doc.count(), 3);
    }

    void testClear() {
        DrawingDocument doc;
        DrawingStyle style;
        doc.add(std::make_unique<RectangleObject>(QRectF(0, 0, 10, 10), style));
        doc.add(std::make_unique<RectangleObject>(QRectF(10, 10, 10, 10), style));

        doc.clear();
        QCOMPARE(doc.count(), 0);
    }
};

QTEST_MAIN(TestDocument)
#include "tst_document.moc"
