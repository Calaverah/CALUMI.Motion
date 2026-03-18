//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNode.h"
#include "Models/AgxGraphicsScene.h"
#include "CALUMIMotion.h"
#include "Utilities/AgxNodeRegistry.h"
#include "Utilities/guidUtil.h"
#include <Utilities/AgxJsonHelper.h>
#include <Models/Blend/AgxBlendInputModel.h>
#include <Widgets/Blend/AgxBlendInputView.h>
#include "Utilities/AgxFormat.h"
#include "Widgets/AgxLineEdit.h"

AgxNode::~AgxNode()
{
    CloseEmbeddedView();
    if (_sidebarContent) _sidebarContent->deleteLater();
}

AgxNode::AgxNode(AgxGraphModel* rootGraphRef) : _nodeStyle(AgxStyleCollection::nodeStyle()), _NodePropertiesWidget(nullptr), _nextPortId{0}, _RootGraphReference(rootGraphRef)
{

}

void AgxNode::SetNameProperty(QString newName)
{
    if (CanSetNameProperty())
    {
        _nameProperty = newName;
        Q_EMIT PropertySheetUpdated();
    }
}

QJsonObject AgxNode::save() const
{
    QJsonObject modelJson;

    modelJson["model-name"] = name();

    if (CanSetNameProperty())
        modelJson["nameProperty"] = _nameProperty;

    modelJson["groupId"] = _groupName;

    QJsonObject propSheet;
    propSheet = getPropertySheetData();

    modelJson["collapsed"] = collapsed;


    if (_EmbeddedGraphModel.get())
    {
        modelJson["embedded-graph"] = _EmbeddedGraphModel->save();
    }

    QJsonObject inPortData;
    QJsonObject outPortData;
    for (unsigned int i = 0; i < _In_Ports.count(); i++)
    {
        inPortData[std::to_string(_In_Ports.at(i)->GetId()).c_str()] = _In_Ports.at(i)->Save();
    }
    for (unsigned int i = 0; i < _Out_Ports.count(); i++)
    {
        outPortData[std::to_string(_Out_Ports.at(i)->GetId()).c_str()] = _Out_Ports.at(i)->Save();
    }

    propSheet["in-ports"] = inPortData;
    propSheet["out-ports"] = outPortData;

    modelJson["property-sheet"] = propSheet;

    return modelJson;
}

void AgxNode::load(QJsonObject const& data)
{
    blockSignals(true);
    
    if (CanSetNameProperty())
    {
        QJsonValue s = data["nameProperty"];
        SetNameProperty(s.toString());
    }
    setGroupId(data["groupId"].toString());

    QJsonObject pSheet = data["property-sheet"].toObject();
    QJsonObject blockData = pSheet["property-blocks"].toObject();

    QJsonObject inPortData = pSheet["in-ports"].toObject();
    QJsonObject outPortData = pSheet["out-ports"].toObject();
    
    for (unsigned int i = 0; i < inPortData.count(); i++)
    {
        auto port = _AddPort(AgxPortType::In);
        port->SetId(inPortData.keys().at(i).toUInt());
    }
    for (unsigned int i = 0; i < outPortData.count(); i++)
    {
        auto port = _AddPort(AgxPortType::Out);
        port->SetId(outPortData.keys().at(i).toUInt());
    }

    for (int i = 0; i < _PropertyBlocks.size(); i++)
    {
        auto key = _PropertyBlocks.keys().at(i);
        _PropertyBlocks[key].loadDefault(blockData[key().tag].toObject());
    }

    insertPropertySheetData(pSheet);

    QJsonValue jCollapse = data["collapsed"];
    if (jCollapse.toBool())
    {
        ToggleCollapse();
    }

    if(_EmbeddedGraphModel.get() && data.contains("embedded-graph"))
    {
        QJsonObject embeddedGraph = data["embedded-graph"].toObject();
        _EmbeddedGraphModel->load(embeddedGraph);
    }
    blockSignals(false);

    Q_EMIT PropertySheetUpdated();
    Q_EMIT embeddedWidgetSizeUpdated();
    QCoreApplication::processEvents();
}

void AgxNode::load(pugi::xml_node& xmlNode)
{
    blockSignals(true);
    if (xmlNode.child("collapsed")) {
        auto clpsd = xmlNode.child("collapsed").child_value();
        if ((_stricmp(clpsd, "True") == 0) != collapsed) {
            ToggleCollapse();
        }
        xmlNode.remove_child("collapsed");
    }

    if (xmlNode.child("name")) {
        _nameProperty = xmlNode.child_value("name");
        xmlNode.remove_child("name");
    }

    if (xmlNode.child("node_type")) {
        
        if (name() != xmlNode.child_value("node_type"))
        {
            AmmendValidationState("Node Type Does Not Match Input", AgxNodeValidationState::State::Warning);
        }
        xmlNode.remove_child("node_type");
    }

    {
        std::ostringstream oss;
        xmlNode.print(oss);
        _ExcessData += oss.str();   
    }

    if(!_ExcessData.isEmpty()) AmmendValidationState("Node Not Fully Imported!", AgxNodeValidationState::State::Warning);
    blockSignals(false);

    Q_EMIT statusUpdate(1.0);
    Q_EMIT PropertySheetUpdated();
    Q_EMIT embeddedWidgetSizeUpdated();
    QApplication::processEvents();
}

AgxConnectionPolicy AgxNode::portConnectionPolicy(AgxPortType portType, AgxPortIndex portIndex) const
{
    auto result = AgxConnectionPolicy::One;
    switch (portType) {
        case AgxPortType::In:
            result = AgxConnectionPolicy::One;
            break;
        case AgxPortType::Out:
            result = AgxConnectionPolicy::Many;
            break;
        case AgxPortType::None:
            break;
    }

    return result;
}

const AgxNodeStyle& AgxNode::nodeStyle() const
{
    return _nodeStyle;
}

void AgxNode::setNodeStyle(const AgxNodeStyle& style) { _nodeStyle = style; }

void AgxNode::AmmendValidationState(const QString& messageToAdd, const AgxNodeValidationState::State& minState)
{
    return;
    auto vState = validationState();
    if (!vState._stateMessage.isEmpty()) vState._stateMessage += "\n";

    switch (minState)
    {
        case AgxNodeValidationState::State::Warning: 
            vState._stateMessage += "WARNING: ";
            break;
        case AgxNodeValidationState::State::Error:
            vState._stateMessage += "ERROR: ";
            break;
    }

    vState._stateMessage += messageToAdd;

    if (vState._state < minState) vState._state = minState;

    setValidatonState(vState);
}

void AgxNode::setValidatonState(const AgxNodeValidationState& validationState)
{}

void AgxNode::InitializeWidget()
{
    //called before any null embedded widget is summoned on child class
    if (!_NodePropertiesWidget)
    {
        _NodePropertiesWidget = new AgxNodePropertiesWidget(nullptr, true);
        connect(this, &AgxNode::PropertySheetUpdated, _NodePropertiesWidget, [this]() {
            _NodePropertiesWidget->SetHiddenEntries(hiddenState);
                });
        //enter standard node widgets here
    }
}

void AgxNode::ToggleCollapse()
{
    collapsed = !collapsed;
}

void AgxNode::insertPropertySheetData(const QJsonObject& data)
{
    //should be called after child node class override is handled
    if (data.contains("hiddenState"))
        hiddenState = data.value("hiddenState").toBool();

    //update standard node data here
    QJsonObject jsonBlock = data["property-blocks"].toObject();
    for (int i = 0; i < _PropertyBlocks.size(); i++)
    {
        auto key = _PropertyBlocks.keys().at(i);
        if (jsonBlock.contains(key().tag))
        {
            _PropertyBlocks[key].insertPropertyBlockData(jsonBlock[key().tag].toObject());
        }
    }

    for (int i = 0; i < _PropertyEntries.size(); i++)
    {
        QString key = _PropertyEntries.at(i).Tag();
        if (data.contains(key))
        {
            if(data[key].toObject().contains("value"))
                _PropertyEntries[i].value = data[key].toObject()["value"].toString();
            if(data[key].toObject().contains("isPresent"))
                _PropertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
        }
    }

    QJsonObject inPortData = data["in-ports"].toObject();
    QJsonObject outPortData = data["out-ports"].toObject();

    for (unsigned int i = 0; i < inPortData.count(); i++)
    {
        QString key = inPortData.keys().at(i);
        //_In_Ports.at(i)->Load(inPortData[key].toObject());

        for (auto port : _In_Ports)
        {
            if (port->GetId() == key.toUInt())
            {
                port->Load(inPortData[key].toObject());
            }
        }
    }
    for (unsigned int i = 0; i < outPortData.count(); i++)
    {
        QString key = outPortData.keys().at(i);
        //_Out_Ports.at(i)->Load(outPortData[key].toObject());
        for (auto port : _Out_Ports)
        {
            if (port->GetId() == key.toUInt())
            {
                port->Load(outPortData[key].toObject());
            }
        }
    }
    
    Q_EMIT PropertySheetUpdated();
    Q_EMIT embeddedWidgetSizeUpdated();
}

QJsonObject AgxNode::getPropertySheetData(bool cleared) const
{
    QJsonObject output;
    //serialize property sheet for parent class

    output["hiddenState"] = QJsonValue(hiddenState);

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
        QJsonObject pair;
        pair["value"] = cleared ? "" : _PropertyEntries[i].value;
        pair["isPresent"] = cleared ? false : _PropertyEntries[i].isPresent;
        output[key] = pair;
    }

    QJsonObject inPortData;
    QJsonObject outPortData;
    for (unsigned int i = 0; i < _In_Ports.count(); i++)
    {
        inPortData[std::to_string(_In_Ports.at(i)->GetId()).c_str()] = _In_Ports.at(i)->Save();
    }
    for (unsigned int i = 0; i < _Out_Ports.count(); i++)
    {
        outPortData[std::to_string(_Out_Ports.at(i)->GetId()).c_str()] = _Out_Ports.at(i)->Save();
    }

    output["in-ports"] = inPortData;
    output["out-ports"] = outPortData;

    return output;
}

AgxPropertyBlockData* AgxNode::getPropertyBlock(TermRef ref) {
    if (!_PropertyBlocks.contains(ref)) return nullptr;

    return &_PropertyBlocks[ref];
}
AgxPropertyBlockData* AgxNode::getPropertyBlock(const QString& block)
{
    for (auto blockTermRef : _PropertyBlocks.keys()) {
        if (blockTermRef().tag.compare(block, Qt::CaseInsensitive) == 0) {
            return &_PropertyBlocks[blockTermRef];
        }
    }

    return nullptr;
}
void AgxNode::SetUpEmbeddedNodeGraph()
{
    _EmbeddedGraphModel = std::make_shared<AgxGraphModel>(_gameType, _RootGraphReference);
    _EmbeddedGraphScene = std::make_shared<AgxGraphicsScene>(*_EmbeddedGraphModel);
}

void AgxNode::CloseEmbeddedView()
{
    if (!_EmbeddedGraphScene.get())
        return;

    auto views = _EmbeddedGraphScene->views();
    AgxGraphicsView* agxView = nullptr;
    for (auto entry : views)
    {
        if (agxView = dynamic_cast<AgxGraphicsView*>(entry))
        {
            break;
        }
    }

    auto topList = QApplication::topLevelWidgets();
    
    for (auto entry : topList)
    {
        if (CALUMIMotion* mainWindow = dynamic_cast<CALUMIMotion*>(entry))
        {
            mainWindow->CloseTab(agxView);
        }
    }

}

QWidget* AgxNode::GetSideBarContent()
{
    return nullptr;

    if (!_sidebarContent)
    {
        
        _sidebarContent = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout();
        _sidebarContent->setLayout(layout);
        QLabel* header = new QLabel(std::format("{} [Node Id: {}]", _nameProperty.toStdString().c_str(),_nodeIdRef).c_str());
        connect(this, &AgxNode::PropertySheetUpdated, this, [this, header]() {header->setText(std::format("{} [Node Id: {}]", _nameProperty.toStdString().c_str(), _nodeIdRef).c_str()); });

        header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout->addWidget(header);

        QFrame* hline = new QFrame();
        hline->setFrameShape(QFrame::HLine);
        hline->setFrameShadow(QFrame::Sunken);
        layout->addWidget(hline);
    }

    return _sidebarContent;
}

void AgxNode::SetSidebarVisibility(bool state)
{
    if (_sidebarContent)
    {
        _sidebarContent->setVisible(state);
        if (auto sfbgsItem = dynamic_cast<AgxSidebarContent*>(_sidebarContent.get()))
        {
            Q_EMIT sfbgsItem->StateChanged();
        }
    }
}


void AgxNode::inputConnectionCreated(const AgxConnectionId& connection)
{
    if (connection.inPortIndex < _In_Ports.count())
    {
        _In_Ports.at(connection.inPortIndex)->setConnectionState(true);
    }
}
void AgxNode::inputConnectionDeleted(const AgxConnectionId& connection)
{
    if (connection.inPortIndex < _In_Ports.count())
    {
        _In_Ports.at(connection.inPortIndex)->setConnectionState(false);
    }
}

std::shared_ptr<AgxPort> AgxNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
{
    unsigned int idxI = index > _In_Ports.count() ? _In_Ports.count() : index;
    unsigned int idxO = index > _Out_Ports.count() ? _Out_Ports.count() : index;

    auto agxPort = std::make_shared<AgxPort>(this);

    switch (portType)
    {
        case AgxPortType::In:
            _In_Ports.insert(idxI, agxPort);
            break;
        case AgxPortType::Out:
            _Out_Ports.insert(idxO, agxPort);
            break;
        default:
            return nullptr;
            break;
    }

    agxPort->Load(data);

    if (agxPort->GetId() == InvalidAgxPortId)
    {
        agxPort->SetId(NewPortId());
    }

    _nextPortId = agxPort->GetId() >= _nextPortId ? agxPort->GetId() + 1 : _nextPortId;

    return agxPort;
}

void AgxNode::_RemovePort(AgxPortType portType, AgxPortIndex index, bool preserve)
{
    //choosing to leave the possibility of both incase we switch to AgxPortType
    unsigned int idxI = index >= _In_Ports.count() ? _In_Ports.count() - 1 : index;
    unsigned int idxO = index >= _Out_Ports.count() ? _Out_Ports.count() - 1 : index;

    if (preserve)
    {
        switch (portType)
        {
            case AgxPortType::In:
                if (auto agxPort = dynamic_cast<AgxPort_SFBGS*>(_In_Ports.at(idxI).get())) {
                    if (agxPort->GetId() == _nextPortId - 1)
                        _nextPortId--;
                }
                break;
            case AgxPortType::Out:
                if (auto agxPort = dynamic_cast<AgxPort_SFBGS*>(_Out_Ports.at(idxO).get())) {
                    if (agxPort->GetId() == _nextPortId - 1)
                        _nextPortId--;
                }
                break;
        }
    }
    
    switch (portType)
    {
        case AgxPortType::In:
            _In_Ports.erase(_In_Ports.begin() + idxI);
            break;
        case AgxPortType::Out:
            _Out_Ports.erase(_Out_Ports.begin() + idxO);
            break;
    }
}

QJsonObject AgxNode::PortData(AgxPortType portType, AgxPortIndex index)
{   
    switch (portType)
    {
        case AgxPortType::In:
            if (index < _In_Ports.count())
                return _In_Ports.at(index)->Save();
            break;
        case AgxPortType::Out:
            if (index < _Out_Ports.count())
                return _Out_Ports.at(index)->Save();
            break;
    }
    return QJsonObject();
}

void AgxNode::SetPortData(AgxPortType portType, AgxPortIndex index, const QJsonObject& dataSet)
{
    switch (portType)
    {
        case AgxPortType::In:
            if (index < _In_Ports.count())
                return _In_Ports.at(index)->Load(dataSet);
            break;
        case AgxPortType::Out:
            if (index < _Out_Ports.count())
                return _Out_Ports.at(index)->Load(dataSet);
            break;
    }
}

unsigned int AgxNode::PortCount(AgxPortType portType) const
{
    switch (portType)
    {
        case AgxPortType::In:
            return _In_Ports.count();
        case AgxPortType::Out:
            return _Out_Ports.count();
        default:
            return 0;
    }
}

void AgxNode::SetPortCount(AgxPortType portType, unsigned int count)
{
    unsigned int portCount = 0;

    switch (portType)
    {
        case AgxPortType::In:
            portCount = _In_Ports.count();
            break;
        case AgxPortType::Out:
            portCount = _Out_Ports.count();
            break;
    }
    if (portCount > count)
    {
        auto size = portCount - count;
        for (unsigned int i = 0; i < size; i++)
        {
            _RemovePort(portType, portCount);
            portCount--;
        }
    } else if (portCount < count)
    {
        auto size = portCount - count;
        for (unsigned int i = 0; i < size; i++)
        {
            _AddPort(portType, portCount);
            portCount++;
        }
    }
    
}

AgxPortType AgxNode::GetPortType(AgxPort* port) const
{
    for (unsigned int i = 0; i < _In_Ports.count(); i++)
    {
        if (_In_Ports[i].get() == port) return AgxPortType::In;
    }
    for (unsigned int i = 0; i < _Out_Ports.count(); i++)
    {
        if (_Out_Ports[i].get() == port) return AgxPortType::Out;
    }

    return AgxPortType::None;
}

void AgxNode::_ExternalPortCommand(AgxPortType portType, AgxPortIndex index, const QString& command, const QString& payload)
{
    if (PortCount(portType) <= index) return;

    switch (portType)
    {
        case AgxPortType::In:
            _In_Ports.at(index)->externalCommand(command, payload);
            break;
        case AgxPortType::Out:
            _Out_Ports.at(index)->externalCommand(command, payload);
            break;
    }
}

AgxPortIndex AgxNode::GetPortIndex(AgxPort* port) const
{
    for (unsigned int i = 0; i < _In_Ports.count(); i++)
    {
        if (_In_Ports[i].get() == port) return i;
    }
    for (unsigned int i = 0; i < _Out_Ports.count(); i++)
    {
        if (_Out_Ports[i].get() == port) return i;
    }

    return InvalidPortIndex;
}

QString AgxNode::portCaption(AgxPortType portType, AgxPortIndex idx) const
{
    QString output = "Error";
    if (portType == AgxPortType::In) {
        output = _In_Ports.count() > idx ? _In_Ports.at(idx).get()->Caption() : output;
    } else if (portType == AgxPortType::Out) {
        output = _Out_Ports.count() > idx ? _Out_Ports.at(idx).get()->Caption() : output;
    }
    return output;
}

bool AgxNode::portCaptionVisible(AgxPortType portType, AgxPortIndex idx) const
{
    bool output = false;
    if (portType == AgxPortType::In) {
        output = _In_Ports.count() > idx ? _In_Ports.at(idx).get()->HasCaption() : output;
    } else if (portType == AgxPortType::Out) {
        output = _Out_Ports.count() > idx ? _Out_Ports.at(idx).get()->HasCaption() : output;
    }

    return output;
}

void AgxNode::ResetPorts()
{
    SetPortCount(AgxPortType::In, 0);
    SetPortCount(AgxPortType::Out, 0);
    _nextPortId = 0;
}

void SFBGSNode::save(pugi::xml_node& parent, QVector<AgxConnectionId> connections, QVector<AgxNodeId> sortedIds, QPointF pos)
{
    auto nodeObject = AgxAppend(parent, "node", AgxFormat::NewLine, 0);

    if(_SFBGS_Hidden[&AgxDictionary::DefaultState].second.compare("true", Qt::CaseInsensitive) == 0)
        AgxAppendValue(nodeObject, AgxDictionary::DefaultState().tag, "True", AgxFormat::NewLine);

    AgxAppendValue(nodeObject, AgxDictionary::NodeType().tag, typeName(), {AgxFormat::NewLine, AgxFormat::Indent},1);
    AgxAppendValue(nodeObject, AgxDictionary::noninstanced().tag, _SFBGS_Hidden[&AgxDictionary::noninstanced].second == "-" ? "False" : _SFBGS_Hidden[&AgxDictionary::noninstanced].second, { AgxFormat::NewLine, AgxFormat::Indent }, 1);

    auto flagsValue = _flags.GetValue();
    if (flagsValue != 0) 
    {
        auto flagsObject = AgxAppend(nodeObject, "flags", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        _flags.ToXML(flagsObject);
    }

    if (GetNodeType() == AgxNodeType::NT_BLEND_NODE)
    {
        AgxAppendValue(nodeObject, "divisions", QString("%1").arg(_Divisions), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        for (auto& port : _In_Ports)
        {
            if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
            {
                FormatBlendInput(nodeObject, sfbgsPort->GetBlendInput());
            }
        }
    }
    
    AgxAppendValue(nodeObject, "name", nameProperty(), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::PosX().tag, CleanUpDecimals(QString("%1").arg(pos.x() / SFBGSxScalar, 0, 'f', 5)), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::PosY().tag, CleanUpDecimals(QString("%1").arg(pos.y() / SFBGSyScalar, 0, 'f', 5)), { AgxFormat::NewLine, AgxFormat::Indent }, 1);

    QString eposx = _SFBGS_Hidden[&AgxDictionary::ExpPosX].second == "-" ? CleanUpDecimals(QString("%1").arg(pos.x() / SFBGSxScalar, 0, 'f', 5)) : _SFBGS_Hidden[&AgxDictionary::ExpPosX].second;
    AgxAppendValue(nodeObject, AgxDictionary::ExpPosX().tag, eposx, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    QString eposy = _SFBGS_Hidden[&AgxDictionary::ExpPosY].second == "-" ? CleanUpDecimals(QString("%1").arg(pos.y() / SFBGSyScalar, 0, 'f', 5)) : _SFBGS_Hidden[&AgxDictionary::ExpPosY].second;
    AgxAppendValue(nodeObject, AgxDictionary::ExpPosY().tag, eposy, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    
    AgxAppendValue(nodeObject, AgxDictionary::UseColor().tag, _SFBGS_Hidden[&AgxDictionary::UseColor].second == "-" ? "False" : _SFBGS_Hidden[&AgxDictionary::UseColor].second, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::UserId().tag, GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "0"), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, "collapsed", isCollapsed()? "True" : "False", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, "guid", getGuid().toString(QUuid::WithoutBraces), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    
    for (auto& port : _In_Ports)
    {
        auto input = AgxAppend(nodeObject, "input", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        AgxAppendValue(input, "name", port.get()->Caption(false), AgxFormat::SpaceBefore);
        AgxAppendValue(input, "id", QString("%1").arg(port.get()->GetId()), AgxFormat::SpaceBefore);
        AgxAppendValue(input, "idx", QString("%1").arg(port.get()->GetPortIndex()), AgxFormat::SpaceBefore);
        input.append_child(pugi::node_pcdata).set_value("\n");

        
        if(!port.get()->isConnected()) 
        {
            char lineEnd[2] = { 0x20, '\0' };
            input.append_child(pugi::node_pcdata).set_value(lineEnd);
        } 
        else 
        {
            for (auto& connection : connections)
            {
                if (connection.inPortIndex != port.get()->GetPortIndex() || connection.inNodeId != _nodeIdRef) continue;

                auto link = AgxAppend(input, "link", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
                AgxAppendValue(link, "node", QString("%1").arg(sortedIds.indexOf(connection.outNodeId) + 1), { AgxFormat::NewLine, AgxFormat::Indent }, 3);
                AgxAppendValue(link, "output", QString("%1").arg(connection.outPortIndex), AgxFormat::SpaceBefore);
                AgxAppendValue(link, "hidden", connection.isHidden ? "True" : "False", AgxFormat::SpaceBefore);
                port.get()->SavePropertySheet(link);
                AgxCloseNode(link, false, true, 2);
                char lineEnd[3] = { 0x0A,0x20,'\0' };
                input.append_child(pugi::node_pcdata).set_value(lineEnd);

            }
        }
    }

    for (auto& port : _Out_Ports)
    {
        auto output = AgxAppend(nodeObject, "output", { AgxFormat::NewLine, AgxFormat::Indent }, 1);

        AgxAppendValue(output, "name", port.get()->Caption(false), AgxFormat::SpaceBefore);
        AgxAppendValue(output, "id", QString("%1").arg(port.get()->GetId()), AgxFormat::SpaceBefore);
        AgxAppendValue(output, "idx", QString("%1").arg(port.get()->GetPortIndex()), AgxFormat::SpaceBefore);
        output.append_child(pugi::node_pcdata).set_value("\n");
        output.append_child(pugi::node_pcdata).set_value(" ");
    }

    FormatBasicPropertySheet(nodeObject, _PropertyEntries);

    for (auto& blockKey : _BlockOrder)
    {
        if (!_PropertyBlocks[blockKey].IsEnabledState()) continue;
        
        FormatPropertyBlock(nodeObject, _PropertyBlocks[blockKey]);
    }

    if (_EmbeddedGraphModel.get())
    {
        auto graphObject = AgxAppend(nodeObject, "graph", AgxFormat::NewLine, 0);
        _EmbeddedGraphModel.get()->save(graphObject);
        AgxCloseNode(graphObject, false, false, 0);
    }

    AgxCloseNode(nodeObject, false, false);
}

QJsonObject SFBGSNode::save() const
{
    QJsonObject output;
    QJsonObject agxNode = AgxNode::save();
    output = JsonMerge(output, agxNode);

    for (int i = 0; i < _SFBGS_Hidden.size(); i++)
    {
        auto key = _SFBGS_Hidden.keys().at(i);
        output[key().tag] = _SFBGS_Hidden[key].second;
    }

    for (int i = 0; i < _SFBGS_Properties.size(); i++)
    {
        QString key = _SFBGS_Properties.at(i).Tag();
        output[key] = _SFBGS_Properties[i].value;
    }

    if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) {
        output["divisions"] = std::to_string(_Divisions).c_str();
    }

    output["guid"] = getGuid().toString(QUuid::StringFormat::WithoutBraces);

    output["flags"] = _flags.ToJson();    

    QJsonObject userIdVal;
    userIdVal["value"] = GetPropertyValue(_SFBGS_Properties, AgxDictionary::GetInstance().UserId().tag);
    output[AgxDictionary::GetInstance().UserId().tag] = userIdVal;

    return output;
}

void SFBGSNode::load(QJsonObject const& data)
{
    AgxNode::load(data);

    _flags.FromJson(data["flags"]);

    for (int i = 0; i < _SFBGS_Hidden.size(); i++)
    {
        auto key = _SFBGS_Hidden.keys().at(i);
        if (data.contains(key().tag) && key().tag != "guid")
            _SFBGS_Hidden[key].second = data.value(key().tag).toString();
    }

    for (int i = 0; i < _SFBGS_Properties.size(); i++)
    {
        QString key = _SFBGS_Properties.at(i).Tag();
        if(data.contains(key) && key != "guid")
            _SFBGS_Properties[i].value = data.value(key).toObject().value("value").toString();
    }

    if (data.contains("guid"))
    {
        setGuid(data.value("guid").toString());
    }

}

void SFBGSNode::load(pugi::xml_node& xmlNode)
{
    blockSignals(true);
    if (xmlNode.child("expanded_pos_x")) {
        _SFBGS_Hidden[&AgxDictionary::ExpPosX].second = xmlNode.child_value("expanded_pos_x");
        xmlNode.remove_child("expanded_pos_x");
    }
    if (xmlNode.child("expanded_pos_y")) {
        _SFBGS_Hidden[&AgxDictionary::ExpPosY].second = xmlNode.child_value("expanded_pos_y");
        xmlNode.remove_child("expanded_pos_y");
    }
    if (xmlNode.child("pos_x")) {
        _SFBGS_Hidden[&AgxDictionary::PosX].second = xmlNode.child_value("pos_x");
        xmlNode.remove_child("pos_x");
    }
    if(xmlNode.child("pos_y")) {
        _SFBGS_Hidden[&AgxDictionary::PosY].second = xmlNode.child_value("pos_y");
        xmlNode.remove_child("pos_y");
    }

    if (xmlNode.child("divisions")) {
        bool ok = false;
        int divRes = QString(xmlNode.child_value("divisions")).toInt(&ok);
        divRes = ok ? divRes : 0;
        _Divisions = divRes;
    }

    if (xmlNode.child("flags")) {
        auto flags = xmlNode.child("flags");
        _flags.FromXML(flags);
        if(flags.first_child()) {
            AmmendValidationState("Flags Not Handled!", AgxNodeValidationState::State::Warning);
        }
        xmlNode.remove_child("flags");
    }

    blockSignals(false);
    Q_EMIT statusUpdate(0.05f);
    blockSignals(true);

    if (xmlNode.child("noninstanced"))
    {
        QString nIns = xmlNode.child_value("noninstanced");
        _SFBGS_Hidden[&AgxDictionary::noninstanced].second = nIns == "True" ? "True" : "False";
        if (nIns != "True" && nIns != "False")
            AmmendValidationState(std::format("Non Instanced Disparity: {}", nIns.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        xmlNode.remove_child("noninstanced");
    }else { AmmendValidationState("NonInstanced Not Found!", AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("use_color_2"))
    {
        QString c2Str = xmlNode.child_value("use_color_2");
        _SFBGS_Hidden[&AgxDictionary::UseColor].second = c2Str.compare("True",Qt::CaseInsensitive) == 0 ? "True" : "False";
        if (c2Str.compare("True",Qt::CaseInsensitive) != 0 && c2Str.compare("False",Qt::CaseInsensitive) != 0)
            AmmendValidationState(std::format("Use Color Disparity: {}", c2Str.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        xmlNode.remove_child("use_color_2");
    } else { AmmendValidationState("use_color_2 Not Found!", AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("default_state"))
    {
        QString state = xmlNode.child_value("default_state");
        _SFBGS_Hidden[&AgxDictionary::DefaultState].second = state.compare("True", Qt::CaseInsensitive) == 0 ? "True" : "False";
        if (state != "True" && state != "False")
            AmmendValidationState(std::format("Default State Disparity: {}", state.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        
        xmlNode.remove_child("default_state");
    } else _SFBGS_Hidden[&AgxDictionary::DefaultState].second = "NONE";

    if (xmlNode.child("user_id"))
    {
        QString uIdStr = xmlNode.child_value("user_id");
        bool ok = false;
        auto value = uIdStr.toInt(&ok);
        if (!ok)
        {
            value = 0;
            AmmendValidationState(std::format("UserId Disparity: {}", uIdStr.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        }
        SetPropertyValue(_SFBGS_Properties, AgxDictionary::GetInstance().UserId().tag, QString("%1").arg(value));
        
        xmlNode.remove_child("user_id");
    } else { AmmendValidationState(tr("User Id Not Found!"), AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("guid"))
    {
        QUuid guidOld = getGuid();
        QUuid guidNew;
        setGuid(guidNew.fromString(xmlNode.child_value("guid")));
        if (getGuid() == guidOld)
            AmmendValidationState("GUID NOT SET!", AgxNodeValidationState::State::Error);
        xmlNode.remove_child("guid");
    }

    

    if (GetNodeType() == AgxNodeType::NT_BLEND_TREE_EMBEDDED || GetNodeType() == AgxNodeType::NT_STATE_MACHINE_EMBEDDED) {
        if (!xmlNode.child("graph")) AmmendValidationState("EMBEDDED TYPE HAS NO GRAPH!", AgxNodeValidationState::State::Error);
        auto xmlGraph = xmlNode.child("graph");
        if (_EmbeddedGraphModel) {

            auto connectionToGraph = QObject::connect(_EmbeddedGraphModel.get(), &AgxGraphModel::statusUpdate, this, [this](float loadPercentage, const QString& message) { Q_EMIT statusUpdate(0.2 + 0.4 * loadPercentage, message); });

            blockSignals(false);
            Q_EMIT statusUpdate(0.2f, "Loading Embedded Graph Node");
            blockSignals(true);

            _EmbeddedGraphModel->load(xmlGraph);
            
            QObject::disconnect(connectionToGraph);
            
        }
        xmlNode.remove_child("graph");
    } else {
        blockSignals(false);
        Q_EMIT statusUpdate(0.2f);
        blockSignals(true);

        if (xmlNode.child("graph")) AmmendValidationState("NON EMBEDDED TYPE HAS GRAPH!", AgxNodeValidationState::State::Error);
    }

    ResetPorts();
    AgxPortId maxPortId = 0;

    //input->point->"weight,range"
    QList<QList<QPair<QString, QString>>> blendPointData;
    for (auto& xmlBlend : xmlNode.children("blendInput")) {
        QList<QPair<QString, QString>> pointsToAdd;
        for (auto& xmlPoint : xmlBlend.children("point")) {
            pointsToAdd.append(QPair{ xmlPoint.child_value("Range"), xmlPoint.child_value("Weight") });
        }
        blendPointData.append(pointsToAdd);
    }
    while(xmlNode.remove_child("blendInput"));

    for (auto& inputNode : xmlNode.children("input")) {

        QJsonObject jportData;

        jportData["agxPortId"] = inputNode.child_value("id");

        auto port = _AddPort(AgxPortType::In, _In_Ports.size(), jportData);

        port->blockSignals(true);
        {
            bool ok = false;
            auto idx = QString(inputNode.child_value("idx")).toUInt(&ok);
            if (ok && idx != GetPortIndex(port.get()))
            {
                AmmendValidationState("Port Index Mismatch!", AgxNodeValidationState::State::Error);
            }
        }
        
        if (maxPortId <= port->GetId()) maxPortId = port->GetId() + 1;
        
        if (auto agxPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if(inputNode.child("name"))
                agxPort->SetName(inputNode.child_value("name"));
            else
                AmmendValidationState("Missing InPort Name!", AgxNodeValidationState::State::Warning);

            { //blend input data transfer
                auto portIdx = GetPortIndex(port.get());
                auto blendModel = agxPort->GetBlendInput();
                if (portIdx < blendPointData.size() && blendModel) {
                    for (auto& inBModel : blendPointData.at(portIdx)) {
                        blendModel->addDataRow(blendModel->rowCount(), inBModel.first, inBModel.second);
                    }
                }
            }

        }
        port->Load(inputNode);

        inputNode.remove_child("id");
        inputNode.remove_child("idx");
        inputNode.remove_child("name");

        //xmlNode.remove_child(inputNode);
        //Q_EMIT dataUpdated(GetPortIndex(port.get()));
        port->blockSignals(false);
        Q_EMIT port->PropertySheetUpdated();
    }
    blockSignals(false);
    Q_EMIT statusUpdate(0.7f);
    blockSignals(true);

    for (auto& outputNode : xmlNode.children("output"))
    {
        auto port = _AddPort(AgxPortType::Out, _Out_Ports.size());
        {
            bool ok = false;
            auto id = QString(outputNode.child_value("id")).toUInt(&ok);
            if (ok)
                port->SetId(id);
        }
        {
            bool ok = false;
            auto idx = QString(outputNode.child_value("idx")).toUInt(&ok);
            if (ok && idx != GetPortIndex(port.get()))
            {
                AmmendValidationState("Port Index Mismatch!", AgxNodeValidationState::State::Error);
            }
        }
        
        if (maxPortId <= port->GetId()) maxPortId = port->GetId() + 1;

        if (auto agxPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (outputNode.child("name"))
                agxPort->SetName(outputNode.child_value("name"));
            else
                AmmendValidationState("Missing OutPort Name!", AgxNodeValidationState::State::Warning);
        }
        outputNode.remove_child("id");
        outputNode.remove_child("idx");
        outputNode.remove_child("name");

        //xmlNode.remove_child(outputNode);
    }
    blockSignals(false);
    Q_EMIT statusUpdate(0.8f);
    blockSignals(true);

    unsigned int blockCount = 0;

    for (auto& pEntry : _PropertyEntries) {
        pEntry.SetIsPresent(false);
    }

    for(auto& propertySheet : xmlNode.children("property_sheet")) {
        if (_stricmp(propertySheet.child("column").child_value("header"), "Property") == 0) {
            if (_stricmp(propertySheet.child_value("num_columns"), "2") == 0) AmmendValidationState("Main Property Sheet Disparity!", AgxNodeValidationState::State::Error);

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

            if(propertySheet.child("row")) AmmendValidationState("Rows Not Read In Main Property Sheet", AgxNodeValidationState::State::Error);
            //xmlNode.remove_child(propertySheet);
        }
        else {
            blockCount++;
            if (blockCount > _PropertyBlocks.size()) AmmendValidationState("Block Count Mismatch", AgxNodeValidationState::State::Error);
            else {
                auto blockKey = _BlockOrder.at(blockCount - 1);
                auto& block = _PropertyBlocks[blockKey];
                if (_stricmp(propertySheet.child_value("num_columns"), std::to_string(block.GetColumnCount()).c_str()) != 0) AmmendValidationState("Block Column Count Mismatch", AgxNodeValidationState::State::Error);

                block.load(propertySheet);
                //for (auto& col : propertySheet.children("column")) propertySheet.remove_child(col);
                //propertySheet.remove_child("num_columns");

                if(!propertySheet.empty()) AmmendValidationState("Property Sheet Disparity", AgxNodeValidationState::State::Error);

                Q_EMIT block.DataUpdated();
            }
        }
    }

    blockSignals(false);
    Q_EMIT statusUpdate(0.9f);
    blockSignals(true);

    _nextPortId = maxPortId;

    blockSignals(false);
    AgxNode::load(xmlNode);
}

void SFBGSNode::insertPropertySheetData(const QJsonObject& data)
{
    //Hidden SFBGS Entries

    for (int i = 0; i < _SFBGS_Hidden.size(); i++)
    {
        auto key = _SFBGS_Hidden.keys().at(i);
        if (data.contains(key().tag) && key().tag != "guid")
            _SFBGS_Hidden[key].second = data.value(key().tag).toString();
    }

    /*if (data.contains("POS X")) {
        _SFBGS_Hidden["POS X"].second = data["POS X"].toString();
    }
    if (data.contains("Expanded POS X")) {
        _SFBGS_Hidden["Expanded POS X"].second = data["Expanded POS X"].toString();
    }
    if (data.contains("POS Y")) {
        _SFBGS_Hidden["POS Y"].second = data["POS Y"].toString();
    }
    if (data.contains("Expanded POS Y")) {
        _SFBGS_Hidden["Expanded POS Y"].second = data["Expanded POS Y"].toString();
    }
    if (data.contains("Non Instanced")) {
        _SFBGS_Hidden["Non Instanced"].second = data["Non Instanced"].toString();
    }
    if (data.contains("Use Color")) {
        _SFBGS_Hidden["Use Color"].second = data["Use Color"].toString();
    }
    if (data.contains("Default State")) {
        _SFBGS_Hidden["Default State"].second = data["Default State"].toString();
    }*/



    if (data.contains(AgxDictionary::UserId().tag)) {
        SetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, data.value(AgxDictionary::UserId().tag).toObject().value("value").toString());
    }

    //SFBGS Entries
    if (data.contains("flags"))
        _flags.FromJson(data["flags"]);

    _Divisions = data["divisions"].toInt();

    AgxNode::insertPropertySheetData(data);
}

QJsonObject SFBGSNode::getPropertySheetData(bool cleared) const
{
    QJsonObject output = AgxNode::getPropertySheetData(cleared);

    output["divisions"] = cleared ? 0 : _Divisions;

    output["flags"] = _flags.ToJson();

    output[AgxDictionary::UserId().tag] = GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag);

    //output["Non Instanced"] = _SFBGS_Hidden["Non Instanced"].second;
    //output["Use Color"] = _SFBGS_Hidden["Use Color"].second;
    //output["Default State"] = _SFBGS_Hidden["Default State"].second;
    
    QJsonObject userIdObj;
    userIdObj["value"] = GetPropertyValue(_SFBGS_Properties, AgxDictionary::GetInstance().UserId().tag);
    output[AgxDictionary::GetInstance().UserId().tag] = userIdObj;
    
    //output["POS X"] = _SFBGS_Hidden["POS X"].second;
    //output["POS Y"] = _SFBGS_Hidden["POS Y"].second;
    //output["Expanded POS X"] = _SFBGS_Hidden["Expanded POS X"].second;
    //output["Expanded POS Y"] = _SFBGS_Hidden["Expanded POS Y"].second;

    for (int i = 0; i < _SFBGS_Hidden.size(); i++)
    {
        auto key = _SFBGS_Hidden.keys().at(i);
        if (key().tag != "guid" && key != &AgxDictionary::NodeType)
            output[key().tag] = _SFBGS_Hidden[key].second;
    }

    return output;
}

QString SFBGSNode::SubCaption() const
{
    return QString("%1 (%2)").arg(GetPropertyValue(_PropertyEntries, AgxDictionary::Name().tag, "-")).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
}

bool SFBGSNode::AltState()
{
    return _SFBGS_Hidden[&AgxDictionary::DefaultState].second == "True";
}

void SFBGSNode::SetAltState(bool enabled)
{
    _SFBGS_Hidden[&AgxDictionary::DefaultState].second = enabled ? "True" : "-";
    Q_EMIT PropertySheetUpdated();
}

SFBGSNode::SFBGSNode(AgxGraphModel* rootGraphReference) : guidObject(), AgxNode(rootGraphReference) {
    {
        AgxPropertyBlockData enterBlockDef({
            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::NonInstanced,"False",AgxColumnTypes::BasicBool),
            AgxPropertyEntryDefinition(&AgxDictionary::x_EnterEvents_x,"",AgxColumnTypes::Event),
            AgxPropertyEntryDefinition(&AgxDictionary::Payload,"",AgxColumnTypes::BasicString) }, nullptr);
        AgxPropertyBlockData exitBlockDef({
            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::NonInstanced,"False",AgxColumnTypes::BasicBool),
            AgxPropertyEntryDefinition(&AgxDictionary::x_ExitEvents_x,"",AgxColumnTypes::Event),
            AgxPropertyEntryDefinition(&AgxDictionary::Payload,"",AgxColumnTypes::BasicString) }, nullptr);
        enterBlockDef.SetEnabledState(false);
        exitBlockDef.SetEnabledState(false);

        _PropertyBlocks.insert(&AgxDictionary::EnterEvents, enterBlockDef);
        _PropertyBlocks.insert(&AgxDictionary::ExitEvents, exitBlockDef);

        connect(this, &AgxNode::ParentGraphTypeUpdated, this, [this](const AgxGraphType& type) {
            if (type == AgxGraphType::SFBGS_StateMachine) {
                _PropertyBlocks[&AgxDictionary::EnterEvents].SetEnabledState(true);
                _PropertyBlocks[&AgxDictionary::ExitEvents].SetEnabledState(true);
            } else {
                _PropertyBlocks[&AgxDictionary::EnterEvents].SetEnabledState(false);
                _PropertyBlocks[&AgxDictionary::ExitEvents].SetEnabledState(false);
            }

                });

        connect(this, &AgxNode::PropertySheetUpdated, this, [this]() {
                _SFBGS_Hidden[&AgxDictionary::NodeType].second = name();
                },Qt::SingleShotConnection);
    }
}

std::shared_ptr<AgxPort> SFBGSNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) 
{
    unsigned int idxI = index > _In_Ports.count() ? _In_Ports.count() : index;
    unsigned int idxO = index > _Out_Ports.count() ? _Out_Ports.count() : index;

    auto sfbgsPort = std::make_shared<AgxPort_SFBGS>(this);

    switch (portType)
    {
        case AgxPortType::In:
            sfbgsPort->AddStandardPropertySheet();
            //sfbgsPort->SetName("In");
            if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) sfbgsPort->AddBlendInput();
            _In_Ports.insert(idxI, sfbgsPort);
        break;
        case AgxPortType::Out:
            sfbgsPort->SetName("Out");
            _Out_Ports.insert(idxO, sfbgsPort);   
        break;
        default:
            return nullptr;
        break;
    }

    sfbgsPort->Load(data);

    if (sfbgsPort->GetId() == InvalidAgxPortId)
    {
        sfbgsPort->SetId(NewPortId());
    }

    _nextPortId = sfbgsPort->GetId() >= _nextPortId ? sfbgsPort->GetId() + 1 : _nextPortId;

    if(_sidebarContent && portType == AgxPortType::In)
    {
        if (auto sfbgsSidebar = dynamic_cast<SFBGS_SidebarContent*>(_sidebarContent.get()))
        {
            auto widget = sfbgsPort->GetEmbeddedWidget();
            sfbgsSidebar->AddContentItem(widget,idxI);
        }
    }

    return sfbgsPort;
}

void SFBGSNode::_RemovePort(AgxPortType portType, AgxPortIndex index, bool preserve)
{
    AgxNode::_RemovePort(portType, index, preserve);
    if (auto sfbgsSidebar = dynamic_cast<SFBGS_SidebarContent*>(_sidebarContent.get()))
    {
        QTimer::singleShot(1, sfbgsSidebar, [this, sfbgsSidebar]() {Q_EMIT sfbgsSidebar->StateChanged(); });
    }
}

void SFBGSNode::SetPortData(AgxPortType portType, AgxPortIndex index, const QJsonObject& dataSet)
{
    AgxNode::SetPortData(portType, index, dataSet);
    if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(_Out_Ports.at(index).get()))
        _nextPortId = sfbgsPort->GetId() >= _nextPortId ? sfbgsPort->GetId() + 1 : _nextPortId;
}

void SFBGSNode::InitializeWidget(bool split)
{
    AgxNode::InitializeWidget();

    _NodePropertiesWidget->CreateFlagEntry(tr("SFBGS Flags"), this, &_flags);

    _NodePropertiesWidget->CreateHiddenEntries(&_SFBGS_Hidden, this, true, _HiddenOrder);
    auto sfbgsPropList = _NodePropertiesWidget->CreatePropertyEntries(&_SFBGS_Properties, this, split);
    for (auto sfbgsProp : sfbgsPropList)
    {
        sfbgsProp->setCheckbox(false);
    }
    _NodePropertiesWidget->CreatePropertyEntries(&_PropertyEntries, this, split);
    _NodePropertiesWidget->SetHiddenEntries(hiddenState);
    _NodePropertiesWidget->CreateGuidLabel(getGuidRef(), this, split);
}

QWidget* SFBGSNode::GetNodePropertyWidget()
{
    if (!_NodePropertiesWidget) {
        InitializeWidget(false);

        for (auto key : _BlockOrder) {
            if(_PropertyBlocks.contains(key))
                _NodePropertiesWidget->CreatePropetryBlock(key, _PropertyBlocks[key]);
        }

        //check for missing blocks
        for (auto& key : _PropertyBlocks.keys()) {
            
            if (!_BlockOrder.contains(key)) {
                _NodePropertiesWidget->CreatePropetryBlock(key, _PropertyBlocks[key]);
                QMessageBox message(QMessageBox::Critical, AgxDictionary::ErrorTerm().translation, QString(tr("Missing Block In Block Order For %1 On Node %2, Please Copy This Message And Make A Bug Report.")).arg(key().tag).arg(name()), QMessageBox::Ok);
                message.exec();
            }
        }

        _NodePropertiesWidget->FinalizeWidget();
    }

    return _NodePropertiesWidget;
}

QWidget* SFBGSNode::GetSideBarContent()
{
    if (!_sidebarContent)
    {
        SFBGS_SidebarContent* sidebar = new SFBGS_SidebarContent();
        _sidebarContent = sidebar;
        
        sidebar->SetTitle(std::format("{} [Node Id: {}]", _nameProperty.toStdString().c_str(), _nodeIdRef).c_str());
        connect(this, &AgxNode::PropertySheetUpdated, sidebar, [this, sidebar]() {sidebar->SetTitle(std::format("{} [Node Id: {}]", _nameProperty.toStdString().c_str(), _nodeIdRef).c_str());});

        sidebar->AddMainItem(GetNodePropertyWidget(),0,0,Qt::AlignRight);

        for (unsigned int i = 0; i < _In_Ports.size(); i++)
        {
            sidebar->AddContentItem(_In_Ports.at(i)->GetEmbeddedWidget(),i+1);
        }

        if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) {
            QHBoxLayout* hBox = new QHBoxLayout();

            hBox->addWidget(new QLabel("Divisions: "), 1, Qt::AlignLeft);
            QSpinBox* box = new QSpinBox();
            box->setValue(_Divisions);
            hBox->addWidget(box);
            QWidget* widget = new QWidget();
            widget->setLayout(hBox);
            sidebar->AddContentItem(widget, 0, 0, Qt::AlignRight, true);

            connect(box, &QSpinBox::editingFinished, sidebar, [this, sidebar, box]() {
                        if(box->value() != _Divisions)
                            sidebar->SendInsertPropertySheetDataCommand(QStringListToQJsonObject({"divisions"}, box->value()));
                    });

            connect(this, &AgxNode::PropertySheetUpdated, box, [this,box]() {
                        box->blockSignals(true);
                        box->setValue(_Divisions);
                        box->blockSignals(false);
                    });
        }
    }

    return _sidebarContent;
}
