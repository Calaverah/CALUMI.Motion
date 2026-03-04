#include "stdafx.h"
#include "AgxConnectionPainter.h"

#include "Models/AgxGraphModel.h"
#include "Models/AgxGraphicsScene.h"
#include "Models/AgxConnectionGraphicsObject.h"
#include "Models/AgxConnectionState.h"

#include "AgxStyleCollection.h"

#include <QtGui/QIcon>


QPainterPath AgxConnectionPainter::cubicPath(AgxConnectionGraphicsObject const& connection) const
{
    QPointF const& in = connection.endPoint(AgxPortType::In);
    QPointF const& out = connection.endPoint(AgxPortType::Out);

    auto const c1c2 = connection.pointsC1C2();

    // cubic spline
    QPainterPath cubic(out);

    cubic.cubicTo(c1c2.first, c1c2.second, in);

    return cubic;
}

void AgxConnectionPainter::drawSketchLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const
{
    AgxConnectionState const& state = cgo.connectionState();

    if (state.requiresPort()) {

        QPen pen;
        pen.setWidth(static_cast<int>(_connectionStyle.constructionLineWidth()));
        pen.setColor(_connectionStyle.constructionColor());
        pen.setStyle(Qt::DashLine);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        auto cubic = cubicPath(cgo);

        // cubic spline
        painter->drawPath(cubic);
    }
}

void AgxConnectionPainter::drawHoveredOrSelected(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const
{
    bool const hovered = cgo.connectionState().hovered();
    bool const selected = cgo.isSelected();

    // drawn as a fat background
    if (hovered || selected) {

        double const lineWidth = _connectionStyle.lineWidth();

        QPen pen;
        pen.setWidth(static_cast<int>(2 * lineWidth));
        pen.setColor(selected ? _connectionStyle.selectedHaloColor()
            : _connectionStyle.hoveredColor());

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        auto const cubic = cubicPath(cgo);
        painter->drawPath(cubic);
    }
}

void AgxConnectionPainter::drawNormalLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const
{
    AgxConnectionState const& state = cgo.connectionState();

    if (state.requiresPort())
        return;

    // colors

    QColor normalColorOut = _connectionStyle.normalColor();
    QColor normalColorIn = _connectionStyle.normalColor();
    QColor selectedColor = _connectionStyle.selectedColor();

    //bool useGradientColor = false;

    AgxGraphModel const& graphModel = cgo.agxNodeScene()->agxGraphModel();

    /*if (connectionStyle.useDataDefinedColors()) {
        using AgxPortType;

        auto const& cId = cgo.connectionId();

        auto dataTypeOut = graphModel
            .portData(cId.outNodeId,
                PortType::Out,
                cId.outPortIndex,
                AgxPortRole::DataType)
            .value<AgxNodeDataType>();

        auto dataTypeIn
            = graphModel.portData(cId.inNodeId, PortType::In, cId.inPortIndex, AgxPortRole::DataType)
            .value<AgxNodeDataType>();

        useGradientColor = (dataTypeOut.id != dataTypeIn.id);

        normalColorOut = connectionStyle.normalColor(dataTypeOut.id);
        normalColorIn = connectionStyle.normalColor(dataTypeIn.id);
        selectedColor = normalColorOut.darker(200);
    }*/

    // geometry

    double const lineWidth = _connectionStyle.lineWidth();

    // draw normal line
    QPen p;

    auto const& cId = cgo.connectionId();

    p.setWidth(lineWidth);

    bool const selected = cgo.isSelected();

    auto cubic = cubicPath(cgo);
    /*if (useGradientColor) {
        painter->setBrush(Qt::NoBrush);

        QColor cOut = normalColorOut;
        if (selected)
            cOut = cOut.darker(200);
        p.setColor(cOut);
        painter->setPen(p);

        unsigned int constexpr segments = 60;

        for (unsigned int i = 0ul; i < segments; ++i) {
            double ratioPrev = double(i) / segments;
            double ratio = double(i + 1) / segments;

            if (i == segments / 2) {
                QColor cIn = normalColorIn;
                if (selected)
                    cIn = cIn.darker(200);

                p.setColor(cIn);
                painter->setPen(p);
            }
            painter->drawLine(cubic.pointAtPercent(ratioPrev), cubic.pointAtPercent(ratio));
        }

        {
            QIcon icon(":convert.png");

            QPixmap pixmap = icon.pixmap(QSize(22, 22));
            painter->drawPixmap(cubic.pointAtPercent(0.50)
                - QPoint(pixmap.width() / 2, pixmap.height() / 2),
                pixmap);
        }
    }
    else*/ {
        
        auto newColor = normalColorOut;
        if(cId.isHidden)
        {
            newColor = _connectionStyle.constructionColor();
            newColor.setAlpha(50);
            p.setStyle(Qt::DashDotLine);
        }

        p.setColor(newColor);

        if (selected) {
            p.setColor(selectedColor);
        }
        

        painter->setPen(p);
        
        painter->setBrush(Qt::NoBrush);

        painter->drawPath(cubic);
    }
}

void AgxConnectionPainter::paint(QPainter* painter, AgxConnectionGraphicsObject const& cgo)
{
    _connectionStyle = AgxStyleCollection::connectionStyle();
    drawHoveredOrSelected(painter, cgo);

    drawSketchLine(painter, cgo);

    drawNormalLine(painter, cgo);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, cgo);
#endif

    // draw end points

    double const pointDiameter = _connectionStyle.pointDiameter();

    painter->setPen(_connectionStyle.constructionColor());
    painter->setBrush(_connectionStyle.constructionColor());
    double const pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(cgo.out(), pointRadius, pointRadius);
    painter->drawEllipse(cgo.in(), pointRadius, pointRadius);
}

QPainterPath AgxConnectionPainter::getPainterStroke(AgxConnectionGraphicsObject const& connection) const
{
    auto cubic = cubicPath(connection);

    QPointF const& out = connection.endPoint(AgxPortType::Out);
    QPainterPath result(out);

    unsigned int constexpr segments = 20;

    for (auto i = 0ul; i < segments; ++i) {
        double ratio = double(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker;
    stroker.setWidth(10.0);

    return stroker.createStroke(result);
}