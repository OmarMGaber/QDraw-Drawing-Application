import QtQuick

Canvas {
    id: root

    property color strokeColor: theme.textPrimary

    implicitWidth: 14
    implicitHeight: 14

    onStrokeColorChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d");
        ctx.reset();
        ctx.strokeStyle = root.strokeColor;
        ctx.fillStyle = root.strokeColor;
        ctx.lineWidth = 1.3;
        ctx.lineCap = "round";
        ctx.lineJoin = "round";
        ctx.beginPath();
        ctx.moveTo(3, 11);
        ctx.lineTo(10, 4);
        ctx.lineTo(12, 6);
        ctx.lineTo(5, 13);
        ctx.closePath();
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(3, 11);
        ctx.lineTo(1.5, 12.5);
        ctx.lineTo(4, 12);
        ctx.closePath();
        ctx.fill();
    }
}
