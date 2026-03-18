//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#include <utility>

#pragma warning(push,0)
#include <QUuid>
#include <QGraphicsObject>
#include "AgxConnectionState.h"

#pragma warning(pop)



class QGraphicsSceneMouseEvent;

class AgxGraphicsScene;
class AgxGraphModel;

class AgxConnectionGraphicsObject : public QGraphicsObject
{
	Q_OBJECT
public:
    // Needed for qgraphicsitem_cast
    enum { Type = UserType + 2 };

    int type() const override { return Type; }
public:
	AgxConnectionGraphicsObject(AgxGraphicsScene& scene, AgxConnectionId const connectionId);
	~AgxConnectionGraphicsObject() = default;

public:
    AgxGraphModel& graphModel() const;

    AgxGraphicsScene* agxNodeScene() const;

    AgxConnectionId const& connectionId() const;

    QRectF boundingRect() const override;

    QPainterPath shape() const override;

    QPointF const& endPoint(AgxPortType portType) const;

    QPointF out() const { return _out; }

    QPointF in() const { return _in; }

    std::pair<QPointF, QPointF> pointsC1C2() const;

    void setEndPoint(AgxPortType portType, QPointF const& point);

    /// Updates the position of both ends
    void move();

    const AgxConnectionState& connectionState() const;

    AgxConnectionState& connectionState();

    void setConnectionHidden(bool hidden);

protected:
    void paint(QPainter* painter,
        QStyleOptionGraphicsItem const* option,
        QWidget* widget = 0) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

private:
    void initializePosition();

    void addGraphicsEffect();

    std::pair<QPointF, QPointF> pointsC1C2Horizontal() const;

    std::pair<QPointF, QPointF> pointsC1C2Vertical() const;

    /// <summary>
    /// Takes into account the collapsed state and returns either the default position or the proper position
    /// </summary>
    /// <returns></returns>
    QPointF GetOutPosition() const;
    /// <summary>
    /// Takes into account the collapsed state and returns either the default position or the proper position
    /// </summary>
    /// <returns></returns>
    QPointF GetInPosition() const;

private:
    AgxConnectionId _connectionId;

    AgxGraphModel& _graphModel;

    AgxConnectionState _connectionState;

    mutable QPointF _out;
    mutable QPointF _in;
};