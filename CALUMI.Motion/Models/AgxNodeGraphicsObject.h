#pragma once


#pragma warning(push,0)
#include <QUuid>
#include <QGraphicsObject>
#pragma warning(pop)
#include "AgxNodeState.h"
#include "Painter/AgxConnectionStyle.h"
#include <Utilities/AgxDefinitions.h>

namespace QtNodes {
    class AbstractGraphModel;
}

class AgxGraphicsScene;
class AgxGraphModel;

class AgxNodeGraphicsObject : public QGraphicsObject
{
	Q_OBJECT
public:
	// Needed for qgraphicsitem_cast
	enum { Type = UserType + 1 };

	int type() const override { return Type; }

public:
	AgxNodeGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId);

	~AgxNodeGraphicsObject() override = default;

	AgxGraphicsScene* agxNodeScene() const;

public:
    AgxGraphModel& graphModel() const;

    AgxNodeId nodeId() { return _nodeId; }

    AgxNodeId nodeId() const { return _nodeId; }

    AgxNodeState& nodeState() { return _nodeState; }

    AgxNodeState const& nodeState() const { return _nodeState; }

    QRectF boundingRect() const override;

    void setGeometryChanged();

    /// Visits all attached connections and corrects
    /// their corresponding end points.
    void moveConnections() const;

    /// Repaints the node once with reacting ports.
    void reactToConnection(AgxConnectionGraphicsObject const* cgo);

    void updateQWidgetEmbedPos();

protected:
    void paint(QPainter* painter,
        QStyleOptionGraphicsItem const* option,
        QWidget* widget = nullptr) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;


    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

private:
    void embedQWidget();
    void setLockedState();

protected:
    AgxNodeId _nodeId;

    AgxGraphModel& _graphModel;

    AgxNodeState _nodeState;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget* _proxyWidget;

    friend class AgxNodePainter;
};




class AgxArrowGraphicsObject : public QGraphicsObject 
{
    Q_OBJECT
public:
    AgxArrowGraphicsObject(QGraphicsObject* parent = nullptr);

    // Inherited via QGraphicsObject
    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

public Q_SLOTS:
    void OnParentUpdated(const QPointF& origin, const QPointF& target, bool renderState = true);

Q_SIGNALS:
    void TargetUpdated(const QPointF& target);

private:
    bool _renderLine = true;
    bool _hovered = false;
    QPointF _origin;
    QPointF _target;
    QPointF _targetPersistent;
    AgxConnectionStyle _connectionStyle;
    QRectF _arrowRect;

    friend class AgxNodeGraphicsObject;
};





class AgxCommentGraphicsObject : public AgxNodeGraphicsObject {

    Q_OBJECT
public:
    AgxCommentGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId);

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    //void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

public Q_SLOT:
    void RecalculateTarget() const;

private:
    AgxArrowGraphicsObject* _arrowGraphicsObject = nullptr;
    QJsonObject _iData;
};