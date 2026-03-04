#include "stdafx.h"
#include "AgxNodeGraphicsObject.h"
#include "Utilities/UndoRedoCommands.h"
#pragma warning(push,0)
#include <QGraphicsEffect>
#include <QtWidgets>
#pragma warning(pop)

#include "Utilities/AgxConnectionIdUtils.h"
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Painter/AgxNodePainter.h"
#include "Models/AgxNodeConnectionInteraction.h"

#include <Widgets/MiniGraphicsView.h>

AgxNodeGraphicsObject::AgxNodeGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId) : _nodeId(nodeId), _graphModel(scene.agxGraphModel()), _nodeState(*this), _proxyWidget(nullptr)
{
    scene.addItem(this);

    setFlag(QGraphicsItem::ItemDoesntPropagateOpacityToChildren, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setLockedState();

    //setCacheMode(QGraphicsItem::NoCache);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    

    QJsonObject nodeStyleJson = _graphModel.nodeData(_nodeId, AgxNodeRole::Style).toJsonObject();

    AgxNodeStyle nodeStyle(nodeStyleJson);

    /*if (nodeStyle.ShadowEnabled) {
        auto effect = new QGraphicsDropShadowEffect;
        effect->setOffset(4, 4);
        effect->setBlurRadius(20);
        effect->setColor(nodeStyle.ShadowColor);

        setGraphicsEffect(effect);
    }*/

    setOpacity(nodeStyle.Opacity);

    setAcceptHoverEvents(true);

    setZValue(0);

    embedQWidget();

    agxNodeScene()->agxNodeGeometry().recomputeSize(_nodeId);

    QPointF const pos = _graphModel.nodeData(_nodeId, AgxNodeRole::Position).toPointF();

    setPos(pos);

    connect(&_graphModel, &AgxGraphModel::nodeFlagsUpdated, [this](AgxNodeId const nodeId) {
        if (_nodeId == nodeId)
            setLockedState();
        });
}

AgxGraphicsScene* AgxNodeGraphicsObject::agxNodeScene() const
{
    return dynamic_cast<AgxGraphicsScene*>(scene());
}

AgxGraphModel& AgxNodeGraphicsObject::graphModel() const
{
    return _graphModel;
}

QRectF AgxNodeGraphicsObject::boundingRect() const
{
    AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
    bool collapsed = agxNodeScene()->agxGraphModel().nodeData<bool>(_nodeId, AgxNodeRole::CollapseState);

    if (collapsed)
        return geometry.collapsedBoundingRect(_nodeId);

    return geometry.boundingRect(_nodeId);
}

void AgxNodeGraphicsObject::reactToConnection(AgxConnectionGraphicsObject const* cgo)
{
    _nodeState.storeConnectionForReaction(cgo);

    update();
}

void AgxNodeGraphicsObject::updateQWidgetEmbedPos()
{
    if (_proxyWidget) {
        AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
        auto prev = _proxyWidget->pos();
        _proxyWidget->setPos(geometry.widgetPosition(_nodeId));
        if (prev != _proxyWidget->pos())
        {
            AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
            geometry.recomputeSize(_nodeId);
        }
    }
}

void AgxNodeGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}

void AgxNodeGraphicsObject::moveConnections() const
{
    auto const& connected = _graphModel.allConnectionIds(_nodeId);

    for (auto& cnId : connected) {
        auto cgo = agxNodeScene()->agxConnectionGraphicsObject(cnId);

        if (cgo)
            cgo->move();
    }
}

void AgxNodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton || (event->modifiers() & Qt::AltModifier) != 0 )
    {
        event->ignore();
        return;
    }

    if (event->button() == Qt::RightButton) 
    {
        if ((event->modifiers() & Qt::ControlModifier) == 0 && !isSelected()) {
            agxNodeScene()->clearSelection(); }
        setSelected(true);
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }

    if (agxNodeScene()->agxGraphModel().nodeData<bool>(_nodeId, AgxNodeRole::CollapseState) || agxNodeScene()->agxGraphModel().nodeFlags(_nodeId) & AgxNodeFlag::Locked) {
        return;
    }

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
    {
        if ((event->modifiers() & Qt::ControlModifier) == 0)
            agxNodeScene()->clearSelection();
        Q_EMIT agxNodeScene()->nodePreClicked(_nodeId, (event->modifiers() & Qt::ControlModifier) != 0);
    }
        
    AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
    
    if(event->button() != Qt::RightButton){
        for (AgxPortType portToCheck : {AgxPortType::In, AgxPortType::Out}) {
            QPointF nodeCoord = sceneTransform().inverted().map(event->scenePos());

            AgxPortIndex const portIndex = geometry.checkPortHit(_nodeId, portToCheck, nodeCoord);

            if (portIndex == InvalidPortIndex)
                continue;

            auto const& connected = _graphModel.connections(_nodeId, portToCheck, portIndex);

            // Start dragging existing connection.
            if (!connected.empty() && portToCheck == AgxPortType::In) {
                auto const& cnId = *connected.begin();

                // Need ConnectionGraphicsObject

                AgxNodeConnectionInteraction interaction(*this,
                    *agxNodeScene()->agxConnectionGraphicsObject(cnId),
                    *agxNodeScene());

                if (_graphModel.detachPossible(cnId))
                    interaction.disconnect(portToCheck);
            }
            else // initialize new Connection
            {
                if (portToCheck == AgxPortType::Out) {
                    auto const outPolicy = _graphModel
                        .portData(_nodeId,
                            portToCheck,
                            portIndex,
                            AgxPortRole::ConnectionPolicyRole)
                        .value<AgxConnectionPolicy>();

                    if (!connected.empty() && outPolicy == AgxConnectionPolicy::One) {
                        for (auto& cnId : connected) {
                            _graphModel.deleteConnection(cnId);
                        }
                    }
                } // if port == out

                AgxConnectionId const incompleteConnectionId = makeIncompleteConnectionId(_nodeId,
                    portToCheck,
                    portIndex);

                // From the moment of creation a draft connection
                // grabs the mouse events and waits for the mouse button release
                agxNodeScene()->makeDraftConnection(incompleteConnectionId);
            }
        }

        if (_graphModel.nodeFlags(_nodeId) & AgxNodeFlag::Resizable) {
            auto pos = event->pos();
            bool const hit = geometry.resizeHandleRect(_nodeId).contains(QPoint(pos.x(), pos.y()));
            _nodeState.setResizing(hit);
        }
    }

    if (isSelected()) {
        Q_EMIT agxNodeScene()->nodeSelected(_nodeId);
    }
    
    QGraphicsObject::mousePressEvent(event);
}

void AgxNodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton) setFlag(QGraphicsItem::ItemIsMovable, true);
    if ((event->modifiers() & Qt::ShiftModifier) == 0)
        QGraphicsObject::mouseReleaseEvent(event);
}

void AgxNodeGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->lastHoveredNode = this;
    
    // bring all the colliding nodes to background
    QList<QGraphicsItem*> overlapItems = collidingItems();

    for (QGraphicsItem* item : overlapItems) {
        if (item->zValue() > 0.0 && item->zValue() < 2.0) {
            item->setZValue(0.0);
        }
    }

    // bring this node forward
    setZValue(1.0);

    _nodeState.setHovered(true);

    update();

    Q_EMIT agxNodeScene()->nodeHovered(_nodeId, event->screenPos());

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
        agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());

    event->accept();
}

void AgxNodeGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->lastHoveredNode = nullptr;
    //QGraphicsObject::hoverLeaveEvent(event);
    
    agxNodeScene()->setGroupHoverState(false,"");

    _nodeState.setHovered(false);

    setZValue(0.0);

    update();

    Q_EMIT agxNodeScene()->nodeHoverLeft(_nodeId);

    event->accept();    
}

void AgxNodeGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    auto pos = event->pos();

    //NodeGeometry geometry(_nodeId, _graphModel, nodeScene());
    AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

    if ((_graphModel.nodeFlags(_nodeId) | AgxNodeFlag::Resizable)
        && geometry.resizeHandleRect(_nodeId).contains(QPoint(pos.x(), pos.y()))) {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else {
        setCursor(QCursor());
    }

    event->accept();
}

void AgxNodeGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    prepareGeometryChange();
    Q_EMIT agxNodeScene()->nodeDoubleClicked(_nodeId);
    QTimer::singleShot(1, this, [this]() { update(); });
}

void AgxNodeGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    Q_EMIT agxNodeScene()->nodeContextMenu(_nodeId, mapToScene(event->pos()));
}

void AgxNodeGraphicsObject::embedQWidget()
{
    AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
    geometry.recomputeSize(_nodeId);

    if (auto w = _graphModel.nodeData(_nodeId, AgxNodeRole::Widget).value<QWidget*>()) {
        _proxyWidget = new QGraphicsProxyWidget(this);

        _proxyWidget->setWidget(w);

        _proxyWidget->setPreferredWidth(5);

        geometry.recomputeSize(_nodeId);

        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag) 
        {
            unsigned int widgetHeight = geometry.size(_nodeId).height()
                - geometry.captionRect(_nodeId).height();

            // If the widget wants to use as much vertical space as possible, set
            // it to have the geom's equivalentWidgetHeight.
            _proxyWidget->setMinimumHeight(widgetHeight);
        }

        updateQWidgetEmbedPos();

        //update();

        _proxyWidget->setOpacity(1.0);
        _proxyWidget->setFlag(QGraphicsItem::ItemIgnoresParentOpacity);
        _proxyWidget->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

        if (auto wAgx = dynamic_cast<IAgxEmbedSceneData*>(w)) {
            wAgx->SetRefData(_nodeId, agxNodeScene());
        }
    }
}

void AgxNodeGraphicsObject::setLockedState()
{
    AgxNodeFlags flags = _graphModel.nodeFlags(_nodeId);

    bool const locked = flags.testFlag(AgxNodeFlag::Locked);

    setFlag(QGraphicsItem::ItemIsMovable, !locked);
    setFlag(QGraphicsItem::ItemIsSelectable, !locked);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, !locked);
}

void AgxNodeGraphicsObject::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    // Deselect all other items after this one is selected.
    // Unless we press a CTRL button to add the item to the selected group before
    // starting moving.
    if (!isSelected()) {
        if (!event->modifiers().testFlag(Qt::ControlModifier))
            scene()->clearSelection();

        setSelected(true);
    }

    if (_nodeState.resizing()) {
        auto diff = event->pos() - event->lastPos();

        if (auto w = _graphModel.nodeData(_nodeId, AgxNodeRole::Widget).value<QWidget*>()) {
            prepareGeometryChange();

            auto oldSize = w->size();

            oldSize += QSize(diff.x(), diff.y());

            w->resize(oldSize);

            AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            // Passes the new size to the model.
            geometry.recomputeSize(_nodeId);

            update();

            moveConnections();

            event->accept();
        }
    }
    else {
        auto diff = event->pos() - event->lastPos();

        agxNodeScene()->undoStack().push(new MoveNodeCommand(agxNodeScene(), diff));

        event->accept();
    }

    QRectF r = agxNodeScene()->sceneRect();

    r = r.united(mapToScene(boundingRect()).boundingRect());

    agxNodeScene()->setSceneRect(r);
    
}

void AgxNodeGraphicsObject::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget)
{
    /*QString tooltip;

    QVariant var = _graphModel.nodeData(_nodeId, AgxNodeRole::ValidationState);
    if (var.canConvert<AgxNodeValidationState>()) {
        auto state = var.value<AgxNodeValidationState>();
        if (state._state != AgxNodeValidationState::State::Valid) {
            tooltip = state._stateMessage;
        }
    }
    setToolTip(tooltip);*/

    updateQWidgetEmbedPos();

    painter->setClipRect(option->exposedRect);

    agxNodeScene()->agxNodePainter().paint(painter, *this);
}

QVariant AgxNodeGraphicsObject::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemScenePositionHasChanged && scene()) {
        moveConnections();
    }

    return QGraphicsObject::itemChange(change, value);
}

AgxCommentGraphicsObject::AgxCommentGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId) : AgxNodeGraphicsObject(scene, nodeId) 
{
    setZValue(4);
}

QVariant AgxCommentGraphicsObject::itemChange(GraphicsItemChange change, const QVariant& value)
{
    //if (change == ItemScenePositionHasChanged) //use causes hover/deselect/etc issues with the border margin and updating the arrow gobj... will fix later.
    {
        _iData = _graphModel.nodeData(_nodeId, AgxNodeRole::InternalData).toJsonObject()["internal-data"].toObject();
        if (_iData.contains("comment-target"))
        {
            if (!_arrowGraphicsObject)
            {
                _arrowGraphicsObject = new AgxArrowGraphicsObject();
                //_arrowGraphicsObject->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
                if (scene()) scene()->addItem(_arrowGraphicsObject);

                connect(this, &QGraphicsObject::destroyed, _arrowGraphicsObject, &QGraphicsObject::deleteLater);
                connect(_arrowGraphicsObject, &AgxArrowGraphicsObject::TargetUpdated, [this](const QPointF& target) {
                    QJsonObject data;
                    QJsonObject targetData;
                    targetData["x"] = target.x();
                    targetData["y"] = target.y();
                    data["comment-target"] = targetData;
                    
                    agxNodeScene()->undoStack().push(new InsertPropertySheetDataCommand(agxNodeScene(), _nodeId, data));
                    _iData = _graphModel.nodeData(_nodeId, AgxNodeRole::InternalData).toJsonObject()["internal-data"].toObject();
                    QTimer::singleShot(1, this, &AgxCommentGraphicsObject::RecalculateTarget);
                        });
            }

            RecalculateTarget();
        }
    }

    return AgxNodeGraphicsObject::itemChange(change, value);
}

void AgxCommentGraphicsObject::RecalculateTarget() const
{
    auto target = _iData["comment-target"].toObject();
    QPointF tgtPt(target["x"].toDouble(), target["y"].toDouble());

    QPointF origin = mapToScene(boundingRect().center());

    QPointF cornerPt(0.0, 0.0);
    QLineF boundLine(boundingRect().center(), cornerPt);
    auto distance = boundLine.length();
    QLineF fullLine(origin, tgtPt);
    auto t = distance / fullLine.length();
    QPointF lerpOrigin = origin + t * (tgtPt - origin);
    lerpOrigin = t < 1 ? lerpOrigin : origin;
    _arrowGraphicsObject->OnParentUpdated(lerpOrigin, tgtPt, t < 1);
}

//void AgxCommentGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//    AgxNodeGraphicsObject::paint(painter, option, widget);
//
//    //agxNodeScene()->agxNodePainter().drawWrappedTextInWidgetBounds(painter, *this, _iData["property-sheet"].toObject()["comment"].toObject()["text"].toString());
//}

void AgxCommentGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->lastHoveredNode = this;

    // bring this node forward
    setZValue(5);

    _nodeState.setHovered(true);

    update();

    Q_EMIT agxNodeScene()->nodeHovered(_nodeId, event->screenPos());

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
        agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());

    event->accept();
}

void AgxCommentGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    agxNodeScene()->lastHoveredNode = nullptr;
    //QGraphicsObject::hoverLeaveEvent(event);

    agxNodeScene()->setGroupHoverState(false, "");

    _nodeState.setHovered(false);

    setZValue(4);

    update();

    Q_EMIT agxNodeScene()->nodeHoverLeft(_nodeId);

    event->accept();
}

AgxArrowGraphicsObject::AgxArrowGraphicsObject(QGraphicsObject* parent) : QGraphicsObject(parent)
{
    _connectionStyle = AgxConnectionStyle();
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setZValue(3); 
    
}

QRectF AgxArrowGraphicsObject::boundingRect() const
{
    QPointF topLeft(
        qMin(_origin.x(), _target.x()) - 50.0,
        qMin(_origin.y(), _target.y()) - 50.0
    );
    QPointF botRight(
        qMax(_origin.x(), _target.x()) + 50.0,
        qMax(_origin.y(), _target.y()) + 50.0
    );
    return QRectF(topLeft, botRight);
}

void AgxArrowGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    qreal arrowSize = 15; // size of head

    QPen pen;

    pen.setWidth(static_cast<int>(_connectionStyle.constructionLineWidth()));
    
    if (isSelected())
        pen.setColor(_connectionStyle.selectedHaloColor());
    else if (_hovered)
        pen.setColor(_connectionStyle.hoveredColor());
    else
        pen.setColor(_connectionStyle.constructionColor());
    
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QLineF line(_target, _origin);

    double angle = std::atan2(-line.dy(), line.dx());
    QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << arrowP2 << line.p1() << arrowP1;
    _arrowRect = QRectF(arrowHead.boundingRect().topLeft() - QPointF(10,10), arrowHead.boundingRect().bottomRight() + QPointF(10,10));

    if (_renderLine) {
        painter->drawLine(line);
        setZValue(3);
    } else setZValue(2.1);

    painter->drawPolyline(arrowHead);
}

void AgxArrowGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!_hovered && !isSelected())
        event->ignore();
    else
        QGraphicsObject::mousePressEvent(event);
}

void AgxArrowGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    _targetPersistent = _target;

    Q_EMIT TargetUpdated(_targetPersistent);
}

void AgxArrowGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    auto temp = _hovered;
    if (_arrowRect.contains(event->pos()))
    {
        _hovered = true;
    }
    else
    {
        _hovered = false;
    }

    if (temp != _hovered) update();

    QGraphicsObject::hoverEnterEvent(event);
}

void AgxArrowGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    _hovered = false;

    update();
    
    QGraphicsObject::hoverLeaveEvent(event);
}

void AgxArrowGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    auto temp = _hovered;
    if (_arrowRect.contains(event->pos()))
    {
        _hovered = true;
    } else
    {
        _hovered = false;
    }

    if (temp != _hovered) update();

    QGraphicsObject::hoverMoveEvent(event);
}

QVariant AgxArrowGraphicsObject::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange)
    {
        QPointF newPos = value.toPointF();
        QPointF oldPos = pos();
        //qDebug() << "Pos " << newPos - oldPos;
        prepareGeometryChange();
        _target = (newPos - oldPos) + _targetPersistent;
        update();
        return QVariant();
    }

    return QGraphicsObject::itemChange(change, value);
}

void AgxArrowGraphicsObject::OnParentUpdated(const QPointF & origin, const QPointF & target, bool renderState)
{
    prepareGeometryChange();
    _renderLine = renderState;
    _origin = origin;
    _target = target;
    _targetPersistent = target;
    update();
}
