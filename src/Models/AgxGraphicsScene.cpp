//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFAMemoryLeak
#include "stdafx.h"
#include "AgxGraphicsScene.h"
#pragma warning(push,0)

#pragma warning(pop)
#include "Painter/AgxNodeGeometry.h"
#include "Painter/AgxNodePainter.h"
#include "Painter/AgxConnectionPainter.h"
#include "Utilities/UndoRedoCommands.h"
#include "AgxConnectionGraphicsObject.h"
#include "AgxNodeGraphicsObject.h"
#include <Application/CALUMIMotionApplication.h>
#include <Utilities/AgxConnectionIdUtils.h>

#include <ranges>

#include "Utilities/QWidgetFactories.h"

AgxGraphicsScene::AgxGraphicsScene(AgxGraphModel& graphModel, QObject* parent) : QGraphicsScene(parent)
                                                                               , m_agxGraphModel(graphModel)
                                                                               , m_agxNodeGeometry(std::make_unique<AgxNodeGeometry>(m_agxGraphModel))
                                                                               , m_agxNodePainter(std::make_unique<AgxNodePainter>())
                                                                               , m_agxConnectionPainter(std::make_unique<AgxConnectionPainter>())
                                                                               , m_nodeDrag(false)
                                                                               , m_undoStack(new QUndoStack(this))
                                                                               , m_orientation(Qt::Horizontal)
{

    setItemIndexMethod(NoIndex);

    connect(&m_agxGraphModel,
        &AgxGraphModel::connectionCreated,
        this,
        &AgxGraphicsScene::onConnectionCreated);

    connect(&m_agxGraphModel,
        &AgxGraphModel::connectionDeleted,
        this,
        &AgxGraphicsScene::onConnectionDeleted);

    connect(&m_agxGraphModel,
        &AgxGraphModel::nodeCreated,
        this,
        &AgxGraphicsScene::onNodeCreated);

    connect(&m_agxGraphModel,
        &AgxGraphModel::nodeDeleted,
        this,
        &AgxGraphicsScene::onNodeDeleted);

    connect(&m_agxGraphModel,
        &AgxGraphModel::nodePositionUpdated,
        this,
        &AgxGraphicsScene::onNodePositionUpdated);

    connect(&m_agxGraphModel,
        &AgxGraphModel::nodeUpdated,
        this,
        &AgxGraphicsScene::onNodeUpdated);

    connect(this, &AgxGraphicsScene::nodeClicked, this, &AgxGraphicsScene::onNodeClicked);

    connect(&m_agxGraphModel, &AgxGraphModel::modelReset, this, &AgxGraphicsScene::onModelReset);

    connect(&graphModel,
        &AgxGraphModel::inPortDataWasSet,
        [this](AgxNodeId const nodeId, AgxPortType const, AgxPortIndex const) { onNodeUpdated(nodeId); });

    traverseGraphAndPopulateGraphicsObjects();

}

AgxGraphModel const& AgxGraphicsScene::agxGraphModel() const { return m_agxGraphModel; }

AgxGraphModel& AgxGraphicsScene::agxGraphModel() { return m_agxGraphModel; }

AgxNodeGeometry const& AgxGraphicsScene::agxNodeGeometry() const
{
    return *m_agxNodeGeometry;
}

AgxNodeGeometry& AgxGraphicsScene::agxNodeGeometry()
{
    return *m_agxNodeGeometry;
}

std::unique_ptr<AgxConnectionGraphicsObject> const& AgxGraphicsScene::makeDraftConnection(const AgxConnectionId& newConnectionId)
{
    m_agxDraftConnection = std::make_unique<AgxConnectionGraphicsObject>(*this, newConnectionId);

    m_agxDraftConnection->grabMouse();

    return m_agxDraftConnection;
}

AgxConnectionGraphicsObject* AgxGraphicsScene::agxConnectionGraphicsObject(const AgxConnectionId& connectionId)
{
    AgxConnectionGraphicsObject* cgo = nullptr;
    if (const auto it = m_agxConnectionGraphicsObjects.find(connectionId); it != m_agxConnectionGraphicsObjects.end()) {
        cgo = it->second.get();
    }
    if (cgo)
        return cgo;

    return nullptr;
}

AgxNodeGraphicsObject* AgxGraphicsScene::agxNodeGraphicsObject(const AgxNodeId nodeId)
{
    AgxNodeGraphicsObject* ngo = nullptr;
    if (const auto it = m_agxNodeGraphicsObjects.find(nodeId); it != m_agxNodeGraphicsObjects.end()) {
        ngo = it->second.get();
    }

    return ngo;
}

void AgxGraphicsScene::resetDraftConnection()
{
    m_agxDraftConnection.reset();
}

void AgxGraphicsScene::clearScene()
{
    for (auto const& allNodeIds = agxGraphModel().allNodeIds(); const auto nodeId : allNodeIds) {
        agxGraphModel().deleteNode(nodeId);
    }
}

AgxNodePainter& AgxGraphicsScene::agxNodePainter() const
{
    return *m_agxNodePainter;
}

AgxConnectionPainter& AgxGraphicsScene::agxConnectionPainter() const
{
    return *m_agxConnectionPainter;
}

void AgxGraphicsScene::setNodePainter(std::unique_ptr<AgxNodePainter> newPainter)
{
    m_agxNodePainter = std::move(newPainter);
}

void AgxGraphicsScene::setConnectionPainter(std::unique_ptr<AgxConnectionPainter> newPainter)
{
    m_agxConnectionPainter = std::move(newPainter);
}

void AgxGraphicsScene::setNodeGeometry(std::unique_ptr<AgxNodeGeometry> newGeom)
{
    m_agxNodeGeometry = std::move(newGeom);
}

QUndoStack& AgxGraphicsScene::undoStack() const
{
    return *m_undoStack;
}

void AgxGraphicsScene::setOrientation(Qt::Orientation const orientation)
{
    if (m_orientation != orientation)
    {
        m_orientation = orientation;

        m_agxNodeGeometry = std::make_unique<AgxNodeGeometry>(m_agxGraphModel);

        onModelReset();
    }
}

void AgxGraphicsScene::onConnectionDeleted(const AgxConnectionId& connectionId)
{
    if (const auto it = m_agxConnectionGraphicsObjects.find(connectionId); it != m_agxConnectionGraphicsObjects.end()) {
        m_agxConnectionGraphicsObjects.erase(it);
    }

    if (m_agxDraftConnection && m_agxDraftConnection->connectionId() == connectionId) {
        m_agxDraftConnection.reset();
    }

    updateAttachedNodes(connectionId, AgxPortType::Out);
    updateAttachedNodes(connectionId, AgxPortType::In);

    Q_EMIT modified(this);
}

void AgxGraphicsScene::onConnectionCreated(const AgxConnectionId& connectionId)
{
    m_agxConnectionGraphicsObjects[connectionId]
        = std::make_unique<AgxConnectionGraphicsObject>(*this, connectionId);

    updateAttachedNodes(connectionId, AgxPortType::Out);
    updateAttachedNodes(connectionId, AgxPortType::In);

    Q_EMIT modified(this);
}

QVector<AgxNodeId> AgxGraphicsScene::selectedNodes() const
{
    QList<QGraphicsItem*> graphicsItems = selectedItems();

    QVector<AgxNodeId> result;
    result.reserve(graphicsItems.size());

    for (QGraphicsItem* item : graphicsItems) {
        if (const auto ngo = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item); ngo != nullptr) {
            result.push_back(ngo->nodeId());
        }
    }
    
    return result;
}

QMenu* AgxGraphicsScene::createSceneMenu(QPointF const scenePos)
{
    auto modelMenu = new QMenu();
    modelMenu->setStyleSheet("QMenu{border-radius: 5px;}");
    modelMenu->setMinimumWidth(300);

    // Add filterbox to the context menu
    auto* txtBox = new QLineEdit(modelMenu);
    txtBox->setStyleSheet("QLineEdit{ border-radius: 5px; margin: 5px 5px 5px 5px; padding-left: 5 px;}");
    txtBox->setPlaceholderText(QStringLiteral("Filter"));
    txtBox->setClearButtonEnabled(true);

    auto* txtBoxAction = new QWidgetAction(modelMenu);
    txtBoxAction->setDefaultWidget(txtBox);

    // 1.
    modelMenu->addAction(txtBoxAction);

    // Add result treeview to the context menu
    auto treeView = new QTreeWidget(modelMenu);
    //treeView->setStyleSheet("QTreeWidget{background-color: transparent; border-radius: 0px; }");
    SetTransparentBackground(treeView);
    treeView->header()->close();

    auto* treeViewAction = new QWidgetAction(modelMenu);
    treeViewAction->setDefaultWidget(treeView);

    // 2.
    modelMenu->addAction(treeViewAction);

    const auto registry = m_agxGraphModel.dataModelRegistry();

    for (auto const& cat : registry->categories()) {
        const auto item = new QTreeWidgetItem(treeView);
        item->setText(0, cat);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    }

    for (const auto& [itemText, searchTerm] : registry->registeredModelsCategoryAssociation()) {
        QList<QTreeWidgetItem*> parent = treeView->findItems(searchTerm, Qt::MatchExactly);

        if (parent.count() <= 0)
            continue;

        const auto item = new QTreeWidgetItem(parent.first());
        item->setText(0, itemText);
    }

    treeView->expandAll();

    connect(treeView,
        &QTreeWidget::itemClicked,
        [this, modelMenu, scenePos](const QTreeWidgetItem* item, int) {
            if (!(item->flags() & Qt::ItemIsSelectable)) {
                return;
            }

            this->undoStack().push(new CreateCommand(this, item->text(0), scenePos));

            modelMenu->close();
        });

    //Setup filtering
    connect(txtBox, &QLineEdit::textChanged, [treeView](const QString& text) {
        QTreeWidgetItemIterator categoryIt(treeView, QTreeWidgetItemIterator::HasChildren);
        while (*categoryIt)
            (*categoryIt++)->setHidden(true);
        QTreeWidgetItemIterator it(treeView, QTreeWidgetItemIterator::NoChildren);
        while (*it) {
            auto modelName = (*it)->text(0);
            const bool match = modelName.contains(text, Qt::CaseInsensitive);
            (*it)->setHidden(!match);
            if (match) {
                QTreeWidgetItem* parent = (*it)->parent();
                while (parent) {
                    parent->setHidden(false);
                    parent = parent->parent();
                }
            }
            ++it;
        }
        });
    treeView->sortByColumn(0, Qt::AscendingOrder);

    // make sure the text box gets focus so the user doesn't have to click on it
    txtBox->setFocus();

    // QMenu's instance auto-destruction
    modelMenu->setAttribute(Qt::WA_DeleteOnClose);

    return modelMenu;
}

void AgxGraphicsScene::traverseGraphAndPopulateGraphicsObjects()
{
    auto allNodeIds = m_agxGraphModel.allNodeIds();

    // First create all the nodes.
    for (AgxNodeId const nodeId : allNodeIds)
    {
        m_agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxNodeGraphicsObject>(*this, nodeId);
        connect(m_agxNodeGraphicsObjects[nodeId].get(), &AgxNodeGraphicsObject::Deselected, [this, nodeId]
        {
            Q_EMIT nodeGODeselected(nodeId);
        });
        connect(m_agxNodeGraphicsObjects[nodeId].get(), &AgxNodeGraphicsObject::Selected, [this, nodeId]
        {
            Q_EMIT nodeGOSelected(nodeId);
        });

    }

    // Then for each node check output connections and insert them.
    for (AgxNodeId const nodeId : allNodeIds)
    {
        const auto nOutPorts = m_agxGraphModel.nodeData(nodeId, AgxNodeRole::OutPortCount).value<AgxPortCount>();

        for (AgxPortIndex index = 0; index < nOutPorts; ++index)
        {
            for (auto const& outConnectionIds = m_agxGraphModel.connections(nodeId, AgxPortType::Out, index); auto& cid : outConnectionIds)
            {
                m_agxConnectionGraphicsObjects[cid] = std::make_unique<AgxConnectionGraphicsObject>(*this,cid);
            }
        }
    }
}

void AgxGraphicsScene::updateAttachedNodes(const AgxConnectionId& connectionId, const AgxPortType& portType)
{
    if (const auto node = agxNodeGraphicsObject(getNodeId(portType, connectionId))) {
        node->update();
    }
}

void AgxGraphicsScene::onNodeDeleted(const AgxNodeId& nodeId)
{
    if (const auto it = m_agxNodeGraphicsObjects.find(nodeId); it != m_agxNodeGraphicsObjects.end()) {
        m_agxNodeGraphicsObjects.erase(it);
        Q_EMIT modified(this);
    }
}

void AgxGraphicsScene::onNodeCreated(const AgxNodeId& nodeId)
{
    if (agxGraphModel().GetNodeType(nodeId) == AgxNodeType::Comment) {

        m_agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxCommentGraphicsObject>(*this, nodeId);

        QTimer::singleShot(5, this, [this, nodeId] {

        const auto pos = agxGraphModel().nodeData(nodeId, AgxNodeRole::Position).toPointF();

        //pos -= QPointF(10, 10);

        if (auto checkData = agxGraphModel().nodeData(nodeId, AgxNodeRole::InternalData).toJsonObject();
            checkData.contains("internal-data") &&
            checkData["internal-data"].toObject().contains("comment-target")
            )
        {
            return;
        }

        QJsonObject data;
        QJsonObject tgtData;
        tgtData["x"] = pos.x();
        tgtData["y"] = pos.y();
        data["comment-target"] = tgtData;
        agxGraphModel().setNodeData(nodeId, AgxNodeRole::InternalData, data);
        Q_EMIT agxGraphModel().nodeUpdated(nodeId);
                           });

    } else {
        m_agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxNodeGraphicsObject>(*this, nodeId);
        connect(m_agxNodeGraphicsObjects[nodeId].get(), &AgxNodeGraphicsObject::Deselected, [this, nodeId]
        {
            Q_EMIT nodeGODeselected(nodeId);
        });
        connect(m_agxNodeGraphicsObjects[nodeId].get(), &AgxNodeGraphicsObject::Selected, [this, nodeId]
        {
            Q_EMIT nodeGOSelected(nodeId);
        });
    }

    Q_EMIT modified(this);
}

void AgxGraphicsScene::onNodePositionUpdated(const AgxNodeId& nodeId)
{
    if (const auto node = agxNodeGraphicsObject(nodeId))
    {
        node->setPos(m_agxGraphModel.nodeData(nodeId, AgxNodeRole::Position).value<QPointF>());
        node->update();
        m_nodeDrag = true;
    }
}

void AgxGraphicsScene::onNodeUpdated(const AgxNodeId& nodeId)
{
    if (const auto node = agxNodeGraphicsObject(nodeId))
    {
        node->setGeometryChanged();

        m_agxNodeGeometry->recomputeSize(nodeId);

        node->updateQWidgetEmbedPos();
        node->update();
        node->moveConnections();
    }
}

void AgxGraphicsScene::onNodeClicked(const AgxNodeId& nodeId)
{
    if (m_nodeDrag) {
        Q_EMIT nodeMoved(nodeId, m_agxGraphModel.nodeData(nodeId, AgxNodeRole::Position).value<QPointF>());
        Q_EMIT modified(this);
    }
    m_nodeDrag = false;
}

void AgxGraphicsScene::onModelReset()
{
    m_agxConnectionGraphicsObjects.clear();
    m_agxNodeGraphicsObjects.clear();

    clear();

    traverseGraphAndPopulateGraphicsObjects();
}

void AgxGraphicsScene::onSelectAllObjectsOfType(const AgxGraphicsItemsFlags flags)
{
    if (flags.testFlag(AgxGraphicsItemsFlag::Node))
        onSelectNodes(agxGraphModel().allNodeIds().values());

    
    if(flags.testFlag(AgxGraphicsItemsFlag::Connection))
    {
        for (auto& graphics_object : m_agxConnectionGraphicsObjects | std::views::values)
        {
            graphics_object.get()->setSelected(true);
        }
    }
}

void AgxGraphicsScene::onSelectedConnections(const QList<AgxConnectionId>& connsToSelect)
{
    for (auto& cidRef : connsToSelect)
    {
        agxConnectionGraphicsObject(cidRef)->setSelected(true);
    }

    QCoreApplication::processEvents();
}

void AgxGraphicsScene::onSelectAnyAndAllObjects()
{ onSelectAllObjectsOfType(AgxGraphicsItemsFlag::All); }

void AgxGraphicsScene::onSelectAllNodes()
{ onSelectAllObjectsOfType(AgxGraphicsItemsFlag::Node); }

void AgxGraphicsScene::onSelectAllConnections()
{ onSelectAllObjectsOfType(AgxGraphicsItemsFlag::Connection); }

void AgxGraphicsScene::onSelectNodes(const QList<AgxNodeId>& nodesToSelect)
{
    for (auto& node : nodesToSelect)
    {
        agxNodeGraphicsObject(node)->setSelected(true);
    }
}

QString AgxGraphicsScene::getLastHoveredGroup() const
{
    if (m_lastHoveredNode)
        return m_agxGraphModel.GetNodeGroup(m_lastHoveredNode->nodeId());

    return "";
}

bool AgxGraphicsScene::setGroupHoverState(const bool shouldHover, const QString& groupId)
{
    if (shouldHover && agxGraphModel().GroupExists(groupId))
    {
        for (auto list = agxGraphModel().GetNodeGroupAssignmentList(); const auto node : list.at(groupId))
        {
            agxNodeGraphicsObject(node)->nodeState().setHovered(true);
            agxNodeGraphicsObject(node)->setZValue(1.0);
            agxNodeGraphicsObject(node)->update();
        }
        return true;
    }

    for (const auto node : agxGraphModel().allNodeIds())
    {
        if(!m_lastHoveredNode || node != m_lastHoveredNode->nodeId())
        {
            agxNodeGraphicsObject(node)->nodeState().setHovered(false);
            agxNodeGraphicsObject(node)->setZValue(0.0);
            agxNodeGraphicsObject(node)->update();
        }
    }
    return false;
}

void AgxGraphicsScene::setConnectionsHidden(const QList<AgxConnectionId>& cidList, const bool hide)
{
    for (auto& cid : cidList)
    {
        if (const auto cgo = agxConnectionGraphicsObject(cid))
        {
            cgo->setConnectionHidden(hide);
        }
    }
}

size_t AgxGraphicsScene::nodeGraphicItemCount() const
{
    return m_agxNodeGraphicsObjects.size();
}

size_t AgxGraphicsScene::connectionGraphicItemCount() const
{
    return m_agxConnectionGraphicsObjects.size();
}

bool AgxGraphicsScene::isNodeSelected(const AgxNodeId nodeId) const
{
    return m_agxNodeGraphicsObjects.at(nodeId)->isSelected();
}
