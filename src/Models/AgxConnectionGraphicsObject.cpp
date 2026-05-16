//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScope
// ReSharper disable CppTooWideScopeInitStatement
#include "stdafx.h"
#include "AgxConnectionGraphicsObject.h"
#pragma warning(push,0)
#include <QGraphicsItem>


#pragma warning(pop)
#include "Utilities/locateNode.h"
#include "Models/AgxNodeGraphicsObject.h"
#include "Models/AgxNodeConnectionInteraction.h"
#include "Models/AgxGraphicsScene.h"
#include "Models/AgxGraphModel.h"
#include <Utilities/AgxConnectionIdUtils.h>

AgxConnectionGraphicsObject::AgxConnectionGraphicsObject(AgxGraphicsScene& scene, const AgxConnectionId& connectionId) :
                                                                                _connectionId(connectionId),
                                                                                _graphModel(scene.agxGraphModel()),
                                                                                _connectionState(*this),
                                                                                _out{ 0, 0 }, _in{ 0, 0 }
{
    scene.addItem(this);

    setFlag(ItemIsMovable, true);
    setFlag(ItemIsFocusable, true);
    setFlag(ItemIsSelectable, true);

    setAcceptHoverEvents(true);

    //addGraphicsEffect();

    setZValue(-1.0);

    initializePosition();
}

void AgxConnectionGraphicsObject::initializePosition()
{
    // This function is only called when the ConnectionGraphicsObject
    // is newly created. At this moment both end coordinates are (0, 0)
    // in Connection G.O. coordinates. The position of the whole
    // Connection G. O. in scene coordinate system is also (0, 0).
    // By moving the whole object to the Node Port position
    // we position both connection ends correctly.

    if (_connectionState.requiredPort() != AgxPortType::None) {
        const AgxPortType attachedPort = oppositePort(_connectionState.requiredPort());

        const AgxPortIndex portIndex = getPortIndex(attachedPort, _connectionId);
        const AgxNodeId nodeId = getNodeId(attachedPort, _connectionId);

        const AgxNodeGraphicsObject* ngo = agxNodeScene()->agxNodeGraphicsObject(nodeId);

        if (ngo) {
            const QTransform nodeSceneTransform = ngo->sceneTransform();

            const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            const QPointF pos = geometry.portScenePosition(nodeId,
                                                           attachedPort,
                                                           portIndex,
                                                           nodeSceneTransform);

            this->setPos(pos);
        }
    }

    move();
}

void AgxConnectionGraphicsObject::addGraphicsEffect()
{
    const auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    //auto effect = new QGraphicsDropShadowEffect;
    //auto effect = new ConnectionBlurEffect(this);
    //effect->setOffset(4, 4);
    //effect->setColor(QColor(Qt::gray).darker(800));
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2Horizontal() const
{
    constexpr double defaultOffset = 300;
    constexpr double minOffset = 300;

    const double xDistance = qAbs(_in.x() - _out.x());
    const bool addNegativeMod = _in.x() <= _out.x();

    double horizontalOffset;

    if (addNegativeMod)
        horizontalOffset = qMin(defaultOffset, qAbs(xDistance));
    else
        horizontalOffset = qMin(defaultOffset, 1200.0);

    horizontalOffset = qMax(horizontalOffset, minOffset);
    const QPointF deltaVector = _in - _out;

    double ratioY = qAbs(deltaVector.x() * deltaVector.x() + deltaVector.y() * deltaVector.y() - 1);
    ratioY = qMin(0.0, ratioY);
    ratioY = qMax(1.0, ratioY);

    double ratioX = xDistance / 200;
    ratioX = ratioX < 0.1 ? 0.1 : ratioX;
    ratioX = ratioX > 1.0 ? 1.0 : ratioX;
    
    horizontalOffset *= ratioX;
    const double verticalOffset = ratioY * 25;

    QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2Vertical() const
{
    constexpr double defaultOffset = 200;

    const double yDistance = _in.y() - _out.y();

    double verticalOffset = qMin(defaultOffset, std::abs(yDistance));

    double horizontalOffset = 0;

    double ratioY = 0.5;

    if (yDistance <= 0) {
        const double xDistance = _in.x() - _out.x() + 20;

        const double vector = xDistance < 0 ? -1.0 : 1.0;

        horizontalOffset = qMin(defaultOffset, std::abs(xDistance)) * vector;

        ratioY = 1.0;
    }

    verticalOffset *= ratioY;

    QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}

QPointF AgxConnectionGraphicsObject::GetOutPosition() const
{
    const bool collapsed = _graphModel.nodeData<bool>(_connectionId.outNodeId, AgxNodeRole::CollapseState);
    const auto nGeom = agxNodeScene()->agxNodeGeometry();
    return collapsed ? nGeom.collapsedPortPosition(_connectionId.outNodeId, AgxPortType::Out) : _out;
}

QPointF AgxConnectionGraphicsObject::GetInPosition() const
{
    const bool collapsed = _graphModel.nodeData<bool>(_connectionId.inNodeId, AgxNodeRole::CollapseState);
    const auto nGeom = agxNodeScene()->agxNodeGeometry();
    return collapsed ? nGeom.collapsedPortPosition(_connectionId.inNodeId, AgxPortType::In) : _in;
}

AgxGraphModel& AgxConnectionGraphicsObject::graphModel() const
{
    return _graphModel;
}

AgxGraphicsScene* AgxConnectionGraphicsObject::agxNodeScene() const
{
    return dynamic_cast<AgxGraphicsScene*>(scene());
}

AgxConnectionId const& AgxConnectionGraphicsObject::connectionId() const
{
    return _connectionId;
}

QRectF AgxConnectionGraphicsObject::boundingRect() const
{
    auto [c1Point, c2Point] = pointsC1C2();

    // `normalized()` fixes inverted rects.
    const QRectF basicRect = QRectF(_out, _in).normalized();

    const QRectF c1c2Rect = QRectF(c1Point, c2Point).normalized();

    QRectF commonRect = basicRect.united(c1c2Rect);

    auto const& connectionStyle = AgxPalette::GetInstance().connectionPalette();
    float const diam = connectionStyle.PointDiameter;
    QPointF const cornerOffset(diam, diam);

    // Expand rect by port circle diameter
    commonRect.setTopLeft(commonRect.topLeft() - cornerOffset);
    commonRect.setBottomRight(commonRect.bottomRight() + 2 * cornerOffset);

    return commonRect;
}

QPainterPath AgxConnectionGraphicsObject::shape() const
{
#ifdef DEBUG_DRAWING
    ////leaving this just in case...
    //QPainterPath path;
    //path.addRect(boundingRect());
    //return path;

#else
    return AgxConnectionPainter::GetPainterStroke(*this);
#endif
}

QPointF const& AgxConnectionGraphicsObject::endPoint(const AgxPortType portType) const
{
    Q_ASSERT(portType != AgxPortType::None);

    return portType == AgxPortType::Out ? _out : _in;
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2() const
{
    return pointsC1C2Horizontal();
}

void AgxConnectionGraphicsObject::setEndPoint(const AgxPortType portType, QPointF const& point) const
{
    if (portType == AgxPortType::In)
        _in = point;
    else
        _out = point;
}

void AgxConnectionGraphicsObject::move()
{
    auto moveEnd = [this](const AgxConnectionId& cId, const AgxPortType portType) {
        const AgxNodeId nodeId = getNodeId(portType, cId);

        if (nodeId == InvalidNodeId)
            return;

        const AgxNodeGraphicsObject* ngo = agxNodeScene()->agxNodeGraphicsObject(nodeId);

        if (ngo)
        {
            const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            const QPointF scenePos = geometry.portScenePosition(nodeId,
                portType,
                getPortIndex(portType, cId),
                ngo->sceneTransform());

            const QPointF connectionPos = sceneTransform().inverted().map(scenePos);

            setEndPoint(portType, connectionPos);
        }
        };

    moveEnd(_connectionId, AgxPortType::Out);
    moveEnd(_connectionId, AgxPortType::In);

    prepareGeometryChange();

    update();
}

AgxConnectionState const& AgxConnectionGraphicsObject::connectionState() const
{
    return _connectionState;
}

AgxConnectionState& AgxConnectionGraphicsObject::connectionState()
{
    return _connectionState;
}

void AgxConnectionGraphicsObject::setConnectionHidden(const bool hidden)
{
    _connectionId.isHidden = hidden;
}

void AgxConnectionGraphicsObject::paint(QPainter* painter,
    QStyleOptionGraphicsItem const* option,
    QWidget*)
{
    if (!scene())
        return;

    painter->setClipRect(option->exposedRect);

    agxNodeScene()->agxConnectionPainter().paint(painter, *this);
}

void AgxConnectionGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
    {
        event->ignore();
        return;
    }

    /*if ((event->modifiers() & Qt::ShiftModifier) != 0) {
        agxNodeScene()->onSelectNodes({ _connectionId.inNodeId, _connectionId.outNodeId });
    }*/

    QGraphicsItem::mousePressEvent(event);
}

void AgxConnectionGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0)
    {
        event->ignore();
        return;
    }

    if (event->button() == Qt::LeftButton) {
        setConnectionHidden(!_connectionId.isHidden);
        update();
    }

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void AgxConnectionGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    prepareGeometryChange();

    //We use static cast due to RTTI rules on qt6 often returning a null pointer even when dynamic cast should succeed
    const auto view = static_cast<QGraphicsView*>(event->widget()); // NOLINT(*-pro-type-static-cast-downcast)
    const auto ngo = locateAgxNodeAt(event->scenePos(), *agxNodeScene(), view->transform());
    if (ngo) {
        ngo->reactToConnection(this);

        _connectionState.setLastHoveredNode(ngo->nodeId());
    }
    else {
        _connectionState.resetLastHoveredNode();
    }

    //-------------------

    const auto requiredPort = _connectionState.requiredPort();

    if (requiredPort != AgxPortType::None)
    {
        setEndPoint(requiredPort, event->pos());
    }

    //-------------------

    update();

    event->accept();
}

void AgxConnectionGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);

    ungrabMouse();
    event->accept();

    //We use static cast due to RTTI rules on qt6 often returning a null pointer even when dynamic cast should succeed
    const auto view = static_cast<QGraphicsView*>(event->widget()); // NOLINT(*-pro-type-static-cast-downcast)

    Q_ASSERT(view);

    const auto ngo = locateAgxNodeAt(event->scenePos(), *agxNodeScene(), view->transform());
    bool wasConnected = false;

    if (ngo) {
        const AgxNodeConnectionInteraction interaction(*ngo, *this, *agxNodeScene());

        wasConnected = interaction.tryConnect();
    }

    // If connection attempt was unsuccessful
    if (!wasConnected) {
        // Resulting unique_ptr is not used and automatically deleted.
        agxNodeScene()->resetDraftConnection();
    }
}

void AgxConnectionGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    _connectionState.setHovered(true);

    update();

    // Signal
    agxNodeScene()->connectionHovered(connectionId(), event->screenPos());

    event->accept();
}

void AgxConnectionGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _connectionState.setHovered(false);

    update();

    // Signal
    agxNodeScene()->connectionHoverLeft(connectionId());

    event->accept();
}