import QtQuick

Canvas {
    id: root

    property color strokeColor: theme.textPrimary

    implicitWidth: 11
    implicitHeight: 11
    width: implicitWidth
    height: implicitHeight

    onStrokeColorChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d");
        ctx.reset();
        ctx.strokeStyle = root.strokeColor;
        ctx.lineWidth = 1.3;
        ctx.lineCap = "round";
        // Arc
        ctx.beginPath();
        ctx.arc(5, 6, 4, Math.PI * 0.25, Math.PI * 1.1, true);
        ctx.stroke();
        // Arrow head
        ctx.beginPath();
        ctx.moveTo(9.5, 4);
        ctx.lineTo(8.5, 7.5);
        ctx.lineTo(5, 7);
        ctx.stroke();
    }
}
