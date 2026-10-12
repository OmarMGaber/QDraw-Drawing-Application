import QtQuick

Canvas {
    id: root

    property color strokeColor: theme.textPrimary

    implicitWidth: 13
    implicitHeight: 13
    width: implicitWidth
    height: implicitHeight

    onStrokeColorChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d");
        ctx.reset();
        ctx.strokeStyle = root.strokeColor;
        ctx.fillStyle = root.strokeColor;
        ctx.lineWidth = 1.3;
        ctx.lineCap = "round";
        ctx.lineJoin = "round";

        // Circular boundary
        ctx.beginPath();
        ctx.arc(6.5, 6.5, 5.5, 0, 2 * Math.PI);
        ctx.stroke();

        // Question mark upper curve
        ctx.beginPath();
        ctx.arc(6.5, 4.8, 1.8, Math.PI, 0, false);
        ctx.lineTo(6.5, 7.3);
        ctx.stroke();

        // Question mark dot
        ctx.beginPath();
        ctx.arc(6.5, 9.4, 0.75, 0, 2 * Math.PI);
        ctx.fill();
    }
}
