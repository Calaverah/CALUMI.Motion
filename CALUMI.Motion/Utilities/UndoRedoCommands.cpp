#include "stdafx.h"
#include "UndoRedoCommands.h"

#pragma warning(push,0)

#include "Utilities/AgxConnectionIdUtils.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QMimeData>
#include <QClipboard>
#include <QApplication>
#include <QGraphicsObject>
#pragma warning(pop)
#include "Utilities/AgxJsonHelper.h"
#include "Models/Blend/AgxBlendInputModel.h"
#include <Widgets/Dialog/AgxProgressDialog.h>
#include "Widgets/AgxGraphicsView.h"

#pragma region MyRegion
static QJsonObject serializeSelectedItems(AgxGraphicsScene* scene)
{
    QJsonObject serializedScene;

    auto& graphModel = scene->agxGraphModel();

    std::unordered_set<AgxNodeId> selectedNodes;

    QJsonArray nodesJsonArray;

    for (QGraphicsItem* item : scene->selectedItems()) {
        if (auto n = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item)) {
            nodesJsonArray.append(graphModel.saveNode(n->nodeId()));

            selectedNodes.insert(n->nodeId());
        }
    }

    QJsonArray connJsonArray;

    for (QGraphicsItem* item : scene->selectedItems()) {
        if (auto c = qgraphicsitem_cast<AgxConnectionGraphicsObject*>(item)) {
            auto const& cid = c->connectionId();

            if (selectedNodes.count(cid.outNodeId) > 0 && selectedNodes.count(cid.inNodeId) > 0) {
                connJsonArray.append(toJson(cid));
            }
        }
    }

    QJsonArray groupJsonArray;
    std::unordered_set<QString> processedGroups;

    for (const auto& entry : selectedNodes)
    {
        QString groupId = scene->agxGraphModel().GetNodeGroup(entry);
        QColor groupColor = scene->agxGraphModel().GetGroupColor(groupId);
        if (processedGroups.count(groupId) <= 0)
        {
            QJsonObject groupObj;
            groupObj.insert("groupId", groupId);
            groupObj.insert("groupColor", colorToJsonObject(scene->agxGraphModel().GetGroupColor(groupId)));
            groupJsonArray.append(groupObj);
            processedGroups.insert(groupId);
        }
    }

    serializedScene["nodes"] = nodesJsonArray;
    serializedScene["groups"] = groupJsonArray;
    serializedScene["connections"] = connJsonArray;

    return serializedScene;
}

static void insertSerializedItems(QJsonObject const& json, AgxGraphicsScene* scene)
{
    AgxGraphicsView* agxView = nullptr;
    for (auto view : scene->views())
    {
        if (agxView = dynamic_cast<AgxGraphicsView*>(view)) break;
    }

    AgxProgressDialog* progBar = new AgxProgressDialog("Inserting Items...", "", 0, 1000, agxView);
    QFutureWatcher<void>* watcher = new QFutureWatcher<void>(agxView);
    QObject::connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
    QObject::connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
    QObject::connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
    QObject::connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
    
    progBar->show();

    Q_EMIT watcher->progressValueChanged(static_cast<int>(0 * 1000));

    AgxGraphModel& graphModel = scene->agxGraphModel();

    QJsonArray const& nodesJsonArray = json["nodes"].toArray();
    
    size_t totalItems = nodesJsonArray.size();
    size_t count = 0;

    QSet<AgxNodeId> nodesToSelect;

    for (QJsonValue node : nodesJsonArray) {
        Q_EMIT watcher->progressTextChanged(std::format("Loading Node {} out of {}", count, totalItems).c_str());
        QJsonObject obj = node.toObject();

        graphModel.loadNode(obj);

        auto id = obj["id"].toVariant().toUInt();
        scene->agxNodeGraphicsObject(id)->setZValue(1.0);
        //scene->agxNodeGraphicsObject(id)->setSelected(true);
        nodesToSelect.insert(id);
        count++;
        Q_EMIT watcher->progressValueChanged(static_cast<int>(static_cast<float>(count)/totalItems * 900));
    }

    scene->onSelectNodes(nodesToSelect.values());

    Q_EMIT watcher->progressTextChanged(std::format("Loading Connections", count, totalItems).c_str());

    QJsonArray const& connJsonArray = json["connections"].toArray();

    for (QJsonValue connection : connJsonArray) {
        
        QJsonObject connJson = connection.toObject();

        AgxConnectionId connId = fromJson(connJson);

        // Restore the connection
        graphModel.addConnection(connId);
        scene->agxConnectionGraphicsObject(connId)->blockSignals(true);
        scene->agxConnectionGraphicsObject(connId)->setSelected(true);
        scene->agxConnectionGraphicsObject(connId)->blockSignals(false);
        count++;
        
        QCoreApplication::processEvents();
    }

    Q_EMIT watcher->progressValueChanged(static_cast<int>(1000));
}

static std::set<QString> intertSerializedItemGroups(QJsonObject const& json, AgxGraphModel& graphModel)
{   
    std::set<QString> addedGroups;

    QJsonArray const& groupJsonArray = json["groups"].toArray();

    for (QJsonValue group : groupJsonArray)
    {
        QJsonObject obj = group.toObject();
        QString groupId = obj.value("groupId").toString();
        if (!graphModel.GroupExists(groupId))
        {
            graphModel.CreateNodeGroup(groupId, JsonObjectToColor(obj.value("groupColor").toObject()));
            addedGroups.insert(groupId);
        }
    }
    return addedGroups;
}

static void deleteSerializedItems(QJsonObject& sceneJson, AgxGraphModel& graphModel)
{
    QJsonArray connectionJsonArray = sceneJson["connections"].toArray();

    for (QJsonValueRef connection : connectionJsonArray) {
        QJsonObject connJson = connection.toObject();

        AgxConnectionId connId = fromJson(connJson);

        graphModel.deleteConnection(connId);
    }

    QJsonArray nodesJsonArray = sceneJson["nodes"].toArray();

    for (QJsonValueRef node : nodesJsonArray) {
        QJsonObject nodeJson = node.toObject();
        graphModel.deleteNode(nodeJson["id"].toVariant().toUInt());
    }
}

static void deleteSerializedItemGroups(std::set<QString>& groups, AgxGraphModel& model)
{
    for (auto& group : groups)
    {
        model.EraseNodeGroup(group);
    }
}

static QPointF computeAverageNodePosition(QJsonObject const& sceneJson)
{
    QPointF averagePos(0, 0);

    QJsonArray nodesJsonArray = sceneJson["nodes"].toArray();

    for (QJsonValueRef node : nodesJsonArray) {
        QJsonObject nodeJson = node.toObject();

        averagePos += QPointF(nodeJson["position"].toObject()["x"].toDouble(),
            nodeJson["position"].toObject()["y"].toDouble());
    }

    averagePos /= static_cast<double>(nodesJsonArray.size());

    return averagePos;
}

//-------------------------------------

CreateCommand::CreateCommand(AgxGraphicsScene* scene,
    QString const name,
    QPointF const& mouseScenePos)
    : _scene(scene)
    , _sceneJson(QJsonObject())
{
    _nodeId = _scene->agxGraphModel().addNode(name);
    if (_nodeId != InvalidNodeId) {
        _scene->agxGraphModel().setNodeData(_nodeId, AgxNodeRole::Position, mouseScenePos);
    }
    else {
        setObsolete(true);
    }
    QString text = "Create " + name;
    setText(text);
}

void CreateCommand::undo()
{
    QJsonArray nodesJsonArray;
    nodesJsonArray.append(_scene->agxGraphModel().saveNode(_nodeId));
    _sceneJson["nodes"] = nodesJsonArray;

    _scene->agxGraphModel().deleteNode(_nodeId);
}

void CreateCommand::redo()
{
    if (_sceneJson.empty() || _sceneJson["nodes"].toArray().empty())
        return;

    insertSerializedItems(_sceneJson, _scene);
}

//-------------------------------------

DeleteCommand::DeleteCommand(AgxGraphicsScene* scene, const QString& overrideString)
    : _scene(scene)
{
    auto& graphModel = _scene->agxGraphModel();
    
    enum ItemType {Unknown, Node, Connection};
    ItemType selectedItemType = Unknown;

    QJsonArray connJsonArray;
    // Delete the selected connections first, ensuring that they won't be
    // automatically deleted when selected nodes are deleted (deleting a
    // node deletes some connections as well)
    for (QGraphicsItem* item : _scene->selectedItems()) {
        if (auto c = qgraphicsitem_cast<AgxConnectionGraphicsObject*>(item)) {
            auto const& cid = c->connectionId();

            connJsonArray.append(toJson(cid));
            _conns.append(cid);
            selectedItemType = Connection;
        }
    }

    QJsonArray nodesJsonArray;
    // Delete the nodes; this will delete many of the connections.
    // Selected connections were already deleted prior to this loop,
    for (QGraphicsItem* item : _scene->selectedItems()) {
        if (auto n = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item)) {
            // saving connections attached to the selected nodes
            for (auto const& cid : graphModel.allConnectionIds(n->nodeId())) {
                connJsonArray.append(toJson(cid));
            }

            nodesJsonArray.append(graphModel.saveNode(n->nodeId()));
            _nodes.append(n->nodeId());
            selectedItemType = Node;
        }
    }

    // If nothing is deleted, cancel this operation
    if (connJsonArray.isEmpty() && nodesJsonArray.isEmpty())
        setObsolete(true);

    _sceneJson["nodes"] = nodesJsonArray;
    _sceneJson["connections"] = connJsonArray;
    QString text = "Unknown Item";
    if (_scene->selectedItems().count() > 1) {
        text = "Selected Items";
    } else if(_scene->selectedItems().count() == 1) {
        switch (selectedItemType)
        {
        case Node: text = graphModel.GetNodeNameProperty(qgraphicsitem_cast<AgxNodeGraphicsObject*>(_scene->selectedItems().at(0))->nodeId());
            break;
        case Connection: text = graphModel.GetNodeNameProperty(qgraphicsitem_cast<AgxConnectionGraphicsObject*>(_scene->selectedItems().at(0))->connectionId().outNodeId) + "'s Connection To " + graphModel.GetNodeNameProperty(qgraphicsitem_cast<AgxConnectionGraphicsObject*>(_scene->selectedItems().at(0))->connectionId().inNodeId);
            break;
        }
    }
    setText(overrideString + " " + text);
}

void DeleteCommand::undo()
{
    insertSerializedItems(_sceneJson, _scene);
}

void DeleteCommand::redo()
{
    //deleteSerializedItems(_sceneJson, _scene->agxGraphModel()); //disabling QtNodes method since it has to json parse and I'd rather bloat up memory than the cpu tbh
    for (auto& connId : _conns) {
        _scene->agxGraphModel().deleteConnection(connId);
    }

    for (auto& nodeId : _nodes) {
        _scene->agxGraphModel().deleteNode(nodeId);
    }
}

//-------------------------------------

static void offsetNodeGroup(QJsonObject& sceneJson, QPointF const& diff)
{
    QJsonArray nodesJsonArray = sceneJson["nodes"].toArray();

    QJsonArray newNodesJsonArray;
    for (QJsonValueRef node : nodesJsonArray) {
        QJsonObject obj = node.toObject();

        QPointF oldPos(obj["position"].toObject()["x"].toDouble(),
            obj["position"].toObject()["y"].toDouble());

        oldPos += diff;

        QJsonObject posJson;
        posJson["x"] = oldPos.x();
        posJson["y"] = oldPos.y();
        obj["position"] = posJson;

        newNodesJsonArray.append(obj);
    }

    sceneJson["nodes"] = newNodesJsonArray;
}

//-------------------------------------

CopyCommand::CopyCommand(AgxGraphicsScene* scene)
{
    QJsonObject sceneJson = serializeSelectedItems(scene);

    if (sceneJson.empty() || sceneJson["nodes"].toArray().empty()) {
        setObsolete(true);
        return;
    }

    QClipboard* clipboard = QApplication::clipboard();

    QByteArray const data = QJsonDocument(sceneJson).toJson();

    QMimeData* mimeData = new QMimeData();
    mimeData->setData("application/qt-nodes-graph", data);
    mimeData->setText(data);

    clipboard->setMimeData(mimeData);

    // Copy command does not have any effective redo/undo operations.
    // It copies the data to the clipboard and could be immediately removed
    // from the stack.
    setObsolete(true);

    setText("Copy");
}

//-------------------------------------

PasteCommand::PasteCommand(AgxGraphicsScene* scene, QPointF const& mouseScenePos)
    : _scene(scene)
    , _mouseScenePos(mouseScenePos)
{
    _newSceneJson = takeSceneJsonFromClipboard();

    if (_newSceneJson.empty() || _newSceneJson["nodes"].toArray().empty()) {
        setObsolete(true);
        return;
    }

    _newSceneJson = makeNewNodeIdsInScene(_newSceneJson);

    QPointF averagePos = computeAverageNodePosition(_newSceneJson);

    offsetNodeGroup(_newSceneJson, _mouseScenePos - averagePos);
    setText("Paste");
}

void PasteCommand::undo()
{
    deleteSerializedItems(_newSceneJson, _scene->agxGraphModel());
    deleteSerializedItemGroups(_groupsAdded, _scene->agxGraphModel());
}

void PasteCommand::redo()
{
    _scene->clearSelection();

    // Ignore if pasted in content does not generate nodes.
    try {
        insertSerializedItems(_newSceneJson, _scene);
        _groupsAdded = intertSerializedItemGroups(_newSceneJson, _scene->agxGraphModel());
    }
    catch (...) {
        // If the paste does not work, delete all selected nodes and connections
        // `deleteNode(...)` implicitly removed connections
        auto& graphModel = _scene->agxGraphModel();

        QJsonArray nodesJsonArray;
        for (QGraphicsItem* item : _scene->selectedItems()) {
            if (auto n = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item)) {
                graphModel.deleteNode(n->nodeId());
            }
        }

        setObsolete(true);
    }
}

QJsonObject PasteCommand::takeSceneJsonFromClipboard()
{
    QClipboard const* clipboard = QApplication::clipboard();
    QMimeData const* mimeData = clipboard->mimeData();

    QJsonDocument json;
    if (mimeData->hasFormat("application/qt-nodes-graph")) {
        json = QJsonDocument::fromJson(mimeData->data("application/qt-nodes-graph"));
    }
    else if (mimeData->hasText()) {
        json = QJsonDocument::fromJson(mimeData->text().toUtf8());
    }

    return json.object();
}

QJsonObject PasteCommand::makeNewNodeIdsInScene(QJsonObject const& sceneJson)
{
    AgxGraphModel& graphModel = _scene->agxGraphModel();

    std::unordered_map<AgxNodeId, AgxNodeId> mapNodeIds;

    QJsonArray nodesJsonArray = sceneJson["nodes"].toArray();

    QJsonArray newNodesJsonArray;
    for (QJsonValueRef node : nodesJsonArray) {
        QJsonObject nodeJson = node.toObject();

        AgxNodeId oldNodeId = nodeJson["id"].toVariant().toUInt();

        AgxNodeId newNodeId = graphModel.newNodeId();

        mapNodeIds[oldNodeId] = newNodeId;

        // Replace NodeId in json
        nodeJson["id"] = static_cast<qint64>(newNodeId);

        newNodesJsonArray.append(nodeJson);
    }

    QJsonArray connectionJsonArray = sceneJson["connections"].toArray();

    QJsonArray newConnJsonArray;
    for (QJsonValueRef connection : connectionJsonArray) {
        QJsonObject connJson = connection.toObject();

        AgxConnectionId connId = fromJson(connJson);

        AgxConnectionId newConnId{ mapNodeIds[connId.outNodeId],
                               connId.outPortIndex,
                               mapNodeIds[connId.inNodeId],
                               connId.inPortIndex };

        newConnJsonArray.append(toJson(newConnId));
    }

    QJsonObject newSceneJson;

    newSceneJson["nodes"] = newNodesJsonArray;
    newSceneJson["connections"] = newConnJsonArray;
    newSceneJson["groups"] = sceneJson["groups"].toArray();

    return newSceneJson;
}

//-------------------------------------

DisconnectCommand::DisconnectCommand(AgxGraphicsScene* scene, AgxConnectionId const connId)
    : _scene(scene)
    , _connId(connId)
{
    //
    setText("Disconnect " + _scene->agxGraphModel().GetNodeNameProperty(_connId.outNodeId) + " From " + _scene->agxGraphModel().GetNodeNameProperty(_connId.inNodeId));
}

void DisconnectCommand::undo()
{
    _scene->agxGraphModel().addConnection(_connId);
}

void DisconnectCommand::redo()
{
    _scene->agxGraphModel().deleteConnection(_connId);
}

//------

ConnectCommand::ConnectCommand(AgxGraphicsScene* scene, AgxConnectionId const connId)
    : _scene(scene)
    , _connId(connId)
{
    //
    setText("Connect " + _scene->agxGraphModel().GetNodeNameProperty(_connId.outNodeId) + " To " + _scene->agxGraphModel().GetNodeNameProperty(_connId.inNodeId));
}

void ConnectCommand::undo()
{
    _scene->agxGraphModel().deleteConnection(_connId);
}

void ConnectCommand::redo()
{
    _scene->agxGraphModel().addConnection(_connId);
}

//------

MoveNodeCommand::MoveNodeCommand(AgxGraphicsScene* scene, QPointF const& diff)
    : _scene(scene)
    , _diff(diff)
{
    _selectedNodes.clear();
    for (QGraphicsItem* item : _scene->selectedItems()) {
        if (auto n = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item)) {
            _selectedNodes.insert(n->nodeId());
        }
    }
    QString text = "Items";
    if (_scene->selectedItems().count() == 1) { auto item = qgraphicsitem_cast<AgxNodeGraphicsObject*>(_scene->selectedItems().at(0)); text = item ? _scene->agxGraphModel().GetNodeNameProperty(item->nodeId()) : ""; }
    setText("Move " + text);
}

void MoveNodeCommand::undo()
{
    for (auto nodeId : _selectedNodes) {
        auto oldPos = _scene->agxGraphModel().nodeData(nodeId, AgxNodeRole::Position).value<QPointF>();

        oldPos -= _diff;

        _scene->agxGraphModel().setNodeData(nodeId, AgxNodeRole::Position, oldPos);
    }
}

void MoveNodeCommand::redo()
{
    for (auto nodeId : _selectedNodes) {
        auto oldPos = _scene->agxGraphModel().nodeData(nodeId, AgxNodeRole::Position).value<QPointF>();

        oldPos += _diff;

        _scene->agxGraphModel().setNodeData(nodeId, AgxNodeRole::Position, oldPos);
    }
}

int MoveNodeCommand::id() const
{
    return static_cast<int>(typeid(MoveNodeCommand).hash_code());
}

bool MoveNodeCommand::mergeWith(QUndoCommand const* c)
{
    auto mc = static_cast<MoveNodeCommand const*>(c);

    if (_selectedNodes == mc->_selectedNodes) {
        _diff += mc->_diff;
        return true;
    }
    return false;
}
#pragma endregion


#pragma region CALUMI_MOTION_COMMANDS
CreateGroupCommand::CreateGroupCommand(AgxGraphicsScene* scene, const QString& groupId) : _scene(scene), _groupId(groupId) { _color = generateRandomQColor(); setText("Create Group: " + _groupId); }

void CreateGroupCommand::undo()
{
    _color = _scene->agxGraphModel().GetGroupColor(_groupId);
    _scene->agxGraphModel().EraseNodeGroup(_groupId);
}
void CreateGroupCommand::redo()
{
    _scene->agxGraphModel().CreateNodeGroup(_groupId, _color);
}

EraseGroupCommand::EraseGroupCommand(AgxGraphicsScene* scene, const QString& groupId) : _scene(scene), _groupId(groupId) { setText("Erase Group: " + _groupId); }

void EraseGroupCommand::undo()
{
    _scene->agxGraphModel().CreateNodeGroup(_groupId, _color);
    for (auto node : _assignedNodes)
    {
        _scene->agxGraphModel().AddToNodeGroup(node, _groupId);
    }
}

void EraseGroupCommand::redo()
{
    _color = _scene->agxGraphModel().GetGroupColor(_groupId);
    _assignedNodes = _scene->agxGraphModel().GetNodeGroupAssignmentList().at(_groupId);

    _scene->agxGraphModel().EraseNodeGroup(_groupId);
}

AddNodeToGroupCommand::AddNodeToGroupCommand(AgxGraphicsScene* scene, const QString& groupId, AgxNodeId nodeId) : _scene(scene), _nextGroupId(groupId), _node(nodeId) { 
    _prevGroupId = _scene->agxGraphModel().GetNodeGroup(_node);
    QString name = _scene->agxGraphModel().GetNodeNameProperty(_node);
    QString text = _prevGroupId == "" ? "Add " + name + " To " + _nextGroupId : "Move " + name + " From " + _prevGroupId + " To " + _nextGroupId;

    setText("Add " + _scene->agxGraphModel().GetNodeNameProperty(_node) + " To " + _nextGroupId); 
}

void AddNodeToGroupCommand::undo()
{
    _scene->agxGraphModel().RemoveFromNodeGroup(_node);
    _scene->agxGraphModel().AddToNodeGroup(_node, _prevGroupId);
}

void AddNodeToGroupCommand::redo()
{
    _scene->agxGraphModel().AddToNodeGroup(_node, _nextGroupId);
}

AddSelectedToGroupCommand::AddSelectedToGroupCommand(AgxGraphicsScene* scene, const QString& groupId, const std::unordered_map<AgxNodeId, QString>& prevAssignments) : _scene(scene), _nextGroupId(groupId), _previousAssignments(prevAssignments)
{
    //QString text = prevAssignments.size() == 1 ? scene->agxGraphModel().GetNodeNameProperty(prevAssignments.begin()->first) : "Items";


    setText("Add Items To " + groupId);
}

void AddSelectedToGroupCommand::undo()
{
    for (auto& entry : _previousAssignments)
    {
        if (entry.second != "" && entry.second != "NONE")
            _scene->agxGraphModel().AddToNodeGroup(entry.first, entry.second);
        else
            _scene->agxGraphModel().RemoveFromNodeGroup(entry.first);
    }
}

void AddSelectedToGroupCommand::redo()
{
    for (auto& entry : _previousAssignments)
    {
        _scene->agxGraphModel().AddToNodeGroup(entry.first, _nextGroupId);
    }
}

RemoveNodeFromGroupCommand::RemoveNodeFromGroupCommand(AgxGraphicsScene* scene, AgxNodeId node) : _scene(scene), _node(node) { 
    _prevGroupId = _scene->agxGraphModel().GetNodeGroup(_node);
    QString groupName = _prevGroupId == "" ? "Group" : _prevGroupId;
    QString text = "Remove " + _scene->agxGraphModel().GetNodeNameProperty(_node) + " From " + groupName;
    setText(text); 
}

void RemoveNodeFromGroupCommand::undo() {
    _scene->agxGraphModel().AddToNodeGroup(_node, _prevGroupId);
}

void RemoveNodeFromGroupCommand::redo() {
    _scene->agxGraphModel().RemoveFromNodeGroup(_node);
}

RenameNodeCommand::RenameNodeCommand(AgxGraphicsScene* scene, AgxNodeId node, const QString& newName) : _scene(scene), _node(node), _newName(newName) { 
    _prevName = _scene->agxGraphModel().GetNodeNameProperty(_node);
    setText("Rename " + _prevName + " To " + _newName); 
}

void RenameNodeCommand::undo()
{
    _scene->agxGraphModel().SetNodeNameProperty(_node, _prevName);
}

void RenameNodeCommand::redo()
{   
    _scene->agxGraphModel().SetNodeNameProperty(_node, _newName);
}

AddPortCommand::AddPortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type, AgxPortIndex index) : _scene(scene), _node(node), _type(type), _port(index) { 
    QString dir;
    switch (_type)
    {
    case AgxPortType::In:
        dir = "In";
        break;
    case AgxPortType::Out:
        dir = "Out";
        break;
    default:
        dir = "Unknown";
        break;
    }
    
    setText("Add " + dir + " Port To " + _scene->agxGraphModel().GetNodeNameProperty(_node)); 
}

AddPortCommand::AddPortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type) : AddPortCommand(scene, node, type, 0) {
    unsigned int portcount = 0;
    switch (_type)
    {
    case AgxPortType::In:
        portcount = _scene->agxGraphModel().nodeData(_node, AgxNodeRole::InPortCount).toUInt();
        break;
    case AgxPortType::Out:
        portcount = _scene->agxGraphModel().nodeData(_node, AgxNodeRole::OutPortCount).toUInt();
        break;
    }
    _port = portcount; 
}

void AddPortCommand::undo()
{
    _portData = _scene->agxGraphModel().removePort(_node, _type, _port, true);
}

void AddPortCommand::redo()
{
    _scene->agxGraphModel().addPort(_node, _type, _port, _portData);
}

RemovePortCommand::RemovePortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type, AgxPortIndex port) : _scene(scene), _node(node), _type(type), _port(port) {
    QString dir;
    switch (_type)
    {
    case AgxPortType::In:
        dir = "In";
        break;
    case AgxPortType::Out:
        dir = "Out";
        break;
    default:
        dir = "Unknown";
        break;
    }

    setText("Remove " + dir + " Port From " + _scene->agxGraphModel().GetNodeNameProperty(_node));
}

void RemovePortCommand::undo()
{
    _scene->agxGraphModel().addPort(_node, _type, _port, _data);

    if (!_connections.empty())
    {
        for (auto& conn : _connections)
        {
            _scene->agxGraphModel().addConnection(conn);
        }
    }
    
}
void RemovePortCommand::redo()
{
    _connections = _scene->agxGraphModel().connections(_node, _type, _port);
    _data = _scene->agxGraphModel().removePort(_node, _type, _port);
}

ChangeGroupColorCommand::ChangeGroupColorCommand(AgxGraphicsScene* scene, const QString& groupId, QColor newColor) : _scene(scene), _groupId(groupId), _newColor(newColor)
{
    setText("Change Group Color For " + _groupId);
    if (!_newColor.isValid()) setObsolete(true);

    _oldColor = _scene->agxGraphModel().GetGroupColor(_groupId);
}

void ChangeGroupColorCommand::undo()
{
    _scene->agxGraphModel().SetGroupColor(_groupId, _oldColor);
    for (auto& i : _scene->agxGraphModel().allNodeIds())
    {
        _scene->agxGraphModel().nodeUpdated(i);
    }
}

void ChangeGroupColorCommand::redo()
{
    _scene->agxGraphModel().SetGroupColor(_groupId, _newColor);
    for (auto& i : _scene->agxGraphModel().allNodeIds())
    {
        _scene->agxGraphModel().nodeUpdated(i);
    }
}


InsertPropertySheetDataCommand::InsertPropertySheetDataCommand(AgxGraphicsScene* scene, const AgxNodeId& node, const QJsonObject& newIData)
    : _scene(scene), _nodeId(node), _newIData(newIData), _model(nullptr)
{
    auto sourceObj = _scene->agxGraphModel().getPropertySheetData(_nodeId, false);

    _oldIData = FindJsonOverlaps(sourceObj, _newIData);

    setText("Modify " + _scene->agxGraphModel().GetNodeNameProperty(_nodeId));

    if (_oldIData == _newIData) {
        setObsolete(true);
    }
    qDebug() << "old " << _oldIData;
    qDebug() << "new " << _newIData;
}
InsertPropertySheetDataCommand::InsertPropertySheetDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QJsonObject& newIData)
    : _scene(scene), _model(model), _newIData(newIData), _nodeId(InvalidNodeId)
{
    auto sourceObj = _scene->agxGraphModel().getPropertySheetData(false);

    _oldIData = FindJsonOverlaps(sourceObj, _newIData);

    setText("Modify Graph Propery");

    if (_oldIData == _newIData) {
        setObsolete(true);
    }
    qDebug() << "old graph " << _oldIData;
    qDebug() << "new graph " << _newIData;
}
void InsertPropertySheetDataCommand::undo()
{
    if(_nodeId != InvalidNodeId)
        _scene->agxGraphModel().insertPropertySheetData(_nodeId, _oldIData);

    if (_model)
        _scene->agxGraphModel().insertPropertySheetData(_oldIData);
}
void InsertPropertySheetDataCommand::redo()
{
    if (_nodeId != InvalidNodeId)
        _scene->agxGraphModel().insertPropertySheetData(_nodeId, _newIData);

    if (_model)
        _scene->agxGraphModel().insertPropertySheetData(_newIData);
}
#pragma endregion

AddRowToPropertyBlockDataCommand::AddRowToPropertyBlockDataCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const QString& block, int index) : _scene(scene), _model(nullptr), _nodeId(nodeId), _block(block), _index(index)
{
    setText("Add Entry To " + _scene->agxGraphModel().GetNodeNameProperty(_nodeId));
}

AddRowToPropertyBlockDataCommand::AddRowToPropertyBlockDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QString& block, int index) : _scene(scene), _model(model), _nodeId(InvalidNodeId), _block(block), _index(index)
{
    setText("Add Entry To Graph");
}

void AddRowToPropertyBlockDataCommand::undo()
{
    if(_nodeId != InvalidNodeId)
        _scene->agxGraphModel().removePropertyBlockEntry(_nodeId, _block, _index);

    if (_model)
        _scene->agxGraphModel().removePropertyBlockEntry(_block, _index);
}

void AddRowToPropertyBlockDataCommand::redo()
{
    if (_nodeId != InvalidNodeId)
        _scene->agxGraphModel().addPropertyBlockEntry(_nodeId, _block, _index);

    if (_model)
        _scene->agxGraphModel().addPropertyBlockEntry(_block, _index);

    qDebug() << "Scene: " << _scene<< " Add Row Command-> id: " << _nodeId << " block: " << _block << " index: " << _index;
}

RemoveRowFromPropertyBlockDataCommand::RemoveRowFromPropertyBlockDataCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const QString& block, int index) : _scene(scene), _model(nullptr), _nodeId(nodeId), _block(block), _index(index)
{
    setText("Remove Entry From " + _scene->agxGraphModel().GetNodeNameProperty(_nodeId));
}

RemoveRowFromPropertyBlockDataCommand::RemoveRowFromPropertyBlockDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QString& block, int index) : _scene(scene), _model(model), _nodeId(InvalidNodeId), _block(block), _index(index)
{
    setText("Remove Entry From Graph");
}

void RemoveRowFromPropertyBlockDataCommand::undo()
{
    if (_nodeId != InvalidNodeId)
        _scene->agxGraphModel().addPropertyBlockEntry(_nodeId, _block, _index, _data);

    if (_model)
        _scene->agxGraphModel().addPropertyBlockEntry(_block, _index, _data);
}

void RemoveRowFromPropertyBlockDataCommand::redo()
{
    if (_nodeId != InvalidNodeId)
        _data = _scene->agxGraphModel().removePropertyBlockEntry(_nodeId, _block, _index);

    if (_model)
        _data = _scene->agxGraphModel().removePropertyBlockEntry(_block, _index);
}

ToggleNodeHiddenStateCommand::ToggleNodeHiddenStateCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId) : _scene(scene), _nodeId(nodeId)
{
    auto sourceObj = _scene->agxGraphModel().getPropertySheetData(_nodeId, false);
    QJsonObject send;
    send["hiddenState"] = "True"; //dummy value
    auto oldData = FindJsonOverlaps(sourceObj, send);
    _previousValue = oldData;
    bool newBool = !oldData["hiddenState"].toBool();
    _newValue["hiddenState"] = QJsonValue(newBool);
    setText("Toggle " + _scene->agxGraphModel().GetNodeNameProperty(_nodeId)+" Hidden Entries");
}

void ToggleNodeHiddenStateCommand::undo()
{
    _scene->agxGraphModel().insertPropertySheetData(_nodeId, _previousValue);
}

void ToggleNodeHiddenStateCommand::redo()
{
    _scene->agxGraphModel().insertPropertySheetData(_nodeId, _newValue);
}

SetNewGraphPropertiesCommand::SetNewGraphPropertiesCommand(AgxGraphicsScene* scene, const AgxGraphType& graphType) : _scene(scene), _newGraphType(graphType) {
    _oldGraphType = scene->agxGraphModel().getGraphType();
    setText("Switch Graph From " + AgxGraphTypeAsString(scene->agxGraphModel().getGraphType()) + " To " + AgxGraphTypeAsString(graphType));
}

void SetNewGraphPropertiesCommand::undo() {
    _newIData = _scene->agxGraphModel().SetNewGraphProperties(_oldGraphType);
    _scene->agxGraphModel().insertPropertySheetData(_oldIData);
}
void SetNewGraphPropertiesCommand::redo() {
    _oldIData = _scene->agxGraphModel().SetNewGraphProperties(_newGraphType);
    _scene->agxGraphModel().insertPropertySheetData(_newIData);
}

AgxPortCommandPayloadCommand::AgxPortCommandPayloadCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const AgxPortType type, const AgxPortIndex& index, const QJsonObject& commandData)
    : _scene(scene), _nodeId(nodeId), _type(type), _index(index), _data(commandData)
{
    if (commandData.contains("command-text")) setText(commandData["command-text"].toString());
}

void AgxPortCommandPayloadCommand::undo()
{
    if (_data.contains("command-undo"))
    {
        QString payload = _data.contains("command-payload") ? _data["command-payload"].toString() : "";
        _scene->agxGraphModel().sendPortCommand(_nodeId, _type, _index, _data["command-undo"].toString(), payload);
    }
}

void AgxPortCommandPayloadCommand::redo()
{
    if(_data.contains("command-redo")) 
    {
        QString payload = _data.contains("command-payload") ? _data["command-payload"].toString() : "";
        _scene->agxGraphModel().sendPortCommand(_nodeId, _type, _index, _data["command-redo"].toString(), payload);
    }
}

AgxNodeAltStateCommand::AgxNodeAltStateCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, bool unset) : _scene(scene), _nodeId(nodeId), _prevNode(InvalidNodeId), _unset(unset)
{
    QString text = _unset ? "Unset Default Node" : "Set Default Node";
    setText(text);

    if(!unset)
        for (auto& key : _scene->agxGraphModel().allNodeIds()) {
            if (_scene->agxGraphModel().nodeData(key, AgxNodeRole::AlternateState).toBool())
            {
                _prevNode = key;
            }
        }

    if (_nodeId == InvalidNodeId) setObsolete(true);
}

void AgxNodeAltStateCommand::undo()
{
    if (_unset)
    {
        _scene->agxGraphModel().setNodeData(_nodeId, AgxNodeRole::AlternateState, true);
    }
    else
    {
        _scene->agxGraphModel().setNodeData(_nodeId, AgxNodeRole::AlternateState, false);
        
        if(_prevNode != InvalidNodeId)
        {
            _scene->agxGraphModel().setNodeData(_prevNode, AgxNodeRole::AlternateState, true);
            Q_EMIT _scene->agxGraphModel().nodeUpdated(_prevNode);
        }
    }
    Q_EMIT _scene->agxGraphModel().nodeUpdated(_nodeId);
}

void AgxNodeAltStateCommand::redo()
{
    if (_unset)
    {
        _scene->agxGraphModel().setNodeData(_nodeId, AgxNodeRole::AlternateState, false);
    }
    else
    {
        _scene->agxGraphModel().setNodeData(_nodeId, AgxNodeRole::AlternateState, true);
        
        if (_prevNode != InvalidNodeId)
        {
            _scene->agxGraphModel().setNodeData(_prevNode, AgxNodeRole::AlternateState, false);
            Q_EMIT _scene->agxGraphModel().nodeUpdated(_prevNode);
        }
    }
    Q_EMIT _scene->agxGraphModel().nodeUpdated(_nodeId);
}

AgxSetGraphCategory::AgxSetGraphCategory(AgxGraphModel* model, const QString& newCategory) : _model(model), _newCat(newCategory)
{
    if (!_model) 
    {
        qDebug() << "ERROR ON CATEGORY COMMAND NO MODEL PTR";
        setObsolete(true);
        return;
    }

    _oldCat = _model->getGraphCategory();
    setText(std::format("Set Graph Category: {} -> {}", _oldCat.toStdString().c_str(), _newCat.toStdString().c_str()).c_str());
    qDebug() << text();
}

void AgxSetGraphCategory::undo()
{
    _model->setGraphCategory(_oldCat);
    Q_EMIT _model->PropertySheetUpdated();
}

void AgxSetGraphCategory::redo()
{
    _model->setGraphCategory(_newCat);
    Q_EMIT _model->PropertySheetUpdated();
}

AgxHideCommand::AgxHideCommand(AgxGraphicsScene* scene, bool hide) : _scene(scene), _toHide(hide)
{
    if (_toHide)
        setText("Hide Selected Objects");
    else
        setText("Unhide Selected Objects");

    auto& graphModel = _scene->agxGraphModel();

    bool isObsolete = true;

    for (QGraphicsItem* item : _scene->selectedItems()) {
        if (auto n = qgraphicsitem_cast<AgxNodeGraphicsObject*>(item)) {

            if (_scene->agxGraphModel().nodeData(n->nodeId(), AgxNodeRole::CollapseState).toBool() != _toHide)
            {
                _selectedNodes.insert(n->nodeId());
                isObsolete = false;
            }
        }
    }

    for (QGraphicsItem* item : _scene->selectedItems()) {
        if (auto c = qgraphicsitem_cast<AgxConnectionGraphicsObject*>(item)) {

            if (c->connectionId().isHidden != _toHide)
            {
                _selectedConnections.append(c->connectionId());
                isObsolete = false;
            }
        }
    }

    if ( isObsolete || (_selectedConnections.count() < 1 && _selectedNodes.count() < 1))
    {
        setObsolete(true);
    }
}

void AgxHideCommand::undo()
{
    _scene->agxGraphModel().SetNodesCollapsed(_selectedNodes.values(), !_toHide);
    _scene->setConnectionsHidden(_selectedConnections, !_toHide);
}

void AgxHideCommand::redo()
{
    _scene->agxGraphModel().SetNodesCollapsed(_selectedNodes.values(), _toHide);
    _scene->setConnectionsHidden(_selectedConnections, _toHide);
}
