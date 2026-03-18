//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <qobject.h>

#pragma warning(pop)

#include "Utilities/Hash/AgxConnectionIdHash.h"
#include "Utilities/AgxDefinitions.h"
#include "AgxNodes/AgxNode.h"
#include "Painter/QColorHelper.h"
#include <Utilities/AgxNodeDelegateModelRegistry.h>

class AgxGraphModel : public QObject
{
    Q_OBJECT

public:
    struct NodeGeometryData
    {
        QSize size;
        QPointF pos;
    };

public:
    AgxGraphModel(AgxGameType type, AgxGraphModel* rootGraph = nullptr);

    inline std::shared_ptr<AgxNodeDelegateModelRegistry> dataModelRegistry() { return _registry; }

    ~AgxGraphModel() override = default;

    virtual QSet<AgxNodeId> allNodeIds() const;

    virtual std::unordered_set<AgxConnectionId> allConnectionIds(AgxNodeId const nodeId) const;

    virtual std::unordered_set<AgxConnectionId> connections(AgxNodeId nodeId,
        AgxPortType portType,
        AgxPortIndex portIndex) const;

    virtual bool connectionExists(AgxConnectionId const connectionId) const;

    virtual AgxNodeId addNode(QString const nodeType);

    void HandleEmbeddedClosures();
    QWidget* GetNodeSidebarContent(const AgxNodeId& nodeId);
    void SetNodeSidebarVisibility(const AgxNodeId& nodeId, bool state);

    virtual bool connectionPossible(AgxConnectionId const connectionId) const;

    virtual void addConnection(AgxConnectionId const connectionId);

    virtual bool nodeExists(AgxNodeId const nodeId) const;

    /// Defines if detaching the connection is possible.
    virtual bool detachPossible(AgxConnectionId const) const { return true; }

    virtual QVariant nodeData(AgxNodeId nodeId, AgxNodeRole role) const;

    template<typename T>
    inline T nodeData(AgxNodeId nodeId, AgxNodeRole role) const
    {
        return nodeData(nodeId, role).value<T>();
    }

    virtual AgxNodeFlags nodeFlags(AgxNodeId nodeId) const;

    virtual bool setNodeData(AgxNodeId nodeId, AgxNodeRole role, QVariant value);

    void insertPropertySheetData(AgxNodeId nodeId, QJsonObject data);
    void insertPropertySheetData(QJsonObject data);
    QJsonObject getPropertySheetData(AgxNodeId nodeId, bool cleared = false);
    QJsonObject getPropertySheetData(bool cleared = false);
    QJsonObject getPropertySheetData(bool cleared = false) const;

    AgxPropertyBlockData* getPropertyBlock(const QString& block);
    AgxPropertyBlockData* getPropertyBlock(TermRef ref);
    void addPropertyBlockEntry(AgxNodeId nodeId, QString block, int index, const QList<AgxPropertyBlockData::Entry>& data = {});
    void addPropertyBlockEntry(QString block, int index, const QList<AgxPropertyBlockData::Entry>& data = {});
    QList<AgxPropertyBlockData::Entry> removePropertyBlockEntry(AgxNodeId nodeId, QString block, int index);
    QList<AgxPropertyBlockData::Entry> removePropertyBlockEntry(QString block, int index);
    inline QVector<AgxPropertyEntryDefinition>* GetPropertyEntries() { return &_PropertyEntries; }
    inline QMap<TermRef, AgxPropertyBlockData>* GetPropertyBlocks() { return &_PropertyBlocks; }
    QJsonObject SetNewGraphProperties(const AgxGraphType& graphType);
    inline AgxGraphType getGraphType() const { return _graphType; }

    void setGraphCategory(const QString& cat);
    inline QString getGraphCategory() const { return _category; }
    const AgxGraphModel* rootGraphReference() const;

Q_SIGNALS:
    void PropertySheetUpdated();
    void GraphTypeUpdated();

public:
    template<typename T>
    inline T portData(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex index, AgxPortRole role) const
    {
        return portData(nodeId, portType, index, role).value<T>();
    }

    virtual QVariant portData(AgxNodeId nodeId,
        AgxPortType portType,
        AgxPortIndex portIndex,
        AgxPortRole role) const;

    virtual bool setPortData(AgxNodeId nodeId,
        AgxPortType portType,
        AgxPortIndex portIndex,
        QVariant const& value,
        AgxPortRole role = AgxPortRole::Data);

    virtual void sendPortCommand(AgxNodeId nodeId,
                         AgxPortType portType,
                         AgxPortIndex portIndex,
                         const QString& command,
                         const QString& payload);

    virtual bool deleteConnection(AgxConnectionId const connectionId);

    virtual bool deleteNode(AgxNodeId const nodeId);

    /**
    * Fetches the NodeDelegateModel for the given `nodeId` and tries to cast the
    * stored pointer to the given type
    */
    template<typename NodeDelegateModelType>
    inline NodeDelegateModelType* delegateModel(AgxNodeId const nodeId)
    {
        auto it = _models.find(nodeId);
        if (it == _models.end())
            return nullptr;

        auto model = dynamic_cast<NodeDelegateModelType*>(it->second.get());

        return model;
    }

    void SetNodeNameProperty(const AgxNodeId nodeId, const QString& newName);
    QString GetNodeNameProperty(const AgxNodeId nodeId) const;
    bool CanSetNodeNameProperty(const AgxNodeId nodeId);

    void ToggleNodeCollapse(const AgxNodeId nodeId);
    void SetNodesCollapsed(const QList<AgxNodeId>& nodes, bool collapsed = true);

    AgxPortType CanModifyPorts(const AgxNodeId nodeId);
    AgxNodeType GetNodeType(const AgxNodeId nodeId);

    QString GetNodeGroup(const AgxNodeId nodeId) const;
    std::vector<QString> GetNodeGroupList() const;
    QColor GetGroupColor(QString groupName) const;
    void SetGroupColor(const QString& groupName, const QColor& color);
    std::unordered_map<QString, QVector<AgxNodeId>> GetNodeGroupAssignmentList() const;
    bool GroupExists(QString groupName) const;


    
    virtual inline bool loopsEnabled() const { return true; }

    virtual void addPort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, QJsonObject data = QJsonObject());
    virtual QJsonObject removePort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, bool preserve = false);

    virtual QJsonObject saveNode(AgxNodeId const nodeId) const;

    virtual QJsonObject save() const;
    virtual void save(pugi::xml_node& parent) const;

    /// @brief Creates a new node based on the informatoin in `nodeJson`.
    /**
   * @param nodeJson conains a `NodeId`, node's position, internal node
   * information.
   */
    virtual void loadNode(QJsonObject const& nodeJson);

    virtual void load(QJsonObject const& jsonDocument);
    virtual void load(pugi::xml_node& xmlNode);

Q_SIGNALS:
    void statusUpdate(float loadPercentage, const QString& message = QString());
    void inPortDataWasSet(AgxNodeId const, AgxPortType const, AgxPortIndex const);
    void portCreated(AgxNodeId const, AgxPortType const, AgxPortIndex const);
    void portDeleted(AgxNodeId const, AgxPortType const, AgxPortIndex const);

public:
    virtual inline AgxNodeId newNodeId() { return _nextNodeId++; }
    virtual inline AgxNodeId newMiscNodeId() {return _nextMiscNodeId++; }

private:
    virtual void sendConnectionCreation(AgxConnectionId const connectionId);

    virtual void sendConnectionDeletion(AgxConnectionId const connectionId);

    public:
        /**
         * Function clears connections attached to the ports that are scheduled to be
         * deleted. It must be called right before the model removes its old port data.
         *
         * @param nodeId Defines the node to be modified
         * @param portType Is either PortType::In or PortType::Out
         * @param first Index of the first port to be removed
         * @param last Index of the last port to be removed
         */
        virtual void portsAboutToBeDeleted(AgxNodeId const nodeId,
                                   AgxPortType const portType,
                                   AgxPortIndex const first,
                                   AgxPortIndex const last);

        /**
         * Signal emitted when model no longer has the old data associated with the
         * given port indices and when the node must be repainted.
         */
        virtual void portsDeleted();

        /**
         * Signal emitted when model is about to create new ports on the given node.
         * @param first Is the first index of the new port after insertion.
         * @param last Is the last index of the new port after insertion.
         *
         * Function caches existing connections that are located after the `last` port
         * index. For such connections the new "post-insertion" addresses are computed
         * and stored until the function AbstractGraphModel::portsInserted is called.
         */
        virtual void portsAboutToBeInserted(const AgxNodeId& nodeId,
                                            const AgxPortType& portType,
                                            const AgxPortIndex& first,
                                            const AgxPortIndex& last);

        /**
         * Function re-creates the connections that were shifted during the port
         * insertion. After that the node is updated.
         */
        virtual void portsInserted();

Q_SIGNALS:
    void connectionCreated(AgxConnectionId const connectionId);
    void connectionDeleted(AgxConnectionId const connectionId);
    void nodeCreated(AgxNodeId const nodeId);
    void nodeDeleted(AgxNodeId const nodeId);
    void nodeUpdated(AgxNodeId const nodeId);
    void nodeFlagsUpdated(AgxNodeId const nodeId);
    void nodePositionUpdated(AgxNodeId const nodeId);
    void modelReset();


private Q_SLOTS:
    /**
     * Fuction is called in three cases:
     *
     * - By underlying NodeDelegateModel when a node has new data to propagate.
     *   @see DataFlowGraphModel::addNode
     * - When a new connection is created.
     *   @see DataFlowGraphModel::addConnection
     * - When a node restored from JSON an needs to send data downstream.
     *   @see DataFlowGraphModel::loadNode
     */
    void onOutPortDataUpdated(AgxNodeId const nodeId, AgxPortIndex const portIndex);

    /// Function is called after detaching a connection.
    void propagateEmptyDataTo(AgxNodeId const nodeId, AgxPortIndex const portIndex);

public Q_SLOTS:
    bool AddToNodeGroup(const AgxNodeId nodeId, QString nodeGroup);
    void RemoveFromNodeGroup(const AgxNodeId nodeId);
    bool CreateNodeGroup(QString nodeGroup, QColor groupColor = generateRandomQColor());
    bool EraseNodeGroup(QString nodeGroup);

public:
    /// <summary>
    /// Only for exporting to the AGX file type.
    /// </summary>
    /// <param name="fileName">file name</param>
    /// <param name="relPath">optional: Game relative path override</param>
    void SetRelativeDataPath(const QString& fileName, const QString& relPath = QString());

    /// <summary>
    /// Used for tracking save/source file location
    /// </summary>
    /// <returns>Full File Path</returns>
    const QString& GetModelFilePath() const;
    /// <summary>
    /// Used for tracking save/source file location
    /// </summary>
    /// <param name="file">Full File Path</param>
    void SetModelFilePath(const QString& file);

public:
    /// <summary>
    /// Used for tab titling only
    /// </summary>
    /// <param name="title">Tab Title String</param>
    void SetGraphTitle(const QString& title = QString(), bool root = true);
    /// <summary>
    /// Used for tab titling only
    /// </summary>
    /// <returns>Tab title string</returns>
    QString GetGraphTitle(bool root = true) const;

private:

    QString GetRelativeDataPath() const;

    //Used for tracking save/source file
    QString _file;
    
    // Used For Tab Title
    QString _agxGraphTitle = "untitled";

    AgxGameType _gameType;
    AgxGraphType _graphType = AgxGraphType::UNDEFINED;
    QString _graphRelDataPath;
    QString _category = "NONE";
    AgxGraphModel* _rootReference = nullptr;

    std::shared_ptr<AgxNodeDelegateModelRegistry> _registry;

    AgxNodeId _nextNodeId; //max 3,999,999,999
    AgxNodeId _nextMiscNodeId = static_cast<AgxNodeId>(4000000000); //allows for 4,000,000,000 - max u32 nodes to be used for special cases like comments. 294,967,295 nodes can be used for this.
    AgxNodeId _firstMiscNodeId = _nextMiscNodeId;

    std::unordered_map<AgxNodeId, std::unique_ptr<AgxNode>> _models;

    std::unordered_set<AgxConnectionId> _connectivity;

    mutable std::unordered_map<AgxNodeId, NodeGeometryData> _nodeGeometryData;

    std::unordered_map<QString, QColor> _nodeGroups;

    QMap<TermRef, AgxPropertyBlockData> _PropertyBlocks;
    QList<TermRef> _BlockOrder;

    QVector<AgxPropertyEntryDefinition> _PropertyEntries;

private:
    QVector<AgxConnectionId> _shiftedByDynamicPortsConnections;

    friend class SFBGS_GraphPropertiesDialogWidget;
};