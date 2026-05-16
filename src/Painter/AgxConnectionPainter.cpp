//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxConnectionPainter.h"

#include "Models/AgxGraphModel.h"
#include "Models/AgxGraphicsScene.h"
#include "Models/AgxConnectionGraphicsObject.h"
#include "Models/AgxConnectionState.h"


QPainterPath AgxConnectionPainter::CubicPath(AgxConnectionGraphicsObject const& connection)
{
    QPointF const& in = connection.endPoint(AgxPortType::In);
    QPointF const& out = connection.endPoint(AgxPortType::Out);

    const auto [c1, c2] = connection.pointsC1C2();

    // cubic spline
    QPainterPath cubic(out);

    cubic.cubicTo(c1, c2, in);

    return cubic;
}

void AgxConnectionPainter::drawSketchLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const
{
    if (const AgxConnectionState& state = cgo.connectionState(); state.requiresPort()) {

        QPen pen;
        pen.setWidth(static_cast<int>(m_connPalette->ConstructionLineWidth));
        pen.setColor(m_connPalette->ConstructionColor);
        pen.setStyle(Qt::DashLine);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        const auto cubic = CubicPath(cgo);

        // cubic spline
        painter->drawPath(cubic);
    }
}

void AgxConnectionPainter::drawHoveredOrSelected(QPainter* painter, const AgxConnectionGraphicsObject& cgo) const
{
    bool const hovered = cgo.connectionState().hovered();

    // drawn as a fat background
    if (const bool selected = cgo.isSelected(); hovered || selected) {

        double const lineWidth = m_connPalette->LineWidth;

        QPen pen;
        pen.setWidth(static_cast<int>(2 * lineWidth));
        pen.setColor(selected ? m_connPalette->SelectedHaloColor
            : m_connPalette->HoveredColor);

        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);

        // cubic spline
        auto const cubic = CubicPath(cgo);
        painter->drawPath(cubic);
    }
}

void AgxConnectionPainter::drawNormalLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const
{
    // ReSharper disable once CppTooWideScopeInitStatement
    AgxConnectionState const& state = cgo.connectionState();

    if (state.requiresPort())
        return;

    // colors
    QColor normalColorOut = m_connPalette->NormalColor;
    QColor selectedColor = m_connPalette->SelectedColor;

    double const lineWidth = m_connPalette->LineWidth;

    // draw normal line
    QPen p;

    auto const& cId = cgo.connectionId();

    p.setWidth(lineWidth);

    bool const selected = cgo.isSelected();

    // ReSharper disable once CppTooWideScope
    auto cubic = CubicPath(cgo);

    auto newColor = normalColorOut;
    if(cId.isHidden)
    {
        newColor = m_connPalette->ConstructionColor;
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

void AgxConnectionPainter::paint(QPainter* painter, AgxConnectionGraphicsObject const& cgo)
{
    m_connPalette = &AgxPalette::GetInstance().connectionPalette();

    if (!m_connPalette)
        qFatal() << "AgxConnectionPainter::paint(QPainter* painter, AgxConnectionGraphicsObject const& cgo): "
                    "connection palette could not be referenced";

    drawHoveredOrSelected(painter, cgo);

    drawSketchLine(painter, cgo);

    drawNormalLine(painter, cgo);

#ifdef NODE_DEBUG_DRAWING
    debugDrawing(painter, cgo);
#endif

    // draw end points

    double const pointDiameter = m_connPalette->PointDiameter;

    painter->setPen(m_connPalette->ConstructionColor);
    painter->setBrush(m_connPalette->ConstructionColor);
    double const pointRadius = pointDiameter / 2.0;
    painter->drawEllipse(cgo.out(), pointRadius, pointRadius);
    painter->drawEllipse(cgo.in(), pointRadius, pointRadius);
}

QPainterPath AgxConnectionPainter::GetPainterStroke(const AgxConnectionGraphicsObject& connection)
{
    const auto cubic = CubicPath(connection);

    QPointF const& out = connection.endPoint(AgxPortType::Out);
    QPainterPath result(out);

    unsigned int constexpr segments = 20;

    for (auto i = 0ul; i < segments; ++i) {
        const double ratio = static_cast<double>(i + 1) / segments;
        result.lineTo(cubic.pointAtPercent(ratio));
    }

    QPainterPathStroker stroker;
    stroker.setWidth(10.0);

    return stroker.createStroke(result);
}