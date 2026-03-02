#pragma once
#pragma warning(push,0)
//#include "QtNodes/internal/Export.hpp
////#include "QtNodes/internal/Definitions.hpp
#include "Utilities/Hash/AgxConnectionIdHash.h"
#include "Utilities/Hash/QUuidStdHash.h"
#include <QGraphicsScene>
#include <QUuid>
#include <QMenu>

#pragma warning(pop)
#include "AgxGraphModel.h"
#include "AgxNodeGraphicsObject.h"
#include "AgxConnectionGraphicsObject.h"
#include "Painter/AgxNodePainter.h"
#include "Painter/AgxConnectionPainter.h"
#include "Painter/AgxNodeGeometry.h"
#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>

class QUndoStack;

class AgxConnectionPainter;
class AgxGraphModel;
class AgxNodePainter;
class AgxNodeGraphicsObject;
class AgxConnectionGraphicsObject;
class NodeStyle;

class AgxGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    AgxGraphicsScene(AgxGraphModel& graphModel, const QString& graphTitle = QString(), QObject* parent = nullptr);
    AgxGraphicsScene() = delete;

    ~AgxGraphicsScene() = default;

public:
    AgxGraphModel const& agxGraphModel() const;
    AgxGraphModel& agxGraphModel();

    AgxNodeGeometry const& agxNodeGeometry() const;
    AgxNodeGeometry& agxNodeGeometry();

    AgxNodePainter& agxNodePainter();
    AgxConnectionPainter& agxConnectionPainter();

    void setNodePainter(std::unique_ptr<AgxNodePainter> newPainter);
    void setConnectionPainter(std::unique_ptr<AgxConnectionPainter> newPainter);
    void setNodeGeometry(std::unique_ptr<AgxNodeGeometry> newGeom);

    QUndoStack& undoStack();

public:
    /**
     * @brief Creates a "draft" instance of ConnectionGraphicsObject.
     *
     * The scene caches a "draft" connection which has one loose end.
     * After attachment the "draft" instance is deleted and instead a
     * normal "full" connection is created.
     * Function @returns the "draft" instance for further geometry
     * manipulations.
     */
    std::unique_ptr<AgxConnectionGraphicsObject> const& makeDraftConnection(AgxConnectionId const newConnectionId);

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

public:

    AgxConnectionGraphicsObject* agxConnectionGraphicsObject(AgxConnectionId connectionId);
    AgxNodeGraphicsObject* agxNodeGraphicsObject(AgxNodeId nodeId);

    Qt::Orientation orientation() const { return _orientation; }
    void setOrientation(Qt::Orientation const orientation);
    
public:
    QMenu* createSceneMenu(QPointF const scenePos);

    std::vector<AgxNodeId> selectedNodes() const;
    
    QString getLastHoveredGroup();
    bool setGroupHoverState(bool value, QString groupId);

    void setConnectionsHidden(const QList<AgxConnectionId>& cids, bool hide);

    size_t nodeGraphicItemCount() const;
    size_t connectionGraphicItemCount() const;


Q_SIGNALS:
    void modified(AgxGraphicsScene*);
    void nodeMoved(AgxNodeId const nodeId, QPointF const& newLocation);
    void nodeClicked(AgxNodeId const nodeId);
    void nodeSelected(AgxNodeId const nodeId);
    void nodeDoubleClicked(AgxNodeId const nodeId);
    void nodeHovered(AgxNodeId const nodeId, QPoint const screenPos);
    void nodeHoverLeft(AgxNodeId const nodeId);
    void connectionHovered(AgxConnectionId const connectionId, QPoint const screenPos);
    void connectionHoverLeft(AgxConnectionId const connectionId);
    void sceneLoaded();
    void nodePreClicked(AgxNodeId const nodeId, bool additive = false);
    void nodeContextMenu(AgxNodeId const nodeId, QPointF const pos);

protected:
    void traverseGraphAndPopulateGraphicsObjects();

    void updateAttachedNodes(AgxConnectionId const connectionId, AgxPortType const portType);


public Q_SLOTS:
    void onConnectionDeleted(AgxConnectionId const connectionId);
    void onConnectionCreated(AgxConnectionId const connectionId);

    void onNodeDeleted(AgxNodeId const nodeId);
    void onNodeCreated(AgxNodeId const nodeId);
    void onNodePositionUpdated(AgxNodeId const nodeId);
    void onNodeUpdated(AgxNodeId const nodeId);
    void onNodeClicked(AgxNodeId const nodeId);
    void onModelReset();

    void onSelectAllObjectsOfType(AgxGraphicsItemsFlags flags);
    void onSelectNodes(const QList<AgxNodeId>& nodesToSelect);
    void onSelectedConnections(const QList<AgxConnectionId>& connsToSelect);
    inline void onSelectAnyAndAllObjects() { onSelectAllObjectsOfType(AgxGraphicsItemsFlag::All); }
    inline void onSelectAllNodes() { onSelectAllObjectsOfType(AgxGraphicsItemsFlag::Node); }
    inline void onSelectAllConnections() { onSelectAllObjectsOfType(AgxGraphicsItemsFlag::Connection); }
    void onRightRefreshSideBarVisibility();

public Q_SLOTS:
    inline bool save() const { return false; }
    inline bool load() { return false; }

private:
    AgxGraphModel& _agxGraphModel;
    std::unique_ptr<AgxConnectionGraphicsObject> _agxDraftConnection;
    std::unordered_map<AgxNodeId, std::unique_ptr<AgxNodeGraphicsObject>> _agxNodeGraphicsObjects;
    std::unordered_map<AgxConnectionId, std::unique_ptr<AgxConnectionGraphicsObject>> _agxConnectionGraphicsObjects;
    std::unique_ptr<AgxNodeGeometry> _agxNodeGeometry;
    std::unique_ptr<AgxNodePainter> _agxNodePainter;
    std::unique_ptr<AgxConnectionPainter> _agxConnectionPainter;
    bool _nodeDrag;
    QUndoStack* _undoStack;
    Qt::Orientation _orientation;

    QString _agxGraphTitle = "untitled";
    
public:
    bool _groupHovered = false;
    AgxNodeGraphicsObject* lastHoveredNode = nullptr;
    
    
};