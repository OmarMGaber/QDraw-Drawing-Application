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
        ctx.lineWidth = 1.3;
        ctx.lineCap = "round";
        ctx.lineJoin = "round";
        ctx.beginPath();
        ctx.moveTo(4, 3);
        ctx.lineTo(11, 3);
        ctx.lineTo(9, 11);
        ctx.lineTo(2, 11);
        ctx.closePath();
        ctx.stroke();
        ctx.beginPath();
        ctx.moveTo(7, 3);
        ctx.lineTo(5, 11);
        ctx.stroke();
    }
}
