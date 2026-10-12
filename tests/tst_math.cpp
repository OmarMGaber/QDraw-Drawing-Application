#include <QTest>
#include <qdraw/math/DrawingMath.h>
#include <qdraw/math/MathConstants.h>
#include <cmath>

using namespace qdraw::math;

class TestMath : public QObject {
    Q_OBJECT

private slots:
    void testDistance() {
        QCOMPARE(distance(QPointF(0, 0), QPointF(3, 4)), 5.0);
        QCOMPARE(distance(QPointF(10, 10), QPointF(10, 10)), 0.0);
    }

    void testPointToSegmentDistance() {
        QPointF a(0, 0);
        QPointF b(10, 0);

        // Point perpendicular to midpoint
        QCOMPARE(pointToSegmentDistance(QPointF(5, 5), a, b), 5.0);

        // Point before segment start
        QCOMPARE(pointToSegmentDistance(QPointF(-3, 4), a, b), 5.0);

        // Point after segment end
        QCOMPARE(pointToSegmentDistance(QPointF(13, 4), a, b), 5.0);

        // Point directly on segment
        QCOMPARE(pointToSegmentDistance(QPointF(7, 0), a, b), 0.0);

        // Degenerate segment (a == b)
        QCOMPARE(pointToSegmentDistance(QPointF(3, 4), a, a), 5.0);
    }

    void testThresholdsAndPaddings() {
        // strokeWidth = 4.0: pad = 4.0 * 0.5 + 1.0 = 3.0
        QCOMPARE(calculateStrokePadding(4.0), 3.0);

        // radius = 5.0, strokeWidth = 2.0: threshold = 5.0 + 1.0 = 6.0
        QCOMPARE(calculateHitThreshold(5.0, 2.0), 6.0);

        // strokeWidth = 0.2: half is 0.1, but min dot radius is 0.5
        QCOMPARE(calculateDotRadius(0.2), 0.5);

        // strokeWidth = 4.0: dot radius = 2.0
        QCOMPARE(calculateDotRadius(4.0), 2.0);

        // Gestures
        QVERIFY(isGestureDragSignificant(QPointF(0, 0), QPointF(1.0, 0)));
        QVERIFY(!isGestureDragSignificant(QPointF(0, 0), QPointF(0.5, 0.5)));
    }

    void testLineMath() {
        QPointF start(10, 20);
        QPointF end(50, 20);
        qreal strokeWidth = 2.0;

        QRectF bounds = calculateLineBoundingRect(start, end, strokeWidth);
        // strokeWidth = 2 -> pad = 2.0
        QCOMPARE(bounds, QRectF(8, 18, 44, 4));

        QVERIFY(lineIntersectsCircle(start, end, strokeWidth, QPointF(30, 21), 2.0));
        QVERIFY(!lineIntersectsCircle(start, end, strokeWidth, QPointF(30, 50), 2.0));
    }

    void testRectangleMath() {
        QRectF rect(10, 10, 80, 80);
        qreal strokeWidth = 2.0;

        QRectF bounds = calculateRectangleBoundingRect(rect, strokeWidth);
        // pad = 2.0
        QCOMPARE(bounds, QRectF(8, 8, 84, 84));

        // Outline check (unfilled)
        QVERIFY(rectangleIntersectsCircle(rect, strokeWidth, false, QPointF(50, 11), 3.0));
        QVERIFY(!rectangleIntersectsCircle(rect, strokeWidth, false, QPointF(50, 50), 5.0));

        // Filled check
        QVERIFY(rectangleIntersectsCircle(rect, strokeWidth, true, QPointF(50, 50), 5.0));
    }

    void testEllipseMath() {
        QRectF rect(0, 0, 100, 100);
        qreal strokeWidth = 2.0;

        QRectF bounds = calculateEllipseBoundingRect(rect, strokeWidth);
        QCOMPARE(bounds, QRectF(-2, -2, 104, 104));

        QVERIFY(isPointInsideEllipse(QPointF(50, 50), rect));
        QVERIFY(!isPointInsideEllipse(QPointF(1, 1), rect));

        // Center hit for filled ellipse
        QVERIFY(ellipseIntersectsCircle(rect, strokeWidth, true, QPointF(50, 50), 5.0));
        // Outside
        QVERIFY(!ellipseIntersectsCircle(rect, strokeWidth, true, QPointF(150, 150), 5.0));
    }

    void testCircleMath() {
        // Drag in (+, +) quadrant
        QRectF box1 = calculateCircleBoundingBox(QPointF(10, 20), QPointF(50, 40));
        // dx = 40, dy = 20 -> side = 40, origin = (10, 20)
        QCOMPARE(box1, QRectF(10, 20, 40, 40));

        // Drag in (-, -) quadrant
        QRectF box2 = calculateCircleBoundingBox(QPointF(100, 100), QPointF(70, 50));
        // dx = -30, dy = -50 -> side = 50, left = 100 - 50 = 50, top = 100 - 50 = 50
        QCOMPARE(box2, QRectF(50, 50, 50, 50));

        // Drag in (+, -) quadrant
        QRectF box3 = calculateCircleBoundingBox(QPointF(10, 100), QPointF(50, 80));
        // dx = 40, dy = -20 -> side = 40, left = 10, top = 100 - 40 = 60
        QCOMPARE(box3, QRectF(10, 60, 40, 40));

        // Drag in (-, +) quadrant
        QRectF box4 = calculateCircleBoundingBox(QPointF(100, 10), QPointF(60, 40));
        // dx = -40, dy = 30 -> side = 40, left = 100 - 40 = 60, top = 10
        QCOMPARE(box4, QRectF(60, 10, 40, 40));

        // Zero-length drag
        QRectF box5 = calculateCircleBoundingBox(QPointF(10, 10), QPointF(10, 10));
        QCOMPARE(box5, QRectF(10, 10, 0, 0));
    }

    void testPointsAndSplineMath() {
        QVector<QPointF> points = { QPointF(0, 0), QPointF(50, 50), QPointF(100, 50) };
        qreal strokeWidth = 2.0;

        QRectF bounds = calculatePointsBoundingRect(points, strokeWidth);
        QCOMPARE(bounds, QRectF(-2, -2, 104, 54));

        QPainterPath smoothPath = createSmoothPath(points);
        QVERIFY(!smoothPath.isEmpty());
        QCOMPARE(smoothPath.elementCount() > 0, true);

        QVERIFY(strokeIntersectsCircle(points, bounds, strokeWidth, QPointF(51, 51), 3.0));
        QVERIFY(!strokeIntersectsCircle(points, bounds, strokeWidth, QPointF(10, 90), 3.0));
    }
};

QTEST_MAIN(TestMath)
#include "tst_math.moc"
