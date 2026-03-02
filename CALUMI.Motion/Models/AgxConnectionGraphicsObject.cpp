#include "stdafx.h"
#include "AgxConnectionGraphicsObject.h"
#pragma warning(push,0)
#include <QGraphicsItem>
//#include "QtNodes/internal/ConnectionIdUtils.hpp

#pragma warning(pop)
#include "Utilities/locateNode.h"
#include "Models/AgxNodeGraphicsObject.h"
#include "Models/AgxNodeConnectionInteraction.h"
#include "Models/AgxGraphicsScene.h"
#include "Models/AgxGraphModel.h"
#include <Utilities/AgxConnectionIdUtils.h>
#include <Painter/AgxStyleCollection.h>

AgxConnectionGraphicsObject::AgxConnectionGraphicsObject(AgxGraphicsScene& scene, AgxConnectionId const connectionId) : _connectionId(connectionId), _graphModel(scene.agxGraphModel()), _connectionState(*this), _out{ 0, 0 }, _in{ 0, 0 }
{
    scene.addItem(this);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

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
        AgxPortType attachedPort = oppositePort(_connectionState.requiredPort());

        AgxPortIndex portIndex = getPortIndex(attachedPort, _connectionId);
        AgxNodeId nodeId = getNodeId(attachedPort, _connectionId);

        AgxNodeGraphicsObject* ngo = agxNodeScene()->agxNodeGraphicsObject(nodeId);

        if (ngo) {
            QTransform nodeSceneTransform = ngo->sceneTransform();

            AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            QPointF pos = geometry.portScenePosition(nodeId,
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
    auto effect = new QGraphicsBlurEffect;

    effect->setBlurRadius(5);
    setGraphicsEffect(effect);

    //auto effect = new QGraphicsDropShadowEffect;
    //auto effect = new ConnectionBlurEffect(this);
    //effect->setOffset(4, 4);
    //effect->setColor(QColor(Qt::gray).darker(800));
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2Horizontal() const
{
    double defaultOffset = 300;
    double const minOffset = 300;

    double xDistance = qAbs(_in.x() - _out.x());
    bool addNegativeMod = _in.x() <= _out.x();

    double horizontalOffset = xDistance;

    if (addNegativeMod)
        horizontalOffset = qMin(defaultOffset, qAbs(xDistance));
    else
        horizontalOffset = qMin(defaultOffset, 1200.0);

    horizontalOffset = qMax(horizontalOffset, minOffset);
    QPointF deltaVector = _in - _out;

    double ratioY = qAbs((deltaVector.x() * deltaVector.x() + deltaVector.y() * deltaVector.y()) - 250 / 250);
    ratioY = qMin(0.0, ratioY);
    ratioY = qMax(1.0, ratioY);

    double ratioX = xDistance / 200;
    ratioX = ratioX < 0.1 ? 0.1 : ratioX;
    ratioX = ratioX > 1.0 ? 1.0 : ratioX;

    /*if (xDistance <= 0) {
        double yDistance = _in.y() - _out.y() + 20;

        double vector = yDistance < 0 ? -1.0 : 1.0;

        verticalOffset = qMin(defaultOffset, std::abs(yDistance)) * vector;

        ratioX = 1.0;
    }*/

    
    horizontalOffset *= ratioX;
    double verticalOffset = ratioY * 25;

    QPointF c1(_out.x() + horizontalOffset, _out.y() + verticalOffset);

    QPointF c2(_in.x() - horizontalOffset, _in.y() - verticalOffset);

    return std::make_pair(c1, c2);
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2Vertical() const
{
    double const defaultOffset = 200;

    double yDistance = _in.y() - _out.y();

    double verticalOffset = qMin(defaultOffset, std::abs(yDistance));

    double horizontalOffset = 0;

    double ratioY = 0.5;

    if (yDistance <= 0) {
        double xDistance = _in.x() - _out.x() + 20;

        double vector = xDistance < 0 ? -1.0 : 1.0;

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
    bool collapsed = _graphModel.nodeData<bool>(_connectionId.outNodeId, AgxNodeRole::CollapseState);
    auto ngeom = agxNodeScene()->agxNodeGeometry();
    return collapsed ? ngeom.collapsedPortPosition(_connectionId.outNodeId, AgxPortType::Out) : _out;
}

QPointF AgxConnectionGraphicsObject::GetInPosition() const
{
    bool collapsed = _graphModel.nodeData<bool>(_connectionId.inNodeId, AgxNodeRole::CollapseState);
    auto ngeom = agxNodeScene()->agxNodeGeometry();
    return collapsed ? ngeom.collapsedPortPosition(_connectionId.inNodeId, AgxPortType::In) : _in;
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
    auto points = pointsC1C2();

    // `normalized()` fixes inverted rects.
    QRectF basicRect = QRectF(_out, _in).normalized();

    QRectF c1c2Rect = QRectF(points.first, points.second).normalized();

    QRectF commonRect = basicRect.united(c1c2Rect);

    auto const& connectionStyle = AgxStyleCollection::connectionStyle();
    float const diam = connectionStyle.pointDiameter();
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
    return agxNodeScene()->agxConnectionPainter().getPainterStroke(*this);
#endif
}

QPointF const& AgxConnectionGraphicsObject::endPoint(AgxPortType portType) const
{
    Q_ASSERT(portType != AgxPortType::None);

    return (portType == AgxPortType::Out ? _out : _in);
}

std::pair<QPointF, QPointF> AgxConnectionGraphicsObject::pointsC1C2() const
{
    return pointsC1C2Horizontal();

    /*switch (agxNodeScene()->orientation()) {
    case Qt::Horizontal:
        return pointsC1C2Horizontal();
        break;

    case Qt::Vertical:
        return pointsC1C2Vertical();
        break;
    }*/

    throw std::logic_error("Unreachable code after switch statement");
}

void AgxConnectionGraphicsObject::setEndPoint(AgxPortType portType, QPointF const& point)
{
    if (portType == AgxPortType::In)
        _in = point;
    else
        _out = point;
}

void AgxConnectionGraphicsObject::move()
{
    auto moveEnd = [this](AgxConnectionId cId, AgxPortType portType) {
        AgxNodeId nodeId = getNodeId(portType, cId);

        if (nodeId == InvalidNodeId)
            return;

        AgxNodeGraphicsObject* ngo = agxNodeScene()->agxNodeGraphicsObject(nodeId);

        if (ngo) {
            AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            QPointF scenePos = geometry.portScenePosition(nodeId,
                portType,
                getPortIndex(portType, cId),
                ngo->sceneTransform());

            QPointF connectionPos = sceneTransform().inverted().map(scenePos);

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

void AgxConnectionGraphicsObject::setConnectionHidden(bool hidden)
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

    auto view = static_cast<QGraphicsView*>(event->widget());
    auto ngo = locateAgxNodeAt(event->scenePos(), *agxNodeScene(), view->transform());
    if (ngo) {
        ngo->reactToConnection(this);

        _connectionState.setLastHoveredNode(ngo->nodeId());
    }
    else {
        _connectionState.resetLastHoveredNode();
    }

    //-------------------

    auto requiredPort = _connectionState.requiredPort();

    if (requiredPort != AgxPortType::None) {
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

    auto view = static_cast<QGraphicsView*>(event->widget());

    Q_ASSERT(view);

    auto ngo = locateAgxNodeAt(event->scenePos(), *agxNodeScene(), view->transform());
    bool wasConnected = false;

    if (ngo) {
        AgxNodeConnectionInteraction interaction(*ngo, *this, *agxNodeScene());

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