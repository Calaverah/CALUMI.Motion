//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <qobject.h>

#pragma warning(pop)

// ReSharper disable once CppUnusedIncludeDirective
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

    explicit AgxGraphModel(AgxGameType type, AgxGraphModel* rootGraph = nullptr);

    std::shared_ptr<AgxNodeDelegateModelRegistry> dataModelRegistry() { return m_registry; }

    ~AgxGraphModel() override = default;

    virtual QSet<AgxNodeId> allNodeIds() const;

    virtual std::unordered_set<AgxConnectionId> allConnectionIds(AgxNodeId nodeId) const;

    virtual std::unordered_set<AgxConnectionId> connections(AgxNodeId nodeId,
        AgxPortType portType,
        AgxPortIndex portIndex) const;

    virtual bool connectionExists(AgxConnectionId connectionId) const;

    virtual AgxNodeId addNode(QString nodeType);

    void HandleEmbeddedClosures();
    QWidget* GetNodeSidebarContent(const AgxNodeId& nodeId);

    virtual bool connectionPossible(AgxConnectionId connectionId) const;

    virtual void addConnection(AgxConnectionId connectionId);

    virtual bool nodeExists(AgxNodeId nodeId) const;

    /// Defines if detaching the connection is possible.
    virtual bool detachPossible(AgxConnectionId const) const { return true; }

    virtual QVariant nodeData(AgxNodeId nodeId, AgxNodeRole role) const;

    template<typename T>
    T nodeData(const AgxNodeId nodeId, const AgxNodeRole role) const
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
    QVector<AgxPropertyEntryDefinition>* GetPropertyEntries() { return &m_propertyEntries; }
    QMap<TermRef, AgxPropertyBlockData>* GetPropertyBlocks() { return &m_propertyBlocks; }
    QJsonObject SetNewGraphProperties(const AgxGraphType& graphType);
    AgxGraphType getGraphType() const { return m_graphType; }

    void setGraphCategory(const QString& cat);
    QString getGraphCategory() const { return m_category; }
    const AgxGraphModel* rootGraphReference() const;

Q_SIGNALS:
    void PropertySheetUpdated();
    void GraphTypeUpdated();

public:
    template<typename T>
    T portData(const AgxNodeId nodeId, const AgxPortType portType, const AgxPortIndex index, const AgxPortRole role) const
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
        AgxPortRole role);

    virtual void sendPortCommand(AgxNodeId nodeId,
                         AgxPortType portType,
                         AgxPortIndex portIndex,
                         const QString& command,
                         const QString& payload);

    virtual bool deleteConnection(AgxConnectionId connectionId);

    virtual bool deleteNode(AgxNodeId nodeId);

    /**
    * Fetches the NodeDelegateModel for the given `nodeId` and tries to cast the
    * stored pointer to the given type
    */
    template<typename NodeDelegateModelType>
    NodeDelegateModelType* delegateModel(AgxNodeId const nodeId)
    {
        const auto it = m_models.find(nodeId);
        if (it == m_models.end())
            return nullptr;

        auto model = dynamic_cast<NodeDelegateModelType*>(it->second.get());

        return model;
    }

    void SetNodeNameProperty(AgxNodeId nodeId, const QString& newName);
    QString GetNodeNameProperty(AgxNodeId nodeId) const;
    bool CanSetNodeNameProperty(AgxNodeId nodeId);

    void ToggleNodeCollapse(AgxNodeId nodeId);
    void SetNodesCollapsed(const QList<AgxNodeId>& nodes, bool collapsed = true);

    AgxPortType CanModifyPorts(AgxNodeId nodeId);
    AgxNodeType GetNodeType(AgxNodeId nodeId);

    QString GetNodeGroup(AgxNodeId nodeId) const;
    std::vector<QString> GetNodeGroupList() const;
    QColor GetGroupColor(QString groupName) const;
    void SetGroupColor(const QString& groupName, const QColor& color);
    std::unordered_map<QString, QVector<AgxNodeId>> GetNodeGroupAssignmentList() const;
    bool GroupExists(QString groupName) const;


    
    virtual bool loopsEnabled() const { return true; }

    virtual void addPort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, QJsonObject data);
    virtual QJsonObject removePort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, bool preserve);

    virtual QJsonObject saveNode(AgxNodeId nodeId) const;

    virtual QJsonObject save() const;
    virtual void save(pugi::xml_node& parent) const;

    /**
     *@brief Creates a new node based on the information in `nodeJson`.
     * @param nodeJson contains a `NodeId`, node's position, internal node
     * information.
     */
    virtual void loadNode(QJsonObject const& nodeJson);

    virtual void load(QJsonObject const& jsonDocument);
    virtual void load(pugi::xml_node& xmlNode);

Q_SIGNALS:
    void statusUpdate(float loadPercentage, const QString& message = QString());
    void inPortDataWasSet(AgxNodeId, AgxPortType, AgxPortIndex);
    void portCreated(AgxNodeId, AgxPortType, AgxPortIndex);
    void portDeleted(AgxNodeId, AgxPortType, AgxPortIndex);

public:
    virtual AgxNodeId newNodeId() { return m_nextNodeId++; }
    virtual AgxNodeId newMiscNodeId() {return m_nextMiscNodeId++; }

private:
    virtual void sendConnectionCreation(AgxConnectionId connectionId);

    virtual void sendConnectionDeletion(AgxConnectionId connectionId);

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
        virtual void portsAboutToBeDeleted(AgxNodeId nodeId,
                                   AgxPortType portType,
                                   AgxPortIndex first,
                                   AgxPortIndex last);

        /**
         * Signal emitted when model no longer has the old data associated with the
         * given port indices and when the node must be repainted.
         */
        virtual void portsDeleted();

        /**
         * Signal emitted when model is about to create new ports on the given node.
         * @param nodeId
         * @param portType
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
    void connectionCreated(AgxConnectionId connectionId);
    void connectionDeleted(AgxConnectionId connectionId);
    void nodeCreated(AgxNodeId nodeId);
    void nodeDeleted(AgxNodeId nodeId);
    void nodeUpdated(AgxNodeId nodeId);
    void nodeFlagsUpdated(AgxNodeId nodeId);
    void nodePositionUpdated(AgxNodeId nodeId);
    void modelReset();


private Q_SLOTS:
    /**
     * Function is called in three cases:
     *
     * - By underlying NodeDelegateModel when a node has new data to propagate.
     *   @see DataFlowGraphModel::addNode
     * - When a new connection is created.
     *   @see DataFlowGraphModel::addConnection
     * - When a node restored from JSON needs to send data downstream.
     *   @see DataFlowGraphModel::loadNode
     */
    void onOutPortDataUpdated(AgxNodeId nodeId, AgxPortIndex portIndex);

    /// Function is called after detaching a connection.
    void propagateEmptyDataTo(AgxNodeId nodeId, AgxPortIndex portIndex);

public Q_SLOTS:
    bool AddToNodeGroup(AgxNodeId nodeId, QString nodeGroup);
    void RemoveFromNodeGroup(AgxNodeId nodeId);
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

    /// <summary>
    /// Used for tab titling only
    /// </summary>
    /// <param name="title">Tab Title String</param>
    /// <param name="root"></param>
    void SetGraphTitle(const QString& title = QString(), bool root = true);
    /// <summary>
    /// Used for tab titling only
    /// </summary>
    /// <returns>Tab title string</returns>
    QString GetGraphTitle(bool root = true) const;

private:

    QString GetRelativeDataPath() const;

    //Used for tracking save/source file
    QString m_file;
    
    // Used For Tab Title
    QString m_agxGraphTitle = "untitled";

    AgxGameType m_gameType;
    AgxGraphType m_graphType = AgxGraphType::UNDEFINED;
    QString m_graphRelDataPath;
    QString m_category = "NONE";
    AgxGraphModel* m_rootReference = nullptr;

    std::shared_ptr<AgxNodeDelegateModelRegistry> m_registry;

    AgxNodeId m_nextNodeId; //max 3,999,999,999
    AgxNodeId m_nextMiscNodeId = 4000000000; //allows for 4,000,000,000 - max u32 nodes to be used for special cases like comments. 294,967,295 nodes can be used for this.
    AgxNodeId m_firstMiscNodeId = m_nextMiscNodeId;

    std::unordered_map<AgxNodeId, std::unique_ptr<AgxNode>> m_models;

    std::unordered_set<AgxConnectionId> m_connectivity;

    mutable std::unordered_map<AgxNodeId, NodeGeometryData> m_nodeGeometryData;

    std::unordered_map<QString, QColor> m_nodeGroups;

    QMap<TermRef, AgxPropertyBlockData> m_propertyBlocks;
    QList<TermRef> m_blockOrder;

    QVector<AgxPropertyEntryDefinition> m_propertyEntries;

    QVector<AgxConnectionId> m_shiftedByDynamicPortsConnections;

    friend class SFBGS_GraphPropertiesDialogWidget;
};