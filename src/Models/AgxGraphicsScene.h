//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QGraphicsScene>

#pragma warning(pop)
#include "AgxGraphModel.h"
#include "AgxNodeGraphicsObject.h"
#include "AgxConnectionGraphicsObject.h"
#include "Painter/AgxNodePainter.h"
#include "Painter/AgxConnectionPainter.h"
#include "Painter/AgxNodeGeometry.h"
#include <memory>
#include <unordered_map>

class QUndoStack;
class AgxConnectionPainter;
class AgxGraphModel;
class AgxNodePainter;
class AgxNodeGraphicsObject;
class AgxConnectionGraphicsObject;

class AgxGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit AgxGraphicsScene(std::shared_ptr<AgxGraphModel> graphModel, QObject* parent = nullptr);
    AgxGraphicsScene() = delete;

    ~AgxGraphicsScene() override = default;

    AgxGraphModel const& agxGraphModel() const;
    AgxGraphModel& agxGraphModel();

    AgxNodeGeometry const& agxNodeGeometry() const;
    AgxNodeGeometry& agxNodeGeometry();

    AgxNodePainter& agxNodePainter() const;
    AgxConnectionPainter& agxConnectionPainter() const;

    void setNodePainter(std::unique_ptr<AgxNodePainter> newPainter);
    void setConnectionPainter(std::unique_ptr<AgxConnectionPainter> newPainter);
    void setNodeGeometry(std::unique_ptr<AgxNodeGeometry> newGeom);

    QUndoStack& undoStack() const;

/**
     * @brief Creates a "draft" instance of ConnectionGraphicsObject.
     *
     * The scene caches a "draft" connection which has one loose end.
     * After attachment the "draft" instance is deleted and instead a
     * normal "full" connection is created.
     * Function @returns the "draft" instance for further geometry
     * manipulations.
     */
    std::unique_ptr<AgxConnectionGraphicsObject> const& makeDraftConnection(const AgxConnectionId& newConnectionId);

    /**
     * @brief Deletes "draft" connection.
     *
     * The function is called when user releases the mouse button during
     * the construction of the new connection without attaching it to any
     * node.
     */
    void resetDraftConnection();

    /// Deletes all the nodes. Connections are removed automatically.
    void clearScene();

    AgxConnectionGraphicsObject* agxConnectionGraphicsObject(const AgxConnectionId& connectionId);
    AgxNodeGraphicsObject* agxNodeGraphicsObject(AgxNodeId nodeId);

    Qt::Orientation orientation() const { return m_orientation; }
    void setOrientation(Qt::Orientation orientation);

    QMenu* createSceneMenu(QPointF scenePos);

    [[nodiscard]] QVector<AgxNodeId> selectedNodes() const;
    
    QString getLastHoveredGroup() const;
    bool setGroupHoverState(bool shouldHover, const QString& groupId);

    void setConnectionsHidden(const QList<AgxConnectionId>& cidList, bool hide);

    size_t nodeGraphicItemCount() const;
    size_t connectionGraphicItemCount() const;

    bool isNodeSelected(AgxNodeId nodeId) const;


Q_SIGNALS:
    void modified(AgxGraphicsScene*agxScene);
    void nodeMoved(const AgxNodeId& nodeId, const QPointF& newLocation);
    void nodeClicked(const AgxNodeId& nodeId);
    void nodeSelected(const AgxNodeId& nodeId);
    void nodeGODeselected(const AgxNodeId& nodeId);
    void nodeGOSelected(const AgxNodeId& nodeId);
    void nodeDoubleClicked(const AgxNodeId& nodeId);
    void nodeHovered(const AgxNodeId& nodeId, QPoint screenPos);
    void nodeHoverLeft(const AgxNodeId& nodeId);
    void connectionHovered(AgxConnectionId connectionId, QPoint screenPos);
    void connectionHoverLeft(AgxConnectionId connectionId);
    void sceneLoaded();
    void nodePreClicked(const AgxNodeId& nodeId, bool additive = false);
    void nodeContextMenu(const AgxNodeId& nodeId, QPointF pos);

protected:
    void traverseGraphAndPopulateGraphicsObjects();
    void updateAttachedNodes(const AgxConnectionId& connectionId, const AgxPortType& portType);

public Q_SLOTS:
    void onConnectionDeleted(const AgxConnectionId& connectionId);
    void onConnectionCreated(const AgxConnectionId& connectionId);

    void onNodeDeleted(const AgxNodeId& nodeId);
    void onNodeCreated(const AgxNodeId& nodeId);
    void onNodePositionUpdated(const AgxNodeId& nodeId);
    void onNodeUpdated(const AgxNodeId& nodeId);
    void onNodeClicked(const AgxNodeId& nodeId);
    void onModelReset();

    void onSelectAllObjectsOfType(AgxGraphicsItemsFlags flags);
    void onSelectNodes(const QList<AgxNodeId>& nodesToSelect);
    void onSelectedConnections(const QList<AgxConnectionId>& connsToSelect);
    void onSelectAnyAndAllObjects();
    void onSelectAllNodes();
    void onSelectAllConnections();

private:
    std::shared_ptr<AgxGraphModel> m_agxGraphModel;
    std::unique_ptr<AgxConnectionGraphicsObject> m_agxDraftConnection;
    std::unordered_map<AgxNodeId, std::unique_ptr<AgxNodeGraphicsObject>> m_agxNodeGraphicsObjects;
    std::unordered_map<AgxConnectionId, std::unique_ptr<AgxConnectionGraphicsObject>> m_agxConnectionGraphicsObjects;
    std::unique_ptr<AgxNodeGeometry> m_agxNodeGeometry;
    std::unique_ptr<AgxNodePainter> m_agxNodePainter;
    std::unique_ptr<AgxConnectionPainter> m_agxConnectionPainter;
    bool m_nodeDrag;
    QUndoStack* m_undoStack;
    Qt::Orientation m_orientation;
    
public:
    bool m_groupHovered = false;
    AgxNodeGraphicsObject* m_lastHoveredNode = nullptr;
    
    
};