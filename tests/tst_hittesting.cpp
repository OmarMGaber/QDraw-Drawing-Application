#include <QTest>
#include <qdraw/drawing/EllipseObject.h>
#include <qdraw/drawing/FreehandStroke.h>
#include <qdraw/drawing/LineObject.h>
#include <qdraw/drawing/RectangleObject.h>

class TestHitTesting : public QObject {
    Q_OBJECT

private slots:
    void testLineHit() {
        DrawingStyle style;
        style.strokeWidth = 2.0;
        LineObject line(QPointF(0, 0), QPointF(100, 0), style);

        // Near middle of line
        QVERIFY(line.intersectsCircle(QPointF(50, 1), 2.0));

        // Far away
        QVERIFY(!line.intersectsCircle(QPointF(50, 50), 2.0));
    }

    void testRectangleHit() {
        DrawingStyle outlineStyle;
        outlineStyle.fillEnabled = false;
        outlineStyle.strokeWidth = 2.0;
        RectangleObject rectOutline(QRectF(10, 10, 80, 80), outlineStyle);

        // Near top border
        QVERIFY(rectOutline.intersectsCircle(QPointF(50, 11), 3.0));

        // Deep in center of unfilled rectangle (radius too small to touch borders)
        QVERIFY(!rectOutline.intersectsCircle(QPointF(50, 50), 5.0));

        // Filled rectangle
        DrawingStyle fillStyle;
        fillStyle.fillEnabled = true;
        RectangleObject rectFilled(QRectF(10, 10, 80, 80), fillStyle);
        QVERIFY(rectFilled.intersectsCircle(QPointF(50, 50), 5.0));
    }

    void testEllipseHit() {
        DrawingStyle fillStyle;
        fillStyle.fillEnabled = true;
        EllipseObject ellipse(QRectF(0, 0, 100, 100), fillStyle);

        // Center of filled ellipse
        QVERIFY(ellipse.intersectsCircle(QPointF(50, 50), 5.0));

        // Well outside
        QVERIFY(!ellipse.intersectsCircle(QPointF(150, 150), 5.0));
    }

    void testFreehandStrokeHit() {
        DrawingStyle style;
        style.strokeWidth = 2.0;
        QVector<QPointF> points = { QPointF(0, 0), QPointF(50, 50), QPointF(100, 50) };
        FreehandStroke stroke(points, style);

        // Point near (50, 50)
        QVERIFY(stroke.intersectsCircle(QPointF(51, 51), 3.0));

        // Point far away
        QVERIFY(!stroke.intersectsCircle(QPointF(10, 90), 3.0));
    }
};

QTEST_MAIN(TestHitTesting)
#include "tst_hittesting.moc"
