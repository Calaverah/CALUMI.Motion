//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScope
// ReSharper disable CppTooWideScopeInitStatement
#include "stdafx.h"
#include "AgxNodeGraphicsObject.h"
#include "Utilities/UndoRedoCommands.h"
#pragma warning(push,0)
#include <QtWidgets>
#pragma warning(pop)
#include "Utilities/AgxConnectionIdUtils.h"
#include "Widgets/AgxNodePropertiesWidget.h"
#include "Painter/AgxNodePainter.h"
#include "Models/AgxNodeConnectionInteraction.h"

#include <Widgets/MiniGraphicsView.h>

AgxNodeGraphicsObject::AgxNodeGraphicsObject(AgxGraphicsScene& scene, const AgxNodeId nodeId) : m_nodeId(nodeId), m_graphModel(scene.agxGraphModel()), m_nodeState(*this), m_proxyWidget(nullptr)
{
    scene.addItem(this);

    setFlag(ItemDoesntPropagateOpacityToChildren, true);
    setFlag(ItemIsFocusable, true);

    setLockedState();

    //setCacheMode(QGraphicsItem::NoCache);
    setCacheMode(DeviceCoordinateCache);

    const auto& nodePalette = AgxPalette::GetInstance().nodePalette();

    setOpacity(nodePalette.Opacity);

    setAcceptHoverEvents(true);

    setZValue(0);

    embedQWidget();

    agxNodeScene()->agxNodeGeometry().recomputeSize(m_nodeId);

    QPointF const pos = m_graphModel.nodeData(m_nodeId, AgxNodeRole::Position).toPointF();

    setPos(pos);

    connect(&m_graphModel, &AgxGraphModel::nodeFlagsUpdated, [this](AgxNodeId const aNodeId)
    {
        if (m_nodeId == aNodeId)
            setLockedState();
    });
}

AgxGraphicsScene* AgxNodeGraphicsObject::agxNodeScene() const
{
    return dynamic_cast<AgxGraphicsScene*>(scene());
}

AgxGraphModel& AgxNodeGraphicsObject::graphModel() const
{
    return m_graphModel;
}

QRectF AgxNodeGraphicsObject::boundingRect() const
{
    const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
    const bool collapsed = agxNodeScene()->agxGraphModel().nodeData<bool>(m_nodeId, AgxNodeRole::CollapseState);

    if (collapsed)
        return geometry.collapsedBoundingRect(m_nodeId);

    return geometry.boundingRect(m_nodeId);
}

void AgxNodeGraphicsObject::reactToConnection(AgxConnectionGraphicsObject const* cgo)
{
    m_nodeState.storeConnectionForReaction(cgo);

    update();
}

void AgxNodeGraphicsObject::updateQWidgetEmbedPos() const
{
    if (m_proxyWidget)
    {
        const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
        const auto prev = m_proxyWidget->pos();
        m_proxyWidget->setPos(geometry.widgetPosition(m_nodeId));

        if (prev != m_proxyWidget->pos())
            geometry.recomputeSize(m_nodeId);

    }
}

void AgxNodeGraphicsObject::setGeometryChanged()
{
    prepareGeometryChange();
}

void AgxNodeGraphicsObject::moveConnections() const
{
    const auto& connected = m_graphModel.allConnectionIds(m_nodeId);

    for (auto& cnId : connected) {
        const auto cgo = agxNodeScene()->agxConnectionGraphicsObject(cnId);

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
        setFlag(ItemIsMovable, false);
    }

    if (agxNodeScene()->agxGraphModel().nodeData<bool>(m_nodeId, AgxNodeRole::CollapseState) || agxNodeScene()->agxGraphModel().nodeFlags(m_nodeId) & AgxNodeFlag::Locked) {
        return;
    }

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
    {
        if ((event->modifiers() & Qt::ControlModifier) == 0)
            agxNodeScene()->clearSelection();
        Q_EMIT agxNodeScene()->nodePreClicked(m_nodeId, (event->modifiers() & Qt::ControlModifier) != 0);
    }

    const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

    if(event->button() != Qt::RightButton){
        for (const AgxPortType portToCheck : {AgxPortType::In, AgxPortType::Out}) {
            const QPointF nodeCoord = sceneTransform().inverted().map(event->scenePos());

            AgxPortIndex const portIndex = geometry.checkPortHit(m_nodeId, portToCheck, nodeCoord);

            if (portIndex == InvalidPortIndex)
                continue;

            auto const& connected = m_graphModel.connections(m_nodeId, portToCheck, portIndex);

            // Start dragging existing connection.
            if (!connected.empty() && portToCheck == AgxPortType::In) {
                auto const& cnId = *connected.begin();

                // Need ConnectionGraphicsObject

                AgxNodeConnectionInteraction interaction(*this,
                    *agxNodeScene()->agxConnectionGraphicsObject(cnId),
                    *agxNodeScene());

                if (m_graphModel.detachPossible(cnId))
                {
                    if (!interaction.disconnect(portToCheck))
                        qWarning() << "AgxNodeGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event): "
                                      "Disconnect failed for " << cnId << " on " << m_nodeId;
                }
            }
            else // initialize new Connection
            {
                if (portToCheck == AgxPortType::Out) {
                    auto const outPolicy = m_graphModel
                        .portData(m_nodeId,
                            portToCheck,
                            portIndex,
                            AgxPortRole::ConnectionPolicyRole)
                        .value<AgxConnectionPolicy>();

                    if (!connected.empty() && outPolicy == AgxConnectionPolicy::One) {
                        for (auto& cnId : connected) {
                            m_graphModel.deleteConnection(cnId);
                        }
                    }
                } // if port == out

                AgxConnectionId const incompleteConnectionId = makeIncompleteConnectionId(m_nodeId,
                    portToCheck,
                    portIndex);

                // From the moment of creation a draft connection
                // grabs the mouse events and waits for the mouse button release
                agxNodeScene()->makeDraftConnection(incompleteConnectionId);
            }
        }

        if (m_graphModel.nodeFlags(m_nodeId) & AgxNodeFlag::Resizable) {
            const auto pos = event->pos();
            bool const hit = geometry.resizeHandleRect(m_nodeId).contains(QPoint(
                                                                                static_cast<int>(pos.x()),
                                                                                static_cast<int>(pos.y())));
            m_nodeState.setResizing(hit);
        }
    }

    if (isSelected()) {
        Q_EMIT agxNodeScene()->nodeSelected(m_nodeId);
    }

    QGraphicsObject::mousePressEvent(event);
}

void AgxNodeGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton) setFlag(ItemIsMovable, true);
    if ((event->modifiers() & Qt::ShiftModifier) == 0)
        QGraphicsObject::mouseReleaseEvent(event);
}

void AgxNodeGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->m_lastHoveredNode = this;

    // bring all the colliding nodes to background
    QList<QGraphicsItem*> overlapItems = collidingItems();

    for (QGraphicsItem* item : overlapItems) {
        if (item->zValue() > 0.0 && item->zValue() < 2.0) {
            item->setZValue(0.0);
        }
    }

    // bring this node forward
    setZValue(1.0);

    m_nodeState.setHovered(true);

    update();

    Q_EMIT agxNodeScene()->nodeHovered(m_nodeId, event->screenPos());

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
        agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());

    event->accept();
}

void AgxNodeGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->m_lastHoveredNode = nullptr;
    //QGraphicsObject::hoverLeaveEvent(event);

    agxNodeScene()->setGroupHoverState(false,"");

    m_nodeState.setHovered(false);

    setZValue(0.0);

    update();

    Q_EMIT agxNodeScene()->nodeHoverLeft(m_nodeId);

    event->accept();
}

void AgxNodeGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    const auto pos = event->pos();

    //NodeGeometry geometry(_nodeId, _graphModel, nodeScene());
    const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

    if (m_graphModel.nodeFlags(m_nodeId) | AgxNodeFlag::Resizable
        && geometry.resizeHandleRect(m_nodeId).contains(QPoint(
                                                                static_cast<int>(pos.x()),
                                                                static_cast<int>(pos.y())))
    )
    {
        setCursor(QCursor(Qt::SizeFDiagCursor));
    }
    else
    {
        setCursor(QCursor());
    }

    event->accept();
}

void AgxNodeGraphicsObject::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);

    prepareGeometryChange();
    Q_EMIT agxNodeScene()->nodeDoubleClicked(m_nodeId);
    QTimer::singleShot(1, this, [this] { update(); });
}

void AgxNodeGraphicsObject::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    Q_EMIT agxNodeScene()->nodeContextMenu(m_nodeId, mapToScene(event->pos()));
}

void AgxNodeGraphicsObject::embedQWidget()
{
    const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();
    geometry.recomputeSize(m_nodeId);

    if (const auto w = m_graphModel.nodeData(m_nodeId, AgxNodeRole::Widget).value<QWidget*>()) {
        m_proxyWidget = new QGraphicsProxyWidget(this);

        m_proxyWidget->setWidget(w);

        m_proxyWidget->setPreferredWidth(5);

        geometry.recomputeSize(m_nodeId);

        if (w->sizePolicy().verticalPolicy() & QSizePolicy::ExpandFlag)
        {
            const unsigned int widgetHeight = static_cast<unsigned int>(geometry.size(m_nodeId).height() - geometry.captionRect(m_nodeId).height());

            // If the widget wants to use as much vertical space as possible, set
            // it to have the geom's equivalentWidgetHeight.
            m_proxyWidget->setMinimumHeight(widgetHeight);
        }

        updateQWidgetEmbedPos();

        //update();

        m_proxyWidget->setOpacity(1.0);
        m_proxyWidget->setFlag(ItemIgnoresParentOpacity);
        m_proxyWidget->setCacheMode(DeviceCoordinateCache);

        if (const auto wAgx = dynamic_cast<IAgxEmbedSceneData*>(w))
        {
            wAgx->SetRefData(m_nodeId, agxNodeScene());
        }
    }
}

void AgxNodeGraphicsObject::setLockedState()
{
    const AgxNodeFlags flags = m_graphModel.nodeFlags(m_nodeId);

    bool const locked = flags.testFlag(AgxNodeFlag::Locked);

    setFlag(ItemIsMovable, !locked);
    setFlag(ItemIsSelectable, !locked);
    setFlag(ItemSendsScenePositionChanges, !locked);
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

    if (m_nodeState.resizing()) {
        const auto diff = event->pos() - event->lastPos();

        if (const auto w = m_graphModel.nodeData(m_nodeId, AgxNodeRole::Widget).value<QWidget*>()) {
            prepareGeometryChange();

            auto oldSize = w->size();

            oldSize += QSizeF(diff.x(), diff.y()).toSize();

            w->resize(oldSize);

            const AgxNodeGeometry& geometry = agxNodeScene()->agxNodeGeometry();

            // Passes the new size to the model.
            geometry.recomputeSize(m_nodeId);

            update();

            moveConnections();

            event->accept();
        }
    }
    else {
        const auto diff = event->pos() - event->lastPos();

        agxNodeScene()->undoStack().push(new MoveNodeCommand(agxNodeScene(), diff));

        event->accept();
    }

    QRectF r = agxNodeScene()->sceneRect();

    r = r.united(mapToScene(boundingRect()).boundingRect());

    agxNodeScene()->setSceneRect(r);

}

void AgxNodeGraphicsObject::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget)
{
    updateQWidgetEmbedPos();

    painter->setClipRect(option->exposedRect);

    agxNodeScene()->agxNodePainter().paint(painter, *this);
}

QVariant AgxNodeGraphicsObject::itemChange(const GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemScenePositionHasChanged && scene()) {
        moveConnections();
    }

    return QGraphicsObject::itemChange(change, value);
}

AgxCommentGraphicsObject::AgxCommentGraphicsObject(AgxGraphicsScene& scene, const AgxNodeId nodeId) : AgxNodeGraphicsObject(scene, nodeId)
{
    setZValue(4);
}

QVariant AgxCommentGraphicsObject::itemChange(const GraphicsItemChange change, const QVariant& value)
{
    //if (change == ItemScenePositionHasChanged) //use causes hover/deselect/etc issues with the border margin and updating the arrow gobj... will fix later.
    {
        m_iData = m_graphModel.nodeData(m_nodeId, AgxNodeRole::InternalData).toJsonObject()["internal-data"].toObject();
        if (m_iData.contains("comment-target"))
        {
            if (!m_arrowGraphicsObject)
            {
                m_arrowGraphicsObject = new AgxArrowGraphicsObject();
                //_arrowGraphicsObject->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
                if (scene()) scene()->addItem(m_arrowGraphicsObject);

                connect(this, &QGraphicsObject::destroyed, m_arrowGraphicsObject, &QGraphicsObject::deleteLater);
                connect(m_arrowGraphicsObject, &AgxArrowGraphicsObject::TargetUpdated, [this](const QPointF& target) {
                    QJsonObject data;
                    QJsonObject targetData;
                    targetData["x"] = target.x();
                    targetData["y"] = target.y();
                    data["comment-target"] = targetData;

                    agxNodeScene()->undoStack().push(new InsertPropertySheetDataCommand(agxNodeScene(), m_nodeId, data));
                    m_iData = m_graphModel.nodeData(m_nodeId, AgxNodeRole::InternalData).toJsonObject()["internal-data"].toObject();
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
    auto target = m_iData["comment-target"].toObject();
    const QPointF tgtPt(target["x"].toDouble(), target["y"].toDouble());

    const QPointF origin = mapToScene(boundingRect().center());

    constexpr QPointF cornerPt(0.0, 0.0);
    const QLineF boundLine(boundingRect().center(), cornerPt);
    const auto distance = boundLine.length();
    const QLineF fullLine(origin, tgtPt);
    const auto t = distance / fullLine.length();
    QPointF lerpOrigin = origin + t * (tgtPt - origin);
    lerpOrigin = t < 1 ? lerpOrigin : origin;
    m_arrowGraphicsObject->OnParentUpdated(lerpOrigin, tgtPt, t < 1);
}

//void AgxCommentGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//    AgxNodeGraphicsObject::paint(painter, option, widget);
//
//    //agxNodeScene()->agxNodePainter().drawWrappedTextInWidgetBounds(painter, *this, _iData["property-sheet"].toObject()["comment"].toObject()["text"].toString());
//}

void AgxCommentGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    agxNodeScene()->m_lastHoveredNode = this;

    // bring this node forward
    setZValue(5);

    m_nodeState.setHovered(true);

    update();

    Q_EMIT agxNodeScene()->nodeHovered(m_nodeId, event->screenPos());

    if ((event->modifiers() & Qt::ShiftModifier) != 0)
        agxNodeScene()->setGroupHoverState(true, agxNodeScene()->getLastHoveredGroup());

    event->accept();
}

void AgxCommentGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    agxNodeScene()->m_lastHoveredNode = nullptr;
    //QGraphicsObject::hoverLeaveEvent(event);

    agxNodeScene()->setGroupHoverState(false, "");

    m_nodeState.setHovered(false);

    setZValue(4);

    update();

    Q_EMIT agxNodeScene()->nodeHoverLeft(m_nodeId);

    event->accept();
}

AgxArrowGraphicsObject::AgxArrowGraphicsObject(QGraphicsObject* parent) : QGraphicsObject(parent)
{
    m_connPalette = &AgxPalette::GetInstance().connectionPalette();
    setCacheMode(DeviceCoordinateCache);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setZValue(3); 
    
}

QRectF AgxArrowGraphicsObject::boundingRect() const
{
    const QPointF topLeft(
        qMin(m_origin.x(), m_target.x()) - 50.0,
        qMin(m_origin.y(), m_target.y()) - 50.0
    );
    const QPointF botRight(
        qMax(m_origin.x(), m_target.x()) + 50.0,
        qMax(m_origin.y(), m_target.y()) + 50.0
    );
    return {topLeft, botRight};
}

void AgxArrowGraphicsObject::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    constexpr qreal arrowSize = 15; // size of head

    QPen pen;

    pen.setWidth(static_cast<int>(m_connPalette->ConstructionLineWidth));
    
    if (isSelected())
        pen.setColor(m_connPalette->SelectedHaloColor);
    else if (m_hovered)
        pen.setColor(m_connPalette->HoveredColor);
    else
        pen.setColor(m_connPalette->ConstructionColor);
    
    pen.setStyle(Qt::DashLine);

    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    const QLineF line(m_target, m_origin);

    const double angle = std::atan2(-line.dy(), line.dx());
    const QPointF arrowP1 = line.p1() + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                          cos(angle + M_PI / 3) * arrowSize);
    const QPointF arrowP2 = line.p1() + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                          cos(angle + M_PI - M_PI / 3) * arrowSize);

    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << arrowP2 << line.p1() << arrowP1;
    m_arrowRect = QRectF(arrowHead.boundingRect().topLeft() - QPointF(10,10), arrowHead.boundingRect().bottomRight() + QPointF(10,10));

    if (m_renderLine) {
        painter->drawLine(line);
        setZValue(3);
    } else setZValue(2.1);

    painter->drawPolyline(arrowHead);
}

void AgxArrowGraphicsObject::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!m_hovered && !isSelected())
        event->ignore();
    else
        QGraphicsObject::mousePressEvent(event);
}

void AgxArrowGraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    m_targetPersistent = m_target;

    Q_EMIT TargetUpdated(m_targetPersistent);
}

void AgxArrowGraphicsObject::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    const auto temp = m_hovered;
    if (m_arrowRect.contains(event->pos()))
    {
        m_hovered = true;
    }
    else
    {
        m_hovered = false;
    }

    if (temp != m_hovered) update();

    QGraphicsObject::hoverEnterEvent(event);
}

void AgxArrowGraphicsObject::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    m_hovered = false;

    update();
    
    QGraphicsObject::hoverLeaveEvent(event);
}

void AgxArrowGraphicsObject::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
    const auto temp = m_hovered;
    if (m_arrowRect.contains(event->pos()))
    {
        m_hovered = true;
    } else
    {
        m_hovered = false;
    }

    if (temp != m_hovered)
        update();

    QGraphicsObject::hoverMoveEvent(event);
}

QVariant AgxArrowGraphicsObject::itemChange(const GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange)
    {
        const QPointF newPos = value.toPointF();
        const QPointF oldPos = pos();
        //qDebug() << "Pos " << newPos - oldPos;
        prepareGeometryChange();
        m_target = newPos - oldPos + m_targetPersistent;
        update();
        return {};
    }

    return QGraphicsObject::itemChange(change, value);
}

void AgxArrowGraphicsObject::OnParentUpdated(const QPointF & origin, const QPointF & target, const bool renderState)
{
    prepareGeometryChange();
    m_renderLine = renderState;
    m_origin = origin;
    m_target = target;
    m_targetPersistent = target;
    update();
}
