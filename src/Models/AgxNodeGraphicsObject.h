//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once


#pragma warning(push,0)
#include <QGraphicsObject>
#pragma warning(pop)
#include "AgxNodeState.h"
#include <Utilities/AgxDefinitions.h>

#include "Painter/AgxPalette.h"

class AgxGraphicsScene;
class AgxGraphModel;

class AgxNodeGraphicsObject : public QGraphicsObject
{
	Q_OBJECT
public:
	// Needed for qgraphicsitem_cast
	enum { Type = UserType + 1 };

	int type() const override { return Type; } // NOLINT(*-use-nodiscard)

    AgxNodeGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId);

	~AgxNodeGraphicsObject() override = default;

	[[nodiscard]] AgxGraphicsScene* agxNodeScene() const;

    [[nodiscard]] AgxGraphModel& graphModel() const;

    [[nodiscard]] AgxNodeId nodeId() const { return m_nodeId; }

    [[nodiscard]] AgxNodeState& nodeState() { return m_nodeState; }

    [[nodiscard]] AgxNodeState const& nodeState() const { return m_nodeState; }

    QRectF boundingRect() const override; // NOLINT(*-use-nodiscard)

    void setGeometryChanged();

    /// Visits all attached connections and corrects
    /// their corresponding end points.
    void moveConnections() const;

    /// Repaints the node once with reacting ports.
    void reactToConnection(AgxConnectionGraphicsObject const* cgo);

    void updateQWidgetEmbedPos() const;

    void paint(QPainter* painter,
        QStyleOptionGraphicsItem const* option,
        QWidget* widget) override;

protected:
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
    AgxNodeId m_nodeId;

    AgxGraphModel& m_graphModel;

    AgxNodeState m_nodeState;

    // either nullptr or owned by parent QGraphicsItem
    QGraphicsProxyWidget* m_proxyWidget;

    friend class AgxNodePainter;
};




class AgxArrowGraphicsObject : public QGraphicsObject 
{
    Q_OBJECT
public:
    explicit AgxArrowGraphicsObject(QGraphicsObject* parent = nullptr);

    // Inherited via QGraphicsObject
    QRectF boundingRect() const override; // NOLINT(*-use-nodiscard)

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

public Q_SLOTS:
    void OnParentUpdated(const QPointF& origin, const QPointF& target, bool renderState = true);

Q_SIGNALS:
    void TargetUpdated(const QPointF& target);

private:
    bool m_renderLine = true;
    bool m_hovered = false;
    QPointF m_origin;
    QPointF m_target;
    QPointF m_targetPersistent;
    const AgxConnectionPalette* m_connPalette = nullptr;
    QRectF m_arrowRect;

    friend class AgxNodeGraphicsObject;
};





class AgxCommentGraphicsObject : public AgxNodeGraphicsObject {

    Q_OBJECT
public:
    AgxCommentGraphicsObject(AgxGraphicsScene& scene, AgxNodeId nodeId);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

    //void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

void RecalculateTarget() const;

private:
    AgxArrowGraphicsObject* m_arrowGraphicsObject = nullptr;
    QJsonObject m_iData;
};