//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxGraphModel.h"
#pragma warning(push,0)
#include "Utilities/AgxConnectionIdUtils.h"
#pragma warning(pop)
#include <stack>
#include "AgxNodes/AgxNode.h"
#include "AgxNodes/AgxCommentNode.h"
#include "Painter/QColorHelper.h"
#include "Utilities/AgxJsonHelper.h"
#include "Utilities/AgxDefinitions.h"
#include "Utilities/AgxNodeRegistry.h"
#include "Utilities/AgxGraphRegistry.h"
#include "Utilities/SettingsRegistry.h"
#include "version.h"
#include <Application/CALUMIMotionApplication.h>

AgxGraphModel::AgxGraphModel(AgxGameType type, AgxGraphModel* rootGraph) : _nextNodeId{0}, _gameType(type)
{
    _registry = AgxNodeRegistry::GetInstance().GetRegistry(_gameType);
    SetNewGraphProperties(AgxGetDefaultGraphType(_gameType));

    _rootReference = rootGraph ? rootGraph : this;
}

QSet<AgxNodeId> AgxGraphModel::allNodeIds() const
{
    QSet<AgxNodeId> nodeIds;
    for_each(_models.begin(), _models.end(), [&nodeIds](auto const& p) { nodeIds.insert(p.first); });

    return nodeIds;
}

std::unordered_set<AgxConnectionId> AgxGraphModel::allConnectionIds(AgxNodeId const nodeId) const
{
    std::unordered_set<AgxConnectionId> result;

    std::copy_if(_connectivity.begin(),
                _connectivity.end(),
                std::inserter(result, std::end(result)),
                [&nodeId](AgxConnectionId const& cid) {
                    return cid.inNodeId == nodeId || cid.outNodeId == nodeId;
                });

    return result;
}

std::unordered_set<AgxConnectionId> AgxGraphModel::connections(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex) const
{
    std::unordered_set<AgxConnectionId> result;

    std::copy_if(_connectivity.begin(),
                _connectivity.end(),
                std::inserter(result, std::end(result)),
                [&portType, &portIndex, &nodeId](AgxConnectionId const& cid) {
                    return (getNodeId(portType, cid) == nodeId
                        && getPortIndex(portType, cid) == portIndex);
                });

    return result;
}

bool AgxGraphModel::connectionExists(AgxConnectionId const connectionId) const
{
    return (_connectivity.find(connectionId) != _connectivity.end());
}

AgxNodeId AgxGraphModel::addNode(QString const nodeType)
{
    std::unique_ptr<AgxNode> model = _registry->create(nodeType, this);

    if (model) {
        AgxNodeId newId;

        model->SetUpNode(_gameType);
        for (unsigned int j = 0; j < model->nPorts(AgxPortType::In); j++)
        {
            model->_AddPort(AgxPortType::In,j);
        }
        for (unsigned int j = 0; j < model->nPorts(AgxPortType::Out); j++)
        {
            model->_AddPort(AgxPortType::Out,j);
        }
        {
            Q_EMIT model->ParentGraphTypeUpdated(_graphType);
        }
        

        if (dynamic_cast<AgxCommentNode*>(model.get()))
        {
            if (_nextMiscNodeId >= (InvalidNodeId - 1))
                return InvalidNodeId;

            newId = newMiscNodeId();
        }
        else
        {
            if (_nextNodeId >= (_firstMiscNodeId - 1))
                return InvalidNodeId;

            newId = newNodeId();
        }

        model->SetNodeIdRef(newId);

        connect(model.get(),
            &AgxNode::dataUpdated,
            [newId, this](AgxPortIndex const portIndex) {
                onOutPortDataUpdated(newId, portIndex);
            });

        connect(model.get(),
            &AgxNode::portsAboutToBeDeleted,
            this,
            [newId, this](AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last) {
                portsAboutToBeDeleted(newId, portType, first, last);
            });

        connect(model.get(),
            &AgxNode::portsDeleted,
            this,
            &AgxGraphModel::portsDeleted);

        connect(model.get(),
            &AgxNode::portsAboutToBeInserted,
            this,
            [newId, this](AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last) {
                portsAboutToBeInserted(newId, portType, first, last);
            });

        connect(model.get(),
            &AgxNode::portsInserted,
            this,
            &AgxGraphModel::portsInserted);

        _models[newId] = std::move(model);
        //_nodePortCounts.insert({ newId, { _models[newId]->nPorts(AgxPortType::In), _models[newId]->nPorts(AgxPortType::Out)} });
        

        Q_EMIT nodeCreated(newId);

        return newId;
    }

    return InvalidNodeId;
}

void AgxGraphModel::HandleEmbeddedClosures()
{
    for (auto nodeId : allNodeIds())
    {
        if (auto agxNode = _models[nodeId].get())
        {
            agxNode->CloseEmbeddedView();
        }
    }
}

QWidget* AgxGraphModel::GetNodeSidebarContent(const AgxNodeId& nodeId)
{
    if (auto agxNode = _models.at(nodeId).get())
    {
        return agxNode->GetSideBarContent();
    }

    return nullptr;
}

void AgxGraphModel::SetNodeSidebarVisibility(const AgxNodeId& nodeId, bool state)
{
    if (auto agxNode = _models.at(nodeId).get())
    {
        agxNode->SetSidebarVisibility(state);
    }
}

bool AgxGraphModel::connectionPossible(AgxConnectionId const connectionId) const
{
    // Check if nodes exist
    if (!nodeExists(connectionId.outNodeId) || !nodeExists(connectionId.inNodeId)) {
        return false;
    }

    // Check either node is collapsed
    if (nodeData<bool>(connectionId.outNodeId, AgxNodeRole::CollapseState) || nodeData<bool>(connectionId.inNodeId, AgxNodeRole::CollapseState)) {
        return false;
    }

    // Check port bounds, i.e. that we do not connect non-existing port numbers
    auto checkPortBounds = [&](AgxPortType const portType) {
        AgxNodeId const nodeId = getNodeId(portType, connectionId);
        auto portCountRole = (portType == AgxPortType::Out) ? AgxNodeRole::OutPortCount
            : AgxNodeRole::InPortCount;

        std::size_t const portCount = nodeData(nodeId, portCountRole).toUInt();

        return getPortIndex(portType, connectionId) < portCount;
        };

    auto getDataType = [&](AgxPortType const portType) {
        return portData(getNodeId(portType, connectionId),
            portType,
            getPortIndex(portType, connectionId),
            AgxPortRole::DataType)
            .value<AgxNodeDataType>();
        };

    auto portVacant = [&](AgxPortType const portType) {
        AgxNodeId const nodeId = getNodeId(portType, connectionId);
        AgxPortIndex const portIndex = getPortIndex(portType, connectionId);
        auto const connected = connections(nodeId, portType, portIndex);

        auto policy = portData(nodeId, portType, portIndex, AgxPortRole::ConnectionPolicyRole)
            .value<AgxConnectionPolicy>();

        return connected.empty() || (policy == AgxConnectionPolicy::Many);
        };

    bool const basicChecks = getDataType(AgxPortType::Out).id == getDataType(AgxPortType::In).id
        && portVacant(AgxPortType::Out) && portVacant(AgxPortType::In)
        && checkPortBounds(AgxPortType::Out) && checkPortBounds(AgxPortType::In);

    // In data-flow mode (this class) it's important to forbid graph loops.
    // We perform depth-first graph traversal starting from the "Input" port of
    // the given connection. We should never encounter the starting "Out" node.

    auto hasLoops = [this, &connectionId]() -> bool {
        std::stack<AgxNodeId> filo;
        filo.push(connectionId.inNodeId);

        while (!filo.empty()) {
            auto id = filo.top();
            filo.pop();

            if (id == connectionId.outNodeId) { // LOOP!
                return true;
            }

            // Add out-connections to continue interations
            std::size_t const nOutPorts = nodeData(id, AgxNodeRole::OutPortCount).toUInt();

            for (AgxPortIndex index = 0; index < nOutPorts; ++index) {
                auto const& outConnectionIds = connections(id, AgxPortType::Out, index);

                for (auto& cid : outConnectionIds) {
                    filo.push(cid.inNodeId);
                }
            }
        }

        return false;
        };

    return basicChecks && (loopsEnabled() || !hasLoops());
}

void AgxGraphModel::addConnection(AgxConnectionId const connectionId)
{
    _connectivity.insert(connectionId);

    sendConnectionCreation(connectionId);

    QVariant const portDataToPropagate = portData(connectionId.outNodeId,
        AgxPortType::Out,
        connectionId.outPortIndex,
        AgxPortRole::Data);

    setPortData(connectionId.inNodeId,
        AgxPortType::In,
        connectionId.inPortIndex,
        portDataToPropagate,
        AgxPortRole::Data);
}

bool AgxGraphModel::nodeExists(AgxNodeId const nodeId) const
{
    return (_models.find(nodeId) != _models.end());
}

QVariant AgxGraphModel::nodeData(AgxNodeId nodeId, AgxNodeRole role) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return result;

    auto& model = it->second;

    switch (role) {
        case AgxNodeRole::Type:
            result = model->name();
            break;

        case AgxNodeRole::Position:
            result = _nodeGeometryData[nodeId].pos;
            break;

        case AgxNodeRole::Size:
            result = _nodeGeometryData[nodeId].size;
            break;

        case AgxNodeRole::CaptionVisible:
            result = model->captionVisible();
            break;

        case AgxNodeRole::Caption:
            result = model->caption();
            break;

        case AgxNodeRole::Style:
        {
            auto& style = _models.at(nodeId).get()->nodeStyle();

            result = style.toJson().toVariantMap();
        } break;

        case AgxNodeRole::InternalData:
        {
            QJsonObject nodeJson;

            nodeJson["internal-data"] = _models.at(nodeId)->save();

            result = nodeJson.toVariantMap();
            break;
        }

        case AgxNodeRole::InPortCount: {
            result = model->PortCount(AgxPortType::In);
        }   break;

        case AgxNodeRole::OutPortCount: {
            result = model->PortCount(AgxPortType::Out);
        }   break;

        case AgxNodeRole::Widget: {
            auto* w = model->embeddedWidget();
            result = QVariant::fromValue(w);
        }   break;

        case AgxNodeRole::ValidationState: {
            auto validationState = model->validationState();
            result = QVariant::fromValue(validationState);
        }   break;

        case AgxNodeRole::AlternateState: {
            result = model->AltState();   
        }   break;

        case AgxNodeRole::SubCaption: {
            result = model->SubCaption();
        }   break;

        case AgxNodeRole::CollapseState: {
            result = model->isCollapsed();
        }   break;
    }

    return result;
}

AgxNodeFlags AgxGraphModel::nodeFlags(AgxNodeId nodeId) const
{
    auto it = _models.find(nodeId);
    auto flags = AgxNodeFlag::NoFlags;

    if (it == _models.end())
        return flags;


    if (it->second->resizable())
        flags = AgxNodeFlag::Resizable;

    return flags;
}

bool AgxGraphModel::setNodeData(AgxNodeId nodeId, AgxNodeRole role, QVariant value)
{
    Q_UNUSED(nodeId);
    Q_UNUSED(role);
    Q_UNUSED(value);

    bool result = false;

    auto agxNode = _models.at(nodeId).get();

    if (!agxNode) return result;

    switch (role) {
    case AgxNodeRole::Type:
        break;
    case AgxNodeRole::Position: {
        _nodeGeometryData[nodeId].pos = value.value<QPointF>();

        Q_EMIT nodePositionUpdated(nodeId);

        result = true;
    } break;

    case AgxNodeRole::Size: {
        _nodeGeometryData[nodeId].size = value.value<QSize>();
        result = true;
    } break;

    case AgxNodeRole::CaptionVisible:
        break;

    case AgxNodeRole::Caption:
        break;

    case AgxNodeRole::Style:
        break;

    case AgxNodeRole::InternalData: {
        agxNode->insertPropertySheetData(value.toJsonObject());
    }   break;

    case AgxNodeRole::InPortCount: {
            agxNode->SetPortCount(AgxPortType::In, value.toUInt());
    }   break;

    case AgxNodeRole::OutPortCount: {
            agxNode->SetPortCount(AgxPortType::Out, value.toUInt());
    }   break;

    case AgxNodeRole::Widget:
        break;

    case AgxNodeRole::ValidationState: {
        if (value.canConvert<AgxNodeValidationState>()) {
            auto state = value.value<AgxNodeValidationState>();
            agxNode->setValidatonState(state);
        }
        Q_EMIT nodeUpdated(nodeId);
    } break;
    case AgxNodeRole::AlternateState: {
        agxNode->SetAltState(value.toBool());
    }   break;
    case AgxNodeRole::SubCaption: 
        break;
    case AgxNodeRole::CollapseState: {
        if (agxNode->isCollapsed() != value.toBool())
            agxNode->ToggleCollapse();
    }   break;
    }

    return result;
}

void AgxGraphModel::insertPropertySheetData(AgxNodeId nodeId, QJsonObject data)
{
    if (auto agxNode = _models[nodeId].get()) {
        agxNode->insertPropertySheetData(data);
    }
}

void AgxGraphModel::insertPropertySheetData(QJsonObject data)
{
    QJsonObject entries = data; //using clone since entries are removed

    QJsonObject blocks = data["property-blocks"].toObject();

    if (entries.contains("property-blocks")) entries.remove("property-blocks");

    for (auto& prop : _PropertyEntries)
    {
        if (entries.contains(prop.Tag())) {
            if(entries[prop.Tag()].toObject().contains("value"))
                prop.value = entries[prop.Tag()].toObject()["value"].toString();
            if (entries[prop.Tag()].toObject().contains("isPresent"))
                prop.SetIsPresent(entries[prop.Tag()].toObject()["isPresent"].toBool());
            entries.remove(prop.Tag());
        }
    }

    if (entries.count() > 0) {
        for (unsigned int i = 0; i < entries.count(); i++) {
            QString key = entries.keys().at(i);
            _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ErrorTerm, entries[key].toString()));
        }
    }

    if (!entries.isEmpty()) {
        qDebug() << "WARNING: GRAPH PROPERTIES INSERT DID NOT COMPLETE";
        qDebug() << "Data Still Remaining: \n\t" << entries;
    }

    //need block data...
    for (int i = 0; i < _PropertyBlocks.size(); i++)
    {
        auto key = _PropertyBlocks.keys().at(i);
        if (blocks.contains(key().tag))
        {
            _PropertyBlocks[key].insertPropertyBlockData(blocks[key().tag].toObject());
            blocks.remove(key().tag);
        }
    }
    if (!blocks.isEmpty()) {
        qDebug() << "WARNING: GRAPH PROPERTY BLOCK INSERT DID NOT COMPLETE";
        qDebug() << "Data Still Remaining: \n\t" << blocks;
    }

    Q_EMIT PropertySheetUpdated();
}

QJsonObject AgxGraphModel::getPropertySheetData(AgxNodeId nodeId, bool cleared)
{
    QJsonObject output;

    if (auto agxNode = _models[nodeId].get()) {
        output = agxNode->getPropertySheetData(cleared);
    }

    return output;
}

QJsonObject AgxGraphModel::getPropertySheetData(bool cleared)
{
    QJsonObject output;

    QJsonObject blocks;
    for (int i = 0; i < _PropertyBlocks.size(); i++)
    {
        auto key = _PropertyBlocks.keys().at(i);
        blocks[key().tag] = _PropertyBlocks.value(key).getPropertyBlockData(cleared);
    }
    output["property-blocks"] = blocks;


    for (int i = 0; i < _PropertyEntries.size(); i++)
    {
        QString key = _PropertyEntries.at(i).Tag();
        QJsonObject pobj;
        pobj["value"] = cleared ? "" : _PropertyEntries[i].value;
        pobj["isPresent"] = cleared ? false : _PropertyEntries[i].isPresent;
        output[key] = pobj;
    }

    return output;
}

QJsonObject AgxGraphModel::getPropertySheetData(bool cleared) const
{
    QJsonObject output;

    QJsonObject blocks;
    for (int i = 0; i < _PropertyBlocks.size(); i++)
    {
        auto key = _PropertyBlocks.keys().at(i);
        blocks[key().tag] = _PropertyBlocks.value(key).getPropertyBlockData(cleared);
    }
    output["property-blocks"] = blocks;


    for (int i = 0; i < _PropertyEntries.size(); i++)
    {
        QString key = _PropertyEntries.at(i).Tag();
        QJsonObject pobj;
        pobj["value"] = cleared ? "" : _PropertyEntries[i].value;
        pobj["isPresent"] = cleared ? false : _PropertyEntries[i].isPresent;
        output[key] = pobj;
    }

    return output;
}

void AgxGraphModel::addPropertyBlockEntry(AgxNodeId nodeId, QString block, int index, const QList<AgxPropertyBlockData::Entry>& data)
{
    if (auto agxNode = _models[nodeId].get())
    {
        if (auto blockPtr = agxNode->getPropertyBlock(block))
        {
            blockPtr->AddRow(index);
            blockPtr->SetRow(index, data);
            Q_EMIT blockPtr->DataUpdated();
        }
    }
}

void AgxGraphModel::addPropertyBlockEntry(QString block, int index, const QList<AgxPropertyBlockData::Entry>& data)
{
    if (auto blockPtr = getPropertyBlock(block))
    {
        blockPtr->AddRow(index);
        blockPtr->SetRow(index, data);
        Q_EMIT blockPtr->DataUpdated();
    }
}

AgxPropertyBlockData* AgxGraphModel::getPropertyBlock(const QString& block)
{
    for (auto blockRef : _PropertyBlocks.keys()) {
        if (block.compare(blockRef().tag) == 0) {
            return &_PropertyBlocks[blockRef];
        }
    }

    return nullptr;
}

AgxPropertyBlockData* AgxGraphModel::getPropertyBlock(TermRef ref)
{
    if(!_PropertyBlocks.contains(ref))
        return nullptr;
    
    return &_PropertyBlocks[ref];
}

QList<AgxPropertyBlockData::Entry> AgxGraphModel::removePropertyBlockEntry(AgxNodeId nodeId, QString block, int index)
{
    if (auto agxNode = _models[nodeId].get())
    {
        if (auto blockPtr = agxNode->getPropertyBlock(block))
        {
            QList<AgxPropertyBlockData::Entry> rowData = *blockPtr->GetRow(index);
            blockPtr->RemoveRow(index);
            return rowData;
        }
    }
    return QList<AgxPropertyBlockData::Entry>();
}

QList<AgxPropertyBlockData::Entry> AgxGraphModel::removePropertyBlockEntry(QString block, int index)
{
    if (auto blockPtr = getPropertyBlock(block))
    {
        QList<AgxPropertyBlockData::Entry> rowData = *blockPtr->GetRow(index);
        blockPtr->RemoveRow(index);
        return rowData;
    }
    return QList<AgxPropertyBlockData::Entry>();
}

QVariant AgxGraphModel::portData(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, AgxPortRole role) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return result;

    auto& model = it->second;

    switch (role) {
    case AgxPortRole::Data:
        if (portType == AgxPortType::Out) {
            result = QVariant::fromValue(model->outData(portIndex));
        }
        break;

    case AgxPortRole::DataType:
        result = QVariant::fromValue(model->dataType(portType, portIndex));
        break;

    case AgxPortRole::ConnectionPolicyRole:
        result = QVariant::fromValue(model->portConnectionPolicy(portType, portIndex));
        break;

    case AgxPortRole::CaptionVisible:
        result = model->portCaptionVisible(portType, portIndex);
        break;

    case AgxPortRole::Caption:
        result = model->portCaption(portType, portIndex);
        break;

    case AgxPortRole::InternalData:
        result = model->PortData(portType, portIndex);
        break;
    }

    return result;
}

bool AgxGraphModel::setPortData(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, QVariant const& value, AgxPortRole role)
{
    Q_UNUSED(nodeId);

    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return false;

    auto& model = it->second;

    switch (role) {
    case AgxPortRole::Data:
        if (portType == AgxPortType::In) {
            model->setInData(value.value<std::shared_ptr<AgxNodeData>>(), portIndex);

            // Triggers repainting on the scene.
            Q_EMIT inPortDataWasSet(nodeId, portType, portIndex);
        }
        break;
    case AgxPortRole::InternalData:
        model->SetPortData(portType, portIndex, value.toJsonObject());
        break;
    default:
        break;
    }

    return false;
}

void AgxGraphModel::sendPortCommand(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, const QString& command, const QString& payload)
{
    auto it = _models.find(nodeId);
    if (it == _models.end()) return;

    auto& model = it->second;
    model->_ExternalPortCommand(portType, portIndex, command, payload);    
}

bool AgxGraphModel::deleteConnection(AgxConnectionId const connectionId)
{
    bool disconnected = false;

    auto it = _connectivity.find(connectionId);

    if (it != _connectivity.end()) {
        disconnected = true;

        _connectivity.erase(it);
    }

    if (disconnected) {
        sendConnectionDeletion(connectionId);

        propagateEmptyDataTo(getNodeId(AgxPortType::In, connectionId),
            getPortIndex(AgxPortType::In, connectionId));
    }

    return disconnected;
}

bool AgxGraphModel::deleteNode(AgxNodeId const nodeId)
{
    // Delete connections to this node first.
    auto connectionIds = allConnectionIds(nodeId);
    for (auto& cId : connectionIds) {
        deleteConnection(cId);
    }

    _nodeGeometryData.erase(nodeId);
    _models.erase(nodeId);
    //_nodePortCounts.erase(nodeId);

    Q_EMIT nodeDeleted(nodeId);

    return true;
}

void AgxGraphModel::onOutPortDataUpdated(AgxNodeId const nodeId, AgxPortIndex const portIndex)
{
    std::unordered_set<AgxConnectionId> const& connected = connections(nodeId,
        AgxPortType::Out,
        portIndex);

    QVariant const portDataToPropagate = portData(nodeId, AgxPortType::Out, portIndex, AgxPortRole::Data);

    for (auto const& cn : connected) {
        setPortData(cn.inNodeId, AgxPortType::In, cn.inPortIndex, portDataToPropagate, AgxPortRole::Data);
    }
}

void AgxGraphModel::SetNodeNameProperty(const AgxNodeId nodeId, const QString& newName)
{
    auto node = _models[nodeId].get();
    if (node && node->CanSetNameProperty())
    {
        node->SetNameProperty(newName);
        Q_EMIT nodeUpdated(nodeId);
    }
}

QString AgxGraphModel::GetNodeNameProperty(const AgxNodeId nodeId) const
{
    return _models.at(nodeId).get()->caption();
}

bool AgxGraphModel::CanSetNodeNameProperty(const AgxNodeId nodeId)
{
    
    if (auto node = _models[nodeId].get()) {
        return node->CanSetNameProperty();
    }
    return false;
}

void AgxGraphModel::ToggleNodeCollapse(const AgxNodeId nodeId)
{
    
    if (auto node = _models[nodeId].get()) {
        node->ToggleCollapse();
        Q_EMIT nodeUpdated(nodeId);
    }
}

void AgxGraphModel::SetNodesCollapsed(const QList<AgxNodeId>& nodes, bool collapsed)
{
    for (auto& nodeId : nodes)
    {
        if (auto node = _models[nodeId].get())
        {
            if(node->isCollapsed() != collapsed)
            {
                node->ToggleCollapse();
                Q_EMIT nodeUpdated(nodeId);
            }

        }
    }
}

AgxPortType AgxGraphModel::CanModifyPorts(const AgxNodeId nodeId)
{
    if (auto node = _models[nodeId].get())
    {
        return node->CanModifyPorts();
    }
    return AgxPortType::None;
}

AgxNodeType AgxGraphModel::GetNodeType(const AgxNodeId nodeId)
{
    if (auto node = _models[nodeId].get())
    {
        return node->GetNodeType();
    }
    return AgxNodeType::UNDEFINED;
}

QString AgxGraphModel::GetNodeGroup(const AgxNodeId nodeId) const
{
    if (auto node = _models.at(nodeId).get()) {
        const QString& name = node->getGroupId();
        if (GroupExists(name))
            return name; }
    return "";
}

std::vector<QString> AgxGraphModel::GetNodeGroupList() const
{
    std::vector<QString> output;
    for (auto& entry : _nodeGroups)
    {
        output.push_back(entry.first);
    }
    return output;
}

QColor AgxGraphModel::GetGroupColor(QString groupName) const
{
    if (GroupExists(groupName))
        return _nodeGroups.at(groupName);

    return generateRandomQColor();
}

void AgxGraphModel::SetGroupColor(const QString& groupName, const QColor& color)
{
    if (GroupExists(groupName))
    {
        _nodeGroups.at(groupName) = color;
    }
}

std::unordered_map<QString, QVector<AgxNodeId>> AgxGraphModel::GetNodeGroupAssignmentList() const
{
    std::unordered_map<QString, QVector<AgxNodeId>> output;

    for (auto& group : _nodeGroups)
    {
        output[group.first] = {};
    }

    for (auto& entry : _models)
    {
        auto node = entry.second.get();

        if (node && node->getGroupId() != "")
        {
            output[node->getGroupId()].push_back(entry.first);
        }
        else
        {
            output["NONE"].push_back(entry.first);
        }
    }
    return output;
}

bool AgxGraphModel::GroupExists(QString groupName) const
{
    for (auto& entry : _nodeGroups)
    {
        if (entry.first == groupName)
            return true;
    }
    return false;
}

bool AgxGraphModel::AddToNodeGroup(const AgxNodeId nodeId, QString nodeGroup)
{
    if (nodeGroup == "" || nodeGroup == "NONE") return false;

    if (auto node = _models.at(nodeId).get())
        node->setGroupId(nodeGroup);

    bool output = _nodeGroups.insert({ nodeGroup, generateRandomQColor()}).second;
    
    Q_EMIT nodeUpdated(nodeId);
    return output;
}

void AgxGraphModel::RemoveFromNodeGroup(const AgxNodeId nodeId)
{
    if (auto node = _models.at(nodeId).get())
        node->setGroupId("");

    Q_EMIT nodeUpdated(nodeId);
}

bool AgxGraphModel::CreateNodeGroup(QString nodeGroup, QColor groupColor)
{
    if (nodeGroup == "" || nodeGroup == "NONE") return false;

    return _nodeGroups.insert({ nodeGroup, groupColor }).second;
}

bool AgxGraphModel::EraseNodeGroup(QString nodeGroup)
{
    for (auto& entry : _models)
    {
        auto node = entry.second.get();

        if (node && node->getGroupId() == nodeGroup)
        {
            node->setGroupId("");
            Q_EMIT nodeUpdated(entry.first);
        }
    }
    return static_cast<bool>(_nodeGroups.erase(nodeGroup));
}

void AgxGraphModel::SetRelativeDataPath(const QString& fileName, const QString& relPath)
{
    QFileInfo fileInfo(fileName);
    QString fileStem = fileName.isEmpty() ? "untitled" : fileInfo.baseName();
    fileStem[0] = fileStem[0].toUpper();

    _graphRelDataPath = relPath.isEmpty() ? SettingsRegistry::GetInstance().GetRelativeDataPath(_gameType) + fileStem + ".agx" : relPath + fileName + ".agx";
}

QString AgxGraphModel::GetRelativeDataPath() const
{
    return _graphRelDataPath;
}

const QString& AgxGraphModel::GetModelFilePath() const
{
    return _file;
}

void AgxGraphModel::SetModelFilePath(const QString& file)
{
    _file = file;
}

void AgxGraphModel::SetGraphTitle(const QString& title, bool root)
{
    if(root)
    {
        _rootReference->_agxGraphTitle = title.isEmpty() ? "untitled" : title;
        Q_EMIT _rootReference->PropertySheetUpdated();
    }
    else
    {
        _agxGraphTitle = title.isEmpty() ? "untitled" : title;
        Q_EMIT PropertySheetUpdated();
    }

    if (auto cApp = dynamic_cast<CALUMIMotionApplication*>(QCoreApplication::instance()))
    {
        cApp->UpdateApplicationTabWidgets();
    }
}

QString AgxGraphModel::GetGraphTitle(bool root) const
{
    if(root)
        return rootGraphReference()->_agxGraphTitle;

    return _agxGraphTitle;
}

QJsonObject AgxGraphModel::SetNewGraphProperties(const AgxGraphType& graphType)
{
    AgxGraphDefinition newDefinition = AgxGraphRegistry::GetInstance().GetGraphDefinition(graphType);

    QJsonObject oldData = getPropertySheetData(false);

    _PropertyBlocks.clear();
    _PropertyEntries.clear();

    _PropertyBlocks = newDefinition._defaultBlocks;
    _PropertyEntries = newDefinition._defaultEntries;
    _BlockOrder = newDefinition._blockOrder;
    _graphType = graphType;

    for (auto key : allNodeIds()) {
        
        if (auto agxNode = _models.at(key).get()) {
            Q_EMIT agxNode->ParentGraphTypeUpdated(_graphType);
            Q_EMIT agxNode->PropertySheetUpdated();
            //Q_EMIT agxNode->embeddedWidgetSizeUpdated();
            if (agxNode->AltState())
            {
                Q_EMIT nodeUpdated(key);
            }
        }
    }

    Q_EMIT GraphTypeUpdated();
    return oldData;
}

void AgxGraphModel::setGraphCategory(const QString& cat) { 
    _category = cat; 
}

const AgxGraphModel* AgxGraphModel::rootGraphReference() const {
    if (_rootReference)
        return _rootReference;

    return this;
}

void AgxGraphModel::addPort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, QJsonObject data)
{
    // STAGE 1.
    // Compute new addresses for the existing connections that are shifted and
    // placed after the new ones
    AgxPortIndex first = portIndex;
    AgxPortIndex last = first;
    portsAboutToBeInserted(nodeId, portType, first, last);

    auto agxNode = _models.at(nodeId).get();

    // STAGE 2. Change the number of connections in your model
    if (portType == AgxPortType::In && agxNode)
        agxNode->_AddPort(AgxPortType::In, portIndex, data);
    else if(agxNode)
        agxNode->_AddPort(AgxPortType::Out, portIndex, data);

    // STAGE 3. Re-create previouly existed and now shifted connections
    portsInserted();

    Q_EMIT nodeUpdated(nodeId);
    Q_EMIT portCreated(nodeId, portType, portIndex);

}

QJsonObject AgxGraphModel::removePort(AgxNodeId nodeId, AgxPortType portType, AgxPortIndex portIndex, bool preserve)
{
    QJsonObject output;

    if(auto agxNode = _models.at(nodeId).get())
    {
        // STAGE 1.
        // Compute new addresses for the existing connections that are shifted upwards
        // instead of the deleted ports.
        AgxPortIndex first = portIndex;
        AgxPortIndex last = first;
        portsAboutToBeDeleted(nodeId, portType, first, last);

        output = agxNode->PortData(portType, portIndex);

        // STAGE 2. Change the number of connections in your model
        if (agxNode)
            agxNode->_RemovePort(portType, portIndex, preserve);

        portsDeleted();

        Q_EMIT nodeUpdated(nodeId);
        Q_EMIT portDeleted(nodeId, portType, portIndex);
    }
    return output;
}

QJsonObject AgxGraphModel::saveNode(AgxNodeId const nodeId) const
{
    QJsonObject nodeJson;

    nodeJson["id"] = static_cast<qint64>(nodeId);

    nodeJson["internal-data"] = _models.at(nodeId)->save();

    {
        QPointF const pos = nodeData(nodeId, AgxNodeRole::Position).value<QPointF>();

        QJsonObject posJson;
        posJson["x"] = pos.x();
        posJson["y"] = pos.y();
        nodeJson["position"] = posJson;

        QSize size = nodeData(nodeId, AgxNodeRole::Size).value<QSize>();

        QJsonObject sizeJson;
        sizeJson["x"] = size.width();
        sizeJson["y"] = size.height();
        nodeJson["size"] = sizeJson;

        //nodeJson["inPortCount"] = QString::number(_nodePortCounts[nodeId].in);
        //nodeJson["outPortCount"] = QString::number(_nodePortCounts[nodeId].out);
    }

    return nodeJson;
}

QJsonObject AgxGraphModel::save() const
{
    QJsonObject sceneJson;

    if (_rootReference == this || !_rootReference) {
        QJsonArray versionArray = {MAJORVAL, MINORVAL, PATCHVAL, REVISIONVAL};
        sceneJson["file-version"] = versionArray;
        sceneJson["file-type"] = AgxFileTypeToString(AgxFileType::BehaviorFile);
    }

    QJsonArray nodesJsonArray;
    for (auto const nodeId : allNodeIds()) {
        nodesJsonArray.append(saveNode(nodeId));
    }
    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray groupJsonArray;
    for (auto const& group : GetNodeGroupList())
    {
        QJsonObject groupObj;
        groupObj.insert(group, colorToJsonObject(GetGroupColor(group)));
    }
    sceneJson["groups"] = groupJsonArray;

    QJsonArray connJsonArray;
    for (auto const& cid : _connectivity) {
        connJsonArray.append(toJson(cid));
    }
    sceneJson["connections"] = connJsonArray;

    sceneJson["game-type"] = AgxGameTypeToString(_gameType);

    sceneJson["graph-category"] = _category;
    sceneJson["graph-type"] = AgxGraphTypeAsString(_graphType);

    
    sceneJson["properties"] = getPropertySheetData();

    return sceneJson;
}

void AgxGraphModel::save(pugi::xml_node& parent) const
{
    AgxAppendValue(parent, "Link_Style", AgxGraphRegistry::GetInstance().GetGraphDefinition(_graphType)._tags.value("Link_Style"), AgxFormat::NewLine, 0);

    //Nodes
    {
        auto nodeList = allNodeIds().values();
        std::sort(nodeList.begin(), nodeList.end());

        QVector<AgxCommentNode*> commentNodes;

        for (auto& nodeId : nodeList)
        {
            if (_models.at(nodeId).get()->GetNodeType() == AgxNodeType::Comment) 
            {
                if (auto cNodePtr = dynamic_cast<AgxCommentNode*>(_models.at(nodeId).get())) commentNodes.push_back(cNodePtr);
                continue;
            }

            auto conns = allConnectionIds(nodeId);

            _models.at(nodeId).get()->save(parent, QVector<AgxConnectionId>({ conns.begin(), conns.end() }), nodeList, nodeData(nodeId, AgxNodeRole::Position).toPointF());
        }

        for (auto& group : GetNodeGroupAssignmentList())
        {
            if (group.first.isEmpty() || group.first.compare("none", Qt::CaseInsensitive) == 0 || group.first.compare("<none>", Qt::CaseInsensitive) == 0) continue;

            auto groupObject = AgxAppend(parent, "node_group", AgxFormat::NewLine,0);
            AgxAppendValue(groupObject, "name", group.first, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
            AgxAppendValue(groupObject, "collapsed", "False", {AgxFormat::NewLine, AgxFormat::Indent}, 1);

            for (auto& gNodeId : group.second)
            {
                auto nodeObject = AgxAppend(groupObject, "node", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
                AgxAppendValue(nodeObject, "idx", QString("%1").arg(nodeList.indexOf(gNodeId) + 1), AgxFormat::None, 0);
            }

            AgxCloseNode(groupObject, false, false, 0);
        }

        for (auto& comment : commentNodes)
        {
            auto commentObject = AgxAppend(parent, "comment", AgxFormat::NewLine, 0);
            AgxAppendValue(commentObject, "text", comment->_text, AgxFormat::None, 0);
            auto position = nodeData(comment->_nodeIdRef, AgxNodeRole::Position).toPointF();
            auto target = comment->_target;
            AgxAppendValue(commentObject, "X", CleanUpDecimals(QString("%1").arg(position.x() / SFBGSxScalar, 0, 'f', 5)), AgxFormat::None, 0);
            AgxAppendValue(commentObject, "Y", CleanUpDecimals(QString("%1").arg(position.y() / SFBGSyScalar,0, 'f', 5)), AgxFormat::None, 0);
            AgxAppendValue(commentObject, "TargetX", CleanUpDecimals(QString("%1").arg(target.x()/ SFBGSxScalar,0, 'f', 5)), AgxFormat::None, 0);
            AgxAppendValue(commentObject, "TargetY", CleanUpDecimals(QString("%1").arg(target.y()/ SFBGSyScalar,0, 'f', 5)), AgxFormat::None, 0);
        }
    }
    
    FormatBasicPropertySheet(parent, _PropertyEntries);

    for (auto& blockKey : _BlockOrder)
    {
        FormatPropertyBlock(parent, _PropertyBlocks[blockKey]);
    }

}

void AgxGraphModel::loadNode(QJsonObject const& nodeJson)
{
    // Possibility of the id clash when reading it from json and not generating a
    // new value.
    // 1. When restoring a scene from a file.
    // Conflict is not possible because the scene must be cleared by the time of
    // loading.
    // 2. When undoing the deletion command.  Conflict is not possible
    // because all the new ids were created past the removed nodes.
    AgxNodeId restoredNodeId = nodeJson["id"].toVariant().toUInt();

    //_nextNodeId = max(_nextNodeId, restoredNodeId + 1);

    QJsonObject const internalDataJson = nodeJson["internal-data"].toObject();

    QString delegateModelName = internalDataJson["model-name"].toString();

    std::unique_ptr<AgxNode> model = _registry->create(delegateModelName, this);

    if (model) {

        model->SetUpNode(_gameType);
        model->SetNodeIdRef(restoredNodeId);
        Q_EMIT model->ParentGraphTypeUpdated(_graphType);

        connect(model.get(),
            &AgxNode::dataUpdated,
            [restoredNodeId, this](AgxPortIndex const portIndex) {
                onOutPortDataUpdated(restoredNodeId, portIndex);
            });

        connect(model.get(),
            &AgxNode::portsAboutToBeDeleted,
            this,
            [restoredNodeId,
            this](AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last) {
                portsAboutToBeDeleted(restoredNodeId, portType, first, last);
            });

        connect(model.get(),
            &AgxNode::portsDeleted,
            this,
            &AgxGraphModel::portsDeleted);

        connect(model.get(),
            &AgxNode::portsAboutToBeInserted,
            this,
            [restoredNodeId,
            this](AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last) {
                portsAboutToBeInserted(restoredNodeId, portType, first, last);
            });

        connect(model.get(),
            &AgxNode::portsInserted,
            this,
            &AgxGraphModel::portsInserted);

        _models[restoredNodeId] = std::move(model);

        Q_EMIT nodeCreated(restoredNodeId);

        //setNodeData(restoredNodeId, AgxNodeRole::InPortCount, nodeJson["inPortCount"].toVariant().toUInt());

        //setNodeData(restoredNodeId, AgxNodeRole::OutPortCount, nodeJson["outPortCount"].toVariant().toUInt());

        QJsonObject posJson = nodeJson["position"].toObject();
        QPointF const pos(posJson["x"].toDouble(), posJson["y"].toDouble());

        setNodeData(restoredNodeId, AgxNodeRole::Position, pos);

        if (nodeJson.contains("size"))
        {
            QJsonObject nodeSize = nodeJson["size"].toObject();
            QSize size(nodeSize["x"].toInt(), nodeSize["y"].toInt());
            if (size.isValid() && size.height() > 0 && size.width() > 0)
            {
                setNodeData(restoredNodeId, AgxNodeRole::Size, size);
            }
        }

        _models[restoredNodeId]->load(internalDataJson);

        for (auto& key : allNodeIds()) {
            if (_models[key] && _models[key]->AltState() && key != restoredNodeId) {
                _models[restoredNodeId]->SetAltState(false);
                break;
            }
        }
        Q_EMIT nodeUpdated(restoredNodeId);
    }
    else {
        throw std::logic_error(std::string("No registered model with name ")
            + delegateModelName.toLocal8Bit().data());
    }
}

void AgxGraphModel::load(QJsonObject const& jsonDocument)
{
    if (jsonDocument.contains("game-type")) _gameType = AgxGameTypeFromString(jsonDocument["game-type"].toString());
    else _gameType = AgxGameType::None;

    if (jsonDocument.contains("graph-category")) _category = jsonDocument["graph-category"].toString();
    else _category = "NONE";

    AgxGraphType type;
    if (jsonDocument.contains("graph-type")) {
        type = static_cast<AgxGraphType>(jsonDocument.value("graph-type").toString("0").toUInt());
    } else {
        type = AgxGetDefaultGraphType(_gameType);
    }

    SetNewGraphProperties(type);

    QCoreApplication::processEvents();

    if (jsonDocument.contains("properties")) {

        insertPropertySheetData(jsonDocument["properties"].toObject());

    }

    QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

    for (QJsonValueRef nodeJson : nodesJsonArray) {
        loadNode(nodeJson.toObject());
    }

    QJsonArray groupJsonArray = jsonDocument["groups"].toArray();

    for (QJsonValueRef groupJson : groupJsonArray)
    {
        QString groupId = groupJson.toObject().value("groupId").toString();
        QColor color = JsonObjectToColor(groupJson.toObject().value("groupColor").toObject());
        if (!GroupExists(groupId))
        {
            CreateNodeGroup(groupId, color);
        }
    }

    QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

    for (QJsonValueRef connection : connectionJsonArray) {
        QJsonObject connJson = connection.toObject();

        AgxConnectionId connId = fromJson(connJson);

        // Restore the connection
        addConnection(connId);
    }

    QCoreApplication::processEvents();
}

void AgxGraphModel::load(pugi::xml_node& xmlNode)
{
    size_t contentSize = 0;
    size_t contentProc = 0;
    for (auto node : xmlNode.children("node"))
    {
        contentSize++;
    }

    if (_rootReference == this) {
        if (xmlNode.child("Category")) {
            _category = xmlNode.child_value("Category");
            xmlNode.remove_child("Category");
        }
        if (xmlNode.child("Name")) {
            _graphRelDataPath = xmlNode.child_value("Name");
            xmlNode.remove_child("Name");

            QFileInfo relFile(_graphRelDataPath);
            SetGraphTitle(relFile.baseName());
        }
    }

    Q_EMIT statusUpdate(0.1f);

    if (xmlNode.child("Link_Style")) {
        //blockSignals(true);
        if (_stricmp(xmlNode.child_value("Link_Style"), "0") == 0) SetNewGraphProperties(AgxGraphType::SFBGS_Default);
        if (_stricmp(xmlNode.child_value("Link_Style"), "1") == 0) SetNewGraphProperties(AgxGraphType::SFBGS_StateMachine);
        //xmlNode.remove_child("Link_Style");
        //blockSignals(false);
    } else qDebug() << "ERROR ON GRAPH READ. MISSING LINK STYLE!";

    //the index based list of nodes, each connection is hashed with the input index
    QList<QMap<QString, pugi::xml_node>> xmlConnections;

    for (auto& node : xmlNode.children("node")) {
        QMap<QString, pugi::xml_node> nodeConnections;
        for (auto& input : node.children("input")) {
            if (input.child("link")) {
                nodeConnections[input.child_value("idx")] = input.child("link");
                //input.remove_child("link");
            }
        }
        xmlConnections.append(nodeConnections);
    }

    Q_EMIT statusUpdate(0.15f);

    for (auto& node : xmlNode.children("node")) 
    {
        QString nodeTypeStr = node.child_value("node_type");
        nodeTypeStr = nodeTypeStr == "NT_STATE_MACHINE_EMBEDDED" || nodeTypeStr == "NT_BLEND_TREE_EMBEDDED" ? "NT_EMBEDDED_GRAPH" : nodeTypeStr;
        auto nodeId = addNode(nodeTypeStr);

        if (nodeId == InvalidNodeId) continue;

        if (auto agxNode = _models[nodeId].get()) {
            QString xpos = node.child("pos_x") ? node.child_value("pos_x") : "0";
            QString ypos = node.child("pos_y") ? node.child_value("pos_y") : "0";

            QPointF pos(SFBGSxScalar *xpos.toDouble(), SFBGSyScalar *ypos.toDouble());
            setNodeData(nodeId, AgxNodeRole::Position, pos);

            auto connectionToNode = QObject::connect(agxNode, &AgxNode::statusUpdate, [this, contentSize, contentProc](float loadPercentage, const QString& message) { 
                Q_EMIT statusUpdate(0.15 + 0.45*(static_cast<float>(contentProc) + loadPercentage) / static_cast<float>(contentSize), message); 
                                                     });
            
            agxNode->load(node);
            QObject::disconnect(connectionToNode);
            
            Q_EMIT nodeUpdated(nodeId);
        } else qDebug() << "ERROR ON NODE READ: " << node.child_value("node_type");

        contentProc++;
    }

    Q_EMIT statusUpdate(0.6f);

    for (auto& xmlComment : xmlNode.children("comment")) {
        auto nodeId = addNode("COMMENT");
        if (auto cNode = dynamic_cast<AgxCommentNode*>(_models[nodeId].get())) {
            if (xmlComment.child("text")) {
                cNode->_text = xmlComment.child_value("text");
                //xmlComment.remove_child("text");
            }
            {
                QPointF targetF;
                if (xmlComment.child("TargetX")) {
                    bool ok = false;
                    cNode->_hasTarget = true;
                    QString x = xmlComment.child_value("TargetX");
                    auto value = x.toDouble(&ok);
                    value = ok ? value : 0.0;
                    targetF.setX(SFBGSxScalar *value);
                    //xmlComment.remove_child("TargetX");
                }
                if (xmlComment.child("TargetY")) {
                    bool ok = false;
                    cNode->_hasTarget = true;
                    QString y = xmlComment.child_value("TargetY");
                    auto value = y.toDouble(&ok);
                    value = ok ? value : 0.0;
                    targetF.setY(SFBGSyScalar *value);
                    //xmlComment.remove_child("TargetY");
                }
                cNode->_target = targetF;
            }
            {
                QPointF posF;
                if (xmlComment.child("X")) {
                    bool ok = false;
                    QString x = xmlComment.child_value("X");
                    auto value = x.toDouble(&ok);
                    value = ok ? value : 0.0;
                    posF.setX(SFBGSxScalar*value);
                    //xmlComment.remove_child("X");
                }
                if (xmlComment.child("Y")) {
                    bool ok = false;
                    QString y = xmlComment.child_value("Y");
                    auto value = y.toDouble(&ok);
                    value = ok ? value : 0.0;
                    posF.setY(SFBGSyScalar *value);
                    //xmlComment.remove_child("Y");
                }
                setNodeData(nodeId, AgxNodeRole::Position, posF);
                Q_EMIT cNode->PropertySheetUpdated();
                Q_EMIT nodeUpdated(nodeId);
            }

        }
    }

    Q_EMIT statusUpdate(0.7f);

    for (AgxNodeId id = 0; id < xmlConnections.size(); id++) {
        auto keys = xmlConnections.at(id).keys();
        for (auto& key : keys) {
            if (xmlConnections.at(id)[key].empty()) continue;
            AgxConnectionId cId;
            cId.inNodeId = id;
            auto cXml = xmlConnections.at(id)[key];
            QString hiddenStr = cXml.child_value("hidden");
            cId.isHidden = hiddenStr == "True" ? true : false;
            QString oNode = cXml.child_value("node");
            QString oPort = cXml.child_value("output");
            cId.inPortIndex = key.toUInt();
            cId.outNodeId = oNode.toUInt() > 0 ? oNode.toUInt() - 1 : 0;
            cId.outPortIndex = oPort.toUInt();
            addConnection(cId);
        }
    }
    size_t grpCount = 1;
    for (auto& groupXml : xmlNode.children("node_group")) {
        QString NameStr = groupXml.child_value("name");
        NameStr = NameStr.isEmpty() ? QString("Import Group %1").arg(grpCount) : NameStr;
        CreateNodeGroup(NameStr);
        for (auto& nodeRef : groupXml.children("node")) {
            QString nIdx = nodeRef.child_value("idx");
            bool ok = false;
            unsigned int idx = nIdx.toUInt(&ok);
            if (ok) {
                AddToNodeGroup(idx-1, NameStr);
            }
        }
    }
    
    unsigned int blockCount = 0;

    for (auto& pEntry : _PropertyEntries) {
        pEntry.SetIsPresent(false);
    }

    Q_EMIT statusUpdate(0.85f);

    for (auto& propertySheet : xmlNode.children("property_sheet")) {
        if (_stricmp(propertySheet.child("column").child_value("header"), "Property") == 0) {

            for (auto& row : propertySheet.children("row")) {
                auto prop = row.first_child();
                auto value = row.last_child();
                for (auto& entry : _PropertyEntries) {
                    if (entry.Tag() == prop.child_value("value")) {
                        entry.value = value.child_value("value");
                        entry.SetIsPresent(true);
                        //propertySheet.remove_child(row);
                        break;
                    }
                }
            }

            //xmlNode.remove_child(propertySheet);
        } else {
            blockCount++;
            if (blockCount <= _PropertyBlocks.size()) 
            {
                if (_BlockOrder.isEmpty()) _BlockOrder = _PropertyBlocks.keys();

                if (blockCount >= 1) {

                    auto blockKey = _BlockOrder.at(blockCount - 1);
                    auto& block = _PropertyBlocks[blockKey];

                    block.load(propertySheet);

                    Q_EMIT block.DataUpdated();
                }
            }
        }
    }

    Q_EMIT statusUpdate(1.0);

    QApplication::processEvents();
}

void AgxGraphModel::sendConnectionCreation(AgxConnectionId const connectionId)
{
    Q_EMIT connectionCreated(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end()) {
        auto& modeli = iti->second;
        auto& modelo = ito->second;
        modeli->inputConnectionCreated(connectionId);
        modelo->outputConnectionCreated(connectionId);
    }
}

void AgxGraphModel::sendConnectionDeletion(AgxConnectionId const connectionId)
{
    Q_EMIT connectionDeleted(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end()) {
        auto& modeli = iti->second;
        auto& modelo = ito->second;
        modeli->inputConnectionDeleted(connectionId);
        modelo->outputConnectionDeleted(connectionId);
    }
}

void AgxGraphModel::portsAboutToBeDeleted(AgxNodeId const nodeId, AgxPortType const portType, AgxPortIndex const first, AgxPortIndex const last)
{
    _shiftedByDynamicPortsConnections.clear();

    auto portCountRole = portType == AgxPortType::In ? AgxNodeRole::InPortCount : AgxNodeRole::OutPortCount;

    unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

    if (first > portCount - 1)
        return;

    if (last < first)
        return;

    auto clampedLast = qMin(last, portCount - 1);

    for (AgxPortIndex portIndex = first; portIndex <= clampedLast; ++portIndex) {
        std::unordered_set<AgxConnectionId> conns = connections(nodeId, portType, portIndex);

        for (auto& connectionId : conns) {
            deleteConnection(connectionId);
        }
    }

    size_t const nRemovedPorts = clampedLast - first + 1;

    for (AgxPortIndex portIndex = clampedLast + 1; portIndex < portCount; ++portIndex) {
        std::unordered_set<AgxConnectionId> conns = connections(nodeId, portType, portIndex);

        for (auto& connectionId : conns) {
            // Erases the information about the port on one side;
            auto c = makeIncompleteConnectionId(connectionId, portType);

            c = makeCompleteConnectionId(c, nodeId, portIndex - nRemovedPorts);

            _shiftedByDynamicPortsConnections.push_back(c);

            deleteConnection(connectionId);
        }
    }

}

void AgxGraphModel::portsDeleted()
{
    for (const auto& connectionId : _shiftedByDynamicPortsConnections) {
        addConnection(connectionId);
    }

    _shiftedByDynamicPortsConnections.clear();
}

void AgxGraphModel::portsAboutToBeInserted(const AgxNodeId& nodeId, const AgxPortType& portType, const AgxPortIndex& first, const AgxPortIndex& last)
{
    _shiftedByDynamicPortsConnections.clear();

    auto portCountRole = portType == AgxPortType::In ? AgxNodeRole::InPortCount : AgxNodeRole::OutPortCount;

    unsigned int portCount = nodeData(nodeId, portCountRole).toUInt();

    if (first > portCount)
        return;

    if (last < first)
        return;

    std::size_t const nNewPorts = last - first + 1;

    for (AgxPortIndex portIndex = first; portIndex < portCount; ++portIndex) {
        std::unordered_set<AgxConnectionId> conns = connections(nodeId, portType, portIndex);

        for (auto connectionId : conns) {
            // Erases the information about the port on one side;
            auto c = makeIncompleteConnectionId(connectionId, portType);

            c = makeCompleteConnectionId(c, nodeId, portIndex + nNewPorts);

            _shiftedByDynamicPortsConnections.push_back(c);

            deleteConnection(connectionId);
        }
    }
}

void AgxGraphModel::portsInserted()
{
    for (auto const connectionId : _shiftedByDynamicPortsConnections) {
        addConnection(connectionId);
    }

    _shiftedByDynamicPortsConnections.clear();
}

void AgxGraphModel::propagateEmptyDataTo(AgxNodeId const nodeId, AgxPortIndex const portIndex)
{
    QVariant emptyData{};

    setPortData(nodeId, AgxPortType::In, portIndex, emptyData, AgxPortRole::Data);
}
