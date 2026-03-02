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

AgxGraphicsScene::AgxGraphicsScene(AgxGraphModel& graphModel, const QString& graphTitle, QObject* parent) : QGraphicsScene(parent)
, _agxGraphModel(graphModel)
, _agxNodeGeometry(std::make_unique<AgxNodeGeometry>(_agxGraphModel))
, _agxNodePainter(std::make_unique<AgxNodePainter>())
, _agxConnectionPainter(std::make_unique<AgxConnectionPainter>())
, _nodeDrag(false)
, _undoStack(new QUndoStack(this))
, _orientation(Qt::Horizontal)
{
    if (!graphTitle.isEmpty())
        _agxGraphTitle = graphTitle;

    setItemIndexMethod(QGraphicsScene::NoIndex);

    connect(&_agxGraphModel,
        &AgxGraphModel::connectionCreated,
        this,
        &AgxGraphicsScene::onConnectionCreated);

    connect(&_agxGraphModel,
        &AgxGraphModel::connectionDeleted,
        this,
        &AgxGraphicsScene::onConnectionDeleted);

    connect(&_agxGraphModel,
        &AgxGraphModel::nodeCreated,
        this,
        &AgxGraphicsScene::onNodeCreated);

    connect(&_agxGraphModel,
        &AgxGraphModel::nodeDeleted,
        this,
        &AgxGraphicsScene::onNodeDeleted);

    connect(&_agxGraphModel,
        &AgxGraphModel::nodePositionUpdated,
        this,
        &AgxGraphicsScene::onNodePositionUpdated);

    connect(&_agxGraphModel,
        &AgxGraphModel::nodeUpdated,
        this,
        &AgxGraphicsScene::onNodeUpdated);

    connect(this, &AgxGraphicsScene::nodeClicked, this, &AgxGraphicsScene::onNodeClicked);

    connect(&_agxGraphModel, &AgxGraphModel::modelReset, this, &AgxGraphicsScene::onModelReset);

    connect(&graphModel,
        &AgxGraphModel::inPortDataWasSet,
        [this](AgxNodeId const nodeId, AgxPortType const, AgxPortIndex const) { onNodeUpdated(nodeId); });

    traverseGraphAndPopulateGraphicsObjects();

}

AgxGraphModel const& AgxGraphicsScene::agxGraphModel() const { return _agxGraphModel; }

AgxGraphModel& AgxGraphicsScene::agxGraphModel() { return _agxGraphModel; }

AgxNodeGeometry const& AgxGraphicsScene::agxNodeGeometry() const
{
    return *_agxNodeGeometry;
}

AgxNodeGeometry& AgxGraphicsScene::agxNodeGeometry()
{
    return *_agxNodeGeometry;
}

std::unique_ptr<AgxConnectionGraphicsObject> const& AgxGraphicsScene::makeDraftConnection(AgxConnectionId const newConnectionId)
{
    _agxDraftConnection = std::make_unique<AgxConnectionGraphicsObject>(*this, newConnectionId);

    _agxDraftConnection->grabMouse();

    return _agxDraftConnection;
}

AgxConnectionGraphicsObject* AgxGraphicsScene::agxConnectionGraphicsObject(AgxConnectionId connectionId)
{
    AgxConnectionGraphicsObject* cgo = nullptr;
    auto it = _agxConnectionGraphicsObjects.find(connectionId);
    if (it != _agxConnectionGraphicsObjects.end()) {
        cgo = it->second.get();
    }
    if (cgo)
        return cgo;

    return nullptr;
}

AgxNodeGraphicsObject* AgxGraphicsScene::agxNodeGraphicsObject(AgxNodeId nodeId)
{
    AgxNodeGraphicsObject* ngo = nullptr;
    auto it = _agxNodeGraphicsObjects.find(nodeId);
    if (it != _agxNodeGraphicsObjects.end()) {
        ngo = it->second.get();
    }

    return ngo;
}

void AgxGraphicsScene::resetDraftConnection()
{
    _agxDraftConnection.reset();
}

void AgxGraphicsScene::clearScene()
{
    auto const& allNodeIds = agxGraphModel().allNodeIds();

    for (auto nodeId : allNodeIds) {
        agxGraphModel().deleteNode(nodeId);
    }
}

AgxNodePainter& AgxGraphicsScene::agxNodePainter()
{
    return *_agxNodePainter;
}

AgxConnectionPainter& AgxGraphicsScene::agxConnectionPainter()
{
    return *_agxConnectionPainter;
}

void AgxGraphicsScene::setNodePainter(std::unique_ptr<AgxNodePainter> newPainter)
{
    _agxNodePainter = std::move(newPainter);
}

void AgxGraphicsScene::setConnectionPainter(std::unique_ptr<AgxConnectionPainter> newPainter)
{
    _agxConnectionPainter = std::move(newPainter);
}

void AgxGraphicsScene::setNodeGeometry(std::unique_ptr<AgxNodeGeometry> newGeom)
{
    _agxNodeGeometry = std::move(newGeom);
}

QUndoStack& AgxGraphicsScene::undoStack()
{
    return *_undoStack;
}

void AgxGraphicsScene::setOrientation(Qt::Orientation const orientation)
{
    if (_orientation != orientation) {
        _orientation = orientation;

        switch (_orientation) {
        case Qt::Horizontal:
            _agxNodeGeometry = std::make_unique<AgxNodeGeometry>(_agxGraphModel);
            break;

        case Qt::Vertical:
            _agxNodeGeometry = std::make_unique<AgxNodeGeometry>(_agxGraphModel);
            break;
        }

        onModelReset();
    }
}

void AgxGraphicsScene::onConnectionDeleted(AgxConnectionId const connectionId)
{
    auto it = _agxConnectionGraphicsObjects.find(connectionId);
    if (it != _agxConnectionGraphicsObjects.end()) {
        _agxConnectionGraphicsObjects.erase(it);
    }

    // TODO: do we need it?
    if (_agxDraftConnection && _agxDraftConnection->connectionId() == connectionId) {
        _agxDraftConnection.reset();
    }

    updateAttachedNodes(connectionId, AgxPortType::Out);
    updateAttachedNodes(connectionId, AgxPortType::In);

    Q_EMIT modified(this);
}

void AgxGraphicsScene::onConnectionCreated(AgxConnectionId const connectionId)
{
    _agxConnectionGraphicsObjects[connectionId]
        = std::make_unique<AgxConnectionGraphicsObject>(*this, connectionId);

    updateAttachedNodes(connectionId, AgxPortType::Out);
    updateAttachedNodes(connectionId, AgxPortType::In);

    Q_EMIT modified(this);
}

std::vector<AgxNodeId> AgxGraphicsScene::selectedNodes() const
{
    QList<QGraphicsItem*> graphicsItems = selectedItems();

    std::vector<AgxNodeId> result;
    result.reserve(graphicsItems.size());

    for (QGraphicsItem* item : graphicsItems) {
        auto ngo = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item);

        if (ngo != nullptr) {
            result.push_back(ngo->nodeId());
        }
    }
    
    return result;
}

QMenu* AgxGraphicsScene::createSceneMenu(QPointF const scenePos)
{
    QMenu* modelMenu = new QMenu();
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
    QTreeWidget* treeView = new QTreeWidget(modelMenu);
    treeView->setStyleSheet("QTreeWidget{background-color: transparent; border-radius: 0px; }");
    treeView->header()->close();

    auto* treeViewAction = new QWidgetAction(modelMenu);
    treeViewAction->setDefaultWidget(treeView);

    // 2.
    modelMenu->addAction(treeViewAction);

    auto registry = _agxGraphModel.dataModelRegistry();

    for (auto const& cat : registry->categories()) {
        auto item = new QTreeWidgetItem(treeView);
        item->setText(0, cat);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    }

    for (auto const& assoc : registry->registeredModelsCategoryAssociation()) {
        QList<QTreeWidgetItem*> parent = treeView->findItems(assoc.second, Qt::MatchExactly);

        if (parent.count() <= 0)
            continue;

        auto item = new QTreeWidgetItem(parent.first());
        item->setText(0, assoc.first);
    }

    treeView->expandAll();

    connect(treeView,
        &QTreeWidget::itemClicked,
        [this, modelMenu, scenePos](QTreeWidgetItem* item, int) {
            if (!(item->flags() & (Qt::ItemIsSelectable))) {
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
            const bool match = (modelName.contains(text, Qt::CaseInsensitive));
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
    auto allNodeIds = _agxGraphModel.allNodeIds();

    // First create all the nodes.
    for (AgxNodeId const nodeId : allNodeIds) {
        _agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxNodeGraphicsObject>(*this, nodeId);
    }

    // Then for each node check output connections and insert them.
    for (AgxNodeId const nodeId : allNodeIds) {
        auto nOutPorts = _agxGraphModel.nodeData(nodeId, AgxNodeRole::OutPortCount).value<AgxPortCount>();

        for (AgxPortIndex index = 0; index < nOutPorts; ++index) {
            auto const& outConnectionIds = _agxGraphModel.connections(nodeId, AgxPortType::Out, index);

            for (auto& cid : outConnectionIds) {
                _agxConnectionGraphicsObjects[cid] = std::make_unique<AgxConnectionGraphicsObject>(*this,cid);
            }
        }
    }
}

void AgxGraphicsScene::updateAttachedNodes(AgxConnectionId const connectionId, AgxPortType const portType)
{
    auto node = agxNodeGraphicsObject(getNodeId(portType, connectionId));

    if (node) {
        node->update();
    }
}

void AgxGraphicsScene::onNodeDeleted(AgxNodeId const nodeId)
{
    auto it = _agxNodeGraphicsObjects.find(nodeId);
    if (it != _agxNodeGraphicsObjects.end()) {
        _agxNodeGraphicsObjects.erase(it);

        Q_EMIT modified(this);
    }
}

void AgxGraphicsScene::onNodeCreated(AgxNodeId const nodeId)
{
    if (agxGraphModel().GetNodeType(nodeId) == AgxNodeType::Comment) {

        _agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxCommentGraphicsObject>(*this, nodeId);

        QTimer::singleShot(5, this, [this, nodeId]() {
                           
        auto pos = agxGraphModel().nodeData(nodeId, AgxNodeRole::Position).toPointF();

        //pos -= QPointF(10, 10);

        auto checkData = agxGraphModel().nodeData(nodeId, AgxNodeRole::InternalData).toJsonObject();

        if (checkData.contains("internal-data") && checkData["internal-data"].toObject().contains("comment-target")) return;

        QJsonObject data;
        QJsonObject tgtData;
        tgtData["x"] = pos.x();
        tgtData["y"] = pos.y();
        data["comment-target"] = tgtData;
        agxGraphModel().setNodeData(nodeId, AgxNodeRole::InternalData, data);
        Q_EMIT agxGraphModel().nodeUpdated(nodeId);
                           });

    } else {
        _agxNodeGraphicsObjects[nodeId] = std::make_unique<AgxNodeGraphicsObject>(*this, nodeId);
    }

    connect(this, &QGraphicsScene::selectionChanged, &agxGraphModel(), [this, nodeId] {
        if (agxGraphModel().nodeExists(nodeId))
            //QTimer::singleShot(0, this, [this, nodeId]() {
                agxGraphModel().SetNodeSidebarVisibility(nodeId, _agxNodeGraphicsObjects[nodeId]->isSelected());
                               //});
            });
    //Waiting a frame so that the command actually sets the state correctly
    QTimer::singleShot(0, this, [this, nodeId] {
                agxGraphModel().SetNodeSidebarVisibility(nodeId, _agxNodeGraphicsObjects[nodeId]->isSelected());
                               });
    Q_EMIT modified(this);
}

void AgxGraphicsScene::onNodePositionUpdated(AgxNodeId const nodeId)
{
    auto node = agxNodeGraphicsObject(nodeId);
    if (node) {
        node->setPos(_agxGraphModel.nodeData(nodeId, AgxNodeRole::Position).value<QPointF>());
        node->update();
        _nodeDrag = true;
    }
}

void AgxGraphicsScene::onNodeUpdated(AgxNodeId const nodeId)
{
    auto node = agxNodeGraphicsObject(nodeId);

    if (node) {
        node->setGeometryChanged();

        _agxNodeGeometry->recomputeSize(nodeId);

        node->updateQWidgetEmbedPos();
        node->update();
        node->moveConnections();


    }
}

void AgxGraphicsScene::onNodeClicked(AgxNodeId const nodeId)
{
    if (_nodeDrag) {
        Q_EMIT nodeMoved(nodeId, _agxGraphModel.nodeData(nodeId, AgxNodeRole::Position).value<QPointF>());
        Q_EMIT modified(this);
    }
    _nodeDrag = false;
}

void AgxGraphicsScene::onModelReset()
{
    _agxConnectionGraphicsObjects.clear();
    _agxNodeGraphicsObjects.clear();

    clear();

    traverseGraphAndPopulateGraphicsObjects();
}

void AgxGraphicsScene::onSelectAllObjectsOfType(AgxGraphicsItemsFlags flags)
{
    if (flags.testFlag(AgxGraphicsItemsFlag::Node))
        onSelectNodes(agxGraphModel().allNodeIds().values());

    
    if(flags.testFlag(AgxGraphicsItemsFlag::Connection))
    {
        blockSignals(true);
        for (auto& conn : _agxConnectionGraphicsObjects)
        {
            conn.second.get()->setSelected(true);
        }
        blockSignals(false);
    }
}

void AgxGraphicsScene::onSelectedConnections(const QList<AgxConnectionId>& connsToSelect)
{
    blockSignals(true);

    for (auto& cidRef : connsToSelect)
    {
        agxConnectionGraphicsObject(cidRef)->blockSignals(true);
        agxConnectionGraphicsObject(cidRef)->setSelected(true);
        agxConnectionGraphicsObject(cidRef)->blockSignals(false);
    }

    blockSignals(false);

    QCoreApplication::processEvents();
}

void AgxGraphicsScene::onRightRefreshSideBarVisibility()
{
    QList<AgxNodeId> nodeIds = agxGraphModel().allNodeIds().values();

    QTimer::singleShot(30, this, [this, nodeIds]() {
                                                        size_t i = 0;
                                                        for (auto nodeId = nodeIds.rbegin(); nodeId != nodeIds.rend(); ++nodeId) {
                                                            auto ngo = agxNodeGraphicsObject(*nodeId);
                                                            ngo->blockSignals(true);
                                                            agxGraphModel().SetNodeSidebarVisibility(*nodeId, ngo->isSelected());
                                                            ngo->blockSignals(false);
                                                            i++;
                                                            if (i % 5 == 0) QCoreApplication::processEvents();
                                                        }
                                                   });

}

void AgxGraphicsScene::onSelectNodes(const QList<AgxNodeId>& nodesToSelect)
{
    blockSignals(true);
    
    for (auto& node : nodesToSelect)
    {
        agxNodeGraphicsObject(node)->blockSignals(true);
        agxNodeGraphicsObject(node)->setSelected(true);
        //node.second.get()->setSelected(true); //not needed as the visibility refresh helper function already does this
    }

    //We manually set sidebar visibility as doing so via QT "selection" signal events is expensive and will lag the view
    //In fact, even this method is slightly laggy hence the concurrant QTimer shot
    onRightRefreshSideBarVisibility();

    blockSignals(false);
}

QString AgxGraphicsScene::getLastHoveredGroup()
{
    if (lastHoveredNode)
        return _agxGraphModel.GetNodeGroup(lastHoveredNode->nodeId());
    else
        return "";
}

bool AgxGraphicsScene::setGroupHoverState(bool shouldHover, QString groupId)
{
    if (shouldHover && agxGraphModel().GroupExists(groupId))
    {
        auto list = agxGraphModel().GetNodeGroupAssignmentList();
        for (auto node : list.at(groupId))
        {
            agxNodeGraphicsObject(node)->nodeState().setHovered(true);
            agxNodeGraphicsObject(node)->setZValue(1.0);
            agxNodeGraphicsObject(node)->update();
        }
        return true;
    }

    for (auto node : agxGraphModel().allNodeIds())
    {
        if(!lastHoveredNode || node != lastHoveredNode->nodeId())
        {
            agxNodeGraphicsObject(node)->nodeState().setHovered(false);
            agxNodeGraphicsObject(node)->setZValue(0.0);
            agxNodeGraphicsObject(node)->update();
        }
    }
    return false;
}

void AgxGraphicsScene::setConnectionsHidden(const QList<AgxConnectionId>& cids, bool hide)
{
    for (auto& cid : cids)
    {
        if (auto cgo = agxConnectionGraphicsObject(cid))
        {
            cgo->setConnectionHidden(hide);
        }
    }
}

size_t AgxGraphicsScene::nodeGraphicItemCount() const
{
    return _agxNodeGraphicsObjects.size();
}

size_t AgxGraphicsScene::connectionGraphicItemCount() const
{
    return _agxConnectionGraphicsObjects.size();
}
