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
#include "Utilities/AgxFormat.h"
#include "Widgets/AgxLineEdit.h"

AgxNode::~AgxNode()
{
    AgxNode::CloseEmbeddedView();
    if (m_sidebarContent)
        m_sidebarContent->deleteLater();
}

AgxNode::AgxNode(AgxGraphModel* rootGraphRef) : m_nextPortId{0}, m_nodePropertiesWidget(nullptr), m_rootGraphReference(rootGraphRef)
{

}

void AgxNode::SetNameProperty(const QString& newName)
{
    if (CanSetNameProperty())
    {
        m_nameProperty = newName;
        Q_EMIT PropertySheetUpdated();
    }
}

QJsonObject AgxNode::save() const
{
    QJsonObject modelJson;

    modelJson["model-name"] = name();

    if (CanSetNameProperty())
        modelJson["nameProperty"] = m_nameProperty;

    modelJson["groupId"] = m_groupName;

    QJsonObject propSheet = getPropertySheetData();

    modelJson["collapsed"] = m_collapsed;


    if (m_embeddedGraphModel.get())
    {
        modelJson["embedded-graph"] = m_embeddedGraphModel->save();
    }

    QJsonObject inPortData;
    QJsonObject outPortData;
    for (unsigned int i = 0; i < m_inPorts.count(); i++)
    {
        inPortData[std::to_string(m_inPorts.at(i)->getId()).c_str()] = m_inPorts.at(i)->save();
    }
    for (unsigned int i = 0; i < m_outPorts.count(); i++)
    {
        outPortData[std::to_string(m_outPorts.at(i)->getId()).c_str()] = m_outPorts.at(i)->save();
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
        const QJsonValue s = data["nameProperty"];
        SetNameProperty(s.toString());
    }
    setGroupId(data["groupId"].toString());

    QJsonObject pSheet = data["property-sheet"].toObject();
    QJsonObject blockData = pSheet["property-blocks"].toObject();

    const QJsonObject inPortData = pSheet["in-ports"].toObject();
    const QJsonObject outPortData = pSheet["out-ports"].toObject();
    
    for (unsigned int i = 0; i < inPortData.count(); i++)
    {
        const auto port = _AddPort(AgxPortType::In);
        port->setId(inPortData.keys().at(i).toUInt());
    }
    for (unsigned int i = 0; i < outPortData.count(); i++)
    {
        const auto port = _AddPort(AgxPortType::Out);
        port->setId(outPortData.keys().at(i).toUInt());
    }

    for (int i = 0; i < m_PropertyBlocks.size(); i++)
    {
        auto key = m_PropertyBlocks.keys().at(i);
        m_PropertyBlocks[key].loadDefault(blockData[key().tag].toObject());
    }

    insertPropertySheetData(pSheet);

    if (const QJsonValue jCollapse = data["collapsed"]; jCollapse.toBool())
    {
        ToggleCollapse();
    }

    if(m_embeddedGraphModel.get() && data.contains("embedded-graph"))
    {
        const QJsonObject embeddedGraph = data["embedded-graph"].toObject();
        m_embeddedGraphModel->load(embeddedGraph);
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
        if (const auto collapsed = xmlNode.child("collapsed").child_value(); QString("True").compare(collapsed, Qt::CaseInsensitive) == 0 != m_collapsed)
        {
            ToggleCollapse();
        }
        xmlNode.remove_child("collapsed");
    }

    if (xmlNode.child("name")) {
        m_nameProperty = xmlNode.child_value("name");
        xmlNode.remove_child("name");
    }

    if (xmlNode.child("node_type")) {
        
        if (name() != xmlNode.child_value("node_type"))
        {
            AmendValidationState("Node Type Does Not Match Input", AgxNodeValidationState::State::Warning);
        }
        xmlNode.remove_child("node_type");
    }

    {
        std::ostringstream oss;
        xmlNode.print(oss);
        m_excessData += oss.str();
    }

    if(!m_excessData.isEmpty()) AmendValidationState("Node Not Fully Imported!", AgxNodeValidationState::State::Warning);
    blockSignals(false);

    Q_EMIT statusUpdate(1.0);
    Q_EMIT PropertySheetUpdated();
    Q_EMIT embeddedWidgetSizeUpdated();
    QApplication::processEvents();
}

AgxConnectionPolicy AgxNode::portConnectionPolicy(const AgxPortType portType, AgxPortIndex portIndex) const
{
    auto result = AgxConnectionPolicy::One;
    switch (portType)
    {
        case AgxPortType::In:
            result = AgxConnectionPolicy::One;
            break;
        case AgxPortType::Out:
            result = AgxConnectionPolicy::Many;
            break;
        default:
            break;
    }

    return result;
}

void AgxNode::AmendValidationState(const QString& messageToAdd, const AgxNodeValidationState::State& minState)
{
    return;
    auto vState = validationState();
    if (!vState.m_stateMessage.isEmpty()) vState.m_stateMessage += "\n";

    switch (minState)
    {
        case AgxNodeValidationState::State::Warning: 
            vState.m_stateMessage += "WARNING: ";
            break;
        case AgxNodeValidationState::State::Error:
            vState.m_stateMessage += "ERROR: ";
            break;
        default:
            break;
    }

    vState.m_stateMessage += messageToAdd;

    if (vState.m_state < minState) vState.m_state = minState;

    setValidationState(vState);
}

void AgxNode::setValidationState(const AgxNodeValidationState& validationState)
{}

void AgxNode::InitializeWidget()
{
    //called before any null embedded widget is summoned on child class
    if (!m_nodePropertiesWidget)
    {
        m_nodePropertiesWidget = new AgxNodePropertiesWidget(nullptr, true);
        //enter standard node widgets here
    }
}

void AgxNode::ToggleCollapse()
{
    m_collapsed = !m_collapsed;
}

void AgxNode::insertPropertySheetData(const QJsonObject& data)
{
    //should be called after child node class override is handled
    if (data.contains("hiddenState"))
        m_hiddenState = data.value("hiddenState").toBool();

    //update standard node data here
    QJsonObject jsonBlock = data["property-blocks"].toObject();
    for (int i = 0; i < m_PropertyBlocks.size(); i++)
    {
        if (auto key = m_PropertyBlocks.keys().at(i); jsonBlock.contains(key().tag))
        {
            m_PropertyBlocks[key].insertPropertyBlockData(jsonBlock[key().tag].toObject());
        }
    }

    for (int i = 0; i < m_propertyEntries.size(); i++)
    {
        if (QString key = m_propertyEntries.at(i).Tag(); data.contains(key))
        {
            if(data[key].toObject().contains("value"))
                m_propertyEntries[i].value = data[key].toObject()["value"].toString();
            if(data[key].toObject().contains("isPresent"))
                m_propertyEntries[i].SetIsPresent(data[key].toObject()["isPresent"].toBool());
        }
    }

    QJsonObject inPortData = data["in-ports"].toObject();
    QJsonObject outPortData = data["out-ports"].toObject();

    for (unsigned int i = 0; i < inPortData.count(); i++)
    {
        QString key = inPortData.keys().at(i);
        //_In_Ports.at(i)->Load(inPortData[key].toObject());

        for (const auto port : m_inPorts)
        {
            if (port->getId() == key.toUInt())
            {
                port->load(inPortData[key].toObject());
            }
        }
    }
    for (unsigned int i = 0; i < outPortData.count(); i++)
    {
        QString key = outPortData.keys().at(i);
        //_Out_Ports.at(i)->Load(outPortData[key].toObject());
        for (const auto port : m_outPorts)
        {
            if (port->getId() == key.toUInt())
            {
                port->load(outPortData[key].toObject());
            }
        }
    }
    
    Q_EMIT PropertySheetUpdated();
    Q_EMIT embeddedWidgetSizeUpdated();
}

QJsonObject AgxNode::getPropertySheetData(const bool cleared) const
{
    QJsonObject output;
    //serialize property sheet for parent class

    output["hiddenState"] = QJsonValue(m_hiddenState);

    QJsonObject blocks;
    for (int i = 0; i < m_PropertyBlocks.size(); i++)
    {
        auto key = m_PropertyBlocks.keys().at(i);
        blocks[key().tag] = m_PropertyBlocks.value(key).getPropertyBlockData(cleared);
    }
    output["property-blocks"] = blocks;

    
    for (int i = 0; i < m_propertyEntries.size(); i++)
    {
        QString key = m_propertyEntries.at(i).Tag();
        QJsonObject pair;
        pair["value"] = cleared ? "" : m_propertyEntries[i].value;
        pair["isPresent"] = cleared ? false : m_propertyEntries[i].isPresent;
        output[key] = pair;
    }

    QJsonObject inPortData;
    QJsonObject outPortData;
    for (unsigned int i = 0; i < m_inPorts.count(); i++)
    {
        inPortData[std::to_string(m_inPorts.at(i)->getId()).c_str()] = m_inPorts.at(i)->save();
    }
    for (unsigned int i = 0; i < m_outPorts.count(); i++)
    {
        outPortData[std::to_string(m_outPorts.at(i)->getId()).c_str()] = m_outPorts.at(i)->save();
    }

    output["in-ports"] = inPortData;
    output["out-ports"] = outPortData;

    return output;
}

AgxPropertyBlockData* AgxNode::getPropertyBlock(const TermRef ref) {
    if (!m_PropertyBlocks.contains(ref))
        return nullptr;

    return &m_PropertyBlocks[ref];
}
AgxPropertyBlockData* AgxNode::getPropertyBlock(const QString& block)
{
    for (const auto blockTermRef : m_PropertyBlocks.keys())
    {
        if (blockTermRef().tag.compare(block, Qt::CaseInsensitive) == 0)
        {
            return &m_PropertyBlocks[blockTermRef];
        }
    }

    return nullptr;
}
void AgxNode::SetUpEmbeddedNodeGraph()
{
    m_embeddedGraphModel = std::make_shared<AgxGraphModel>(m_gameType, m_rootGraphReference);
    m_embeddedGraphScene = std::make_shared<AgxGraphicsScene>(*m_embeddedGraphModel);
}

void AgxNode::CloseEmbeddedView()
{
    if (!m_embeddedGraphScene.get())
        return;

    auto views = m_embeddedGraphScene->views();
    AgxGraphicsView* agxView = nullptr;
    for (const auto entry : views)
    {
        agxView = dynamic_cast<AgxGraphicsView*>(entry);
        if (agxView)
        {
            break;
        }
    }

    for (auto topList = QApplication::topLevelWidgets(); const auto entry : topList)
    {
        if (const auto mainWindow = dynamic_cast<CALUMIMotion*>(entry))
        {
            mainWindow->CloseTab(agxView);
        }
    }

}

QWidget* AgxNode::GetSideBarContent()
{
    return nullptr;

    if (!m_sidebarContent)
    {
        
        m_sidebarContent = new QWidget();
        auto layout = new QVBoxLayout();
        m_sidebarContent->setLayout(layout);
        auto header = new QLabel(std::format("{} [Node Id: {}]", m_nameProperty.toStdString().c_str(),m_nodeIdRef).c_str());
        connect(this, &AgxNode::PropertySheetUpdated, this, [this, header] {header->setText(std::format("{} [Node Id: {}]", m_nameProperty.toStdString().c_str(), m_nodeIdRef).c_str()); });

        header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        layout->addWidget(header);

        auto hLine = new QFrame();
        hLine->setFrameShape(QFrame::HLine);
        hLine->setFrameShadow(QFrame::Sunken);
        layout->addWidget(hLine);
    }

    return m_sidebarContent;
}

void AgxNode::SetSidebarVisibility(const bool state)
{
    if (m_sidebarContent)
    {
        m_sidebarContent->setVisible(state);
        if (const auto sfbgsItem = dynamic_cast<AgxSidebarContent*>(m_sidebarContent.get()))
        {
            Q_EMIT sfbgsItem->StateChanged();
        }
    }
}


void AgxNode::inputConnectionCreated(const AgxConnectionId& connection)
{
    if (connection.inPortIndex < m_inPorts.count())
    {
        m_inPorts.at(connection.inPortIndex)->setConnectionState(true);
    }
}
void AgxNode::inputConnectionDeleted(const AgxConnectionId& connection)
{
    if (connection.inPortIndex < m_inPorts.count())
    {
        m_inPorts.at(connection.inPortIndex)->setConnectionState(false);
    }
}

void AgxNode::SetNodeIdRef(const AgxNodeId& nodeId)
{
    m_nodeIdRef = nodeId;
    Q_EMIT PropertySheetUpdated();
}

std::shared_ptr<AgxPort> AgxNode::_AddPort(const AgxPortType portType, const AgxPortIndex index, const QJsonObject data)
{
    const unsigned int idxI = index > m_inPorts.count() ? m_inPorts.count() : index;
    const unsigned int idxO = index > m_outPorts.count() ? m_outPorts.count() : index;

    auto agxPort = std::make_shared<AgxPort>(this);

    switch (portType)
    {
        case AgxPortType::In:
            m_inPorts.insert(idxI, agxPort);
            break;
        case AgxPortType::Out:
            m_outPorts.insert(idxO, agxPort);
            break;
        default:
            return nullptr;
    }

    agxPort->load(data);

    if (agxPort->getId() == InvalidAgxPortId)
    {
        agxPort->setId(NewPortId());
    }

    m_nextPortId = agxPort->getId() >= m_nextPortId ? agxPort->getId() + 1 : m_nextPortId;

    return agxPort;
}

void AgxNode::_RemovePort(const AgxPortType portType, const AgxPortIndex index, const bool preserve)
{
    //choosing to leave the possibility of both incase we switch to AgxPortType
    const unsigned int idxI = index >= m_inPorts.count() ? m_inPorts.count() - 1 : index;
    const unsigned int idxO = index >= m_outPorts.count() ? m_outPorts.count() - 1 : index;

    if (preserve)
    {
        switch (portType)
        {
            case AgxPortType::In:
                if (const auto agxPort = dynamic_cast<AgxPort_SFBGS*>(m_inPorts.at(idxI).get())) {
                    if (agxPort->getId() == m_nextPortId - 1)
                        m_nextPortId--;
                }
                break;
            case AgxPortType::Out:
                if (const auto agxPort = dynamic_cast<AgxPort_SFBGS*>(m_outPorts.at(idxO).get())) {
                    if (agxPort->getId() == m_nextPortId - 1)
                        m_nextPortId--;
                }
                break;
            default:
                break;
        }
    }
    
    switch (portType)
    {
        case AgxPortType::In:
            m_inPorts.erase(m_inPorts.begin() + idxI);
            break;
        case AgxPortType::Out:
            m_outPorts.erase(m_outPorts.begin() + idxO);
            break;
        default:
            break;
    }
}

QJsonObject AgxNode::PortData(const AgxPortType portType, const AgxPortIndex index)
{   
    switch (portType)
    {
        case AgxPortType::In:
            if (index < m_inPorts.count())
                return m_inPorts.at(index)->save();
            break;
        case AgxPortType::Out:
            if (index < m_outPorts.count())
                return m_outPorts.at(index)->save();
            break;
        default:
            break;
    }
    return QJsonObject();
}

void AgxNode::SetPortData(const AgxPortType portType, const AgxPortIndex index, const QJsonObject& dataSet)
{
    switch (portType)
    {
        case AgxPortType::In:
            if (index < m_inPorts.count())
                return m_inPorts.at(index)->load(dataSet);
            break;
        case AgxPortType::Out:
            if (index < m_outPorts.count())
                return m_outPorts.at(index)->load(dataSet);
            break;
        default:
            break;
    }
}

unsigned int AgxNode::PortCount(const AgxPortType portType) const
{
    switch (portType)
    {
        case AgxPortType::In:
            return m_inPorts.count();
        case AgxPortType::Out:
            return m_outPorts.count();
        default:
            return 0;
    }
}

void AgxNode::SetPortCount(const AgxPortType portType, const unsigned int count)
{
    unsigned int portCount = 0;

    switch (portType)
    {
        case AgxPortType::In:
            portCount = m_inPorts.count();
            break;
        case AgxPortType::Out:
            portCount = m_outPorts.count();
            break;
        default:
            break;
    }
    if (portCount > count)
    {
        const auto size = portCount - count;
        for (unsigned int i = 0; i < size; i++)
        {
            _RemovePort(portType, portCount);
            portCount--;
        }
    } else if (portCount < count)
    {
        const auto size = portCount - count;
        for (unsigned int i = 0; i < size; i++)
        {
            _AddPort(portType, portCount);
            portCount++;
        }
    }
    
}

AgxPortType AgxNode::GetPortType(AgxPort* port) const
{
    for (unsigned int i = 0; i < m_inPorts.count(); i++)
    {
        if (m_inPorts[i].get() == port) return AgxPortType::In;
    }
    for (unsigned int i = 0; i < m_outPorts.count(); i++)
    {
        if (m_outPorts[i].get() == port) return AgxPortType::Out;
    }

    return AgxPortType::None;
}

void AgxNode::_ExternalPortCommand(const AgxPortType portType, const AgxPortIndex index, const QString& command, const QString& payload)
{
    if (PortCount(portType) <= index) return;

    switch (portType)
    {
        case AgxPortType::In:
            m_inPorts.at(index)->externalCommand(command, payload);
            break;
        case AgxPortType::Out:
            m_outPorts.at(index)->externalCommand(command, payload);
            break;
        default:
            break;
    }
}

AgxPortIndex AgxNode::GetPortIndex(AgxPort* port) const
{
    for (unsigned int i = 0; i < m_inPorts.count(); i++)
    {
        if (m_inPorts[i].get() == port) return i;
    }
    for (unsigned int i = 0; i < m_outPorts.count(); i++)
    {
        if (m_outPorts[i].get() == port) return i;
    }

    return InvalidPortIndex;
}

QString AgxNode::portCaption(const AgxPortType portType, const AgxPortIndex idx) const
{
    QString output = "Error";
    if (portType == AgxPortType::In) {
        output = m_inPorts.count() > idx ? m_inPorts.at(idx).get()->caption() : output;
    } else if (portType == AgxPortType::Out) {
        output = m_outPorts.count() > idx ? m_outPorts.at(idx).get()->caption() : output;
    }
    return output;
}

bool AgxNode::portCaptionVisible(const AgxPortType portType, const AgxPortIndex idx) const
{
    bool output = false;
    if (portType == AgxPortType::In)
    {
        output = m_inPorts.count() > idx ? m_inPorts.at(idx).get()->hasCaption() : output;
    }
    else if (portType == AgxPortType::Out)
    {
        output = m_outPorts.count() > idx ? m_outPorts.at(idx).get()->hasCaption() : output;
    }

    return output;
}

void AgxNode::ResetPorts()
{
    SetPortCount(AgxPortType::In, 0);
    SetPortCount(AgxPortType::Out, 0);
    m_nextPortId = 0;
}

void SFBGSNode::save(pugi::xml_node& parent, QVector<AgxConnectionId> connections, const QVector<AgxNodeId> sortedIds, const QPointF pos)
{
    auto nodeObject = AgxAppend(parent, "node", AgxFormat::NewLine, 0);

    if(m_sfbgsHidden[&AgxDictionary::DefaultState].second.compare("true", Qt::CaseInsensitive) == 0)
        AgxAppendValue(nodeObject, AgxDictionary::DefaultState().tag, "True", AgxFormat::NewLine);

    AgxAppendValue(nodeObject, AgxDictionary::NodeType().tag, typeName(), {AgxFormat::NewLine, AgxFormat::Indent},1);
    AgxAppendValue(nodeObject, AgxDictionary::noninstanced().tag, m_sfbgsHidden[&AgxDictionary::noninstanced].second == "-" ? "False" : m_sfbgsHidden[&AgxDictionary::noninstanced].second, { AgxFormat::NewLine, AgxFormat::Indent }, 1);

    if (const auto flagsValue = m_flags.GetValue(); flagsValue != 0)
    {
        auto flagsObject = AgxAppend(nodeObject, "flags", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        m_flags.ToXML(flagsObject);
    }

    if (GetNodeType() == AgxNodeType::NT_BLEND_NODE)
    {
        AgxAppendValue(nodeObject, "divisions", QString("%1").arg(m_divisions), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        for (auto& port : m_inPorts)
        {
            if (const auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
            {
                FormatBlendInput(nodeObject, sfbgsPort->getBlendInput());
            }
        }
    }
    
    AgxAppendValue(nodeObject, "name", nameProperty(), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::PosX().tag, CleanUpDecimals(QString("%1").arg(pos.x() / SFBGSxScalar, 0, 'f', 5)), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::PosY().tag, CleanUpDecimals(QString("%1").arg(pos.y() / SFBGSyScalar, 0, 'f', 5)), { AgxFormat::NewLine, AgxFormat::Indent }, 1);

    const QString ePosX = m_sfbgsHidden[&AgxDictionary::ExpPosX].second == "-" ? CleanUpDecimals(QString("%1").arg(pos.x()
        / SFBGSxScalar, 0, 'f', 5)) : m_sfbgsHidden[&AgxDictionary::ExpPosX].second;
    AgxAppendValue(nodeObject, AgxDictionary::ExpPosX().tag, ePosX, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    const QString ePosY = m_sfbgsHidden[&AgxDictionary::ExpPosY].second == "-" ? CleanUpDecimals(QString("%1").arg(pos.y()
        / SFBGSyScalar, 0, 'f', 5)) : m_sfbgsHidden[&AgxDictionary::ExpPosY].second;
    AgxAppendValue(nodeObject, AgxDictionary::ExpPosY().tag, ePosY, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    
    AgxAppendValue(nodeObject, AgxDictionary::UseColor().tag, m_sfbgsHidden[&AgxDictionary::UseColor].second == "-" ? "False" : m_sfbgsHidden[&AgxDictionary::UseColor].second, { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, AgxDictionary::UserId().tag, GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "0"), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, "collapsed", isCollapsed()? "True" : "False", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    AgxAppendValue(nodeObject, "guid", getGuid().toString(QUuid::WithoutBraces), { AgxFormat::NewLine, AgxFormat::Indent }, 1);
    
    for (auto& port : m_inPorts)
    {
        auto input = AgxAppend(nodeObject, "input", { AgxFormat::NewLine, AgxFormat::Indent }, 1);
        AgxAppendValue(input, "name", port.get()->caption(false), AgxFormat::SpaceBefore);
        AgxAppendValue(input, "id", QString("%1").arg(port.get()->getId()), AgxFormat::SpaceBefore);
        AgxAppendValue(input, "idx", QString("%1").arg(port.get()->getPortIndex()), AgxFormat::SpaceBefore);
        input.append_child(pugi::node_pcdata).set_value("\n");

        
        if(!port.get()->isConnected()) 
        {
            constexpr char lineEnd[2] = { 0x20, '\0' };
            input.append_child(pugi::node_pcdata).set_value(lineEnd);
        } 
        else 
        {
            for (auto& [outNodeId, outPortIndex, inNodeId, inPortIndex, isHidden] : connections)
            {
                if (inPortIndex != port.get()->getPortIndex() || inNodeId != m_nodeIdRef) continue;

                auto link = AgxAppend(input, "link", { AgxFormat::NewLine, AgxFormat::Indent }, 2);
                AgxAppendValue(link, "node", QString("%1").arg(sortedIds.indexOf(outNodeId) + 1), { AgxFormat::NewLine, AgxFormat::Indent }, 3);
                AgxAppendValue(link, "output", QString("%1").arg(outPortIndex), AgxFormat::SpaceBefore);
                AgxAppendValue(link, "hidden", isHidden ? "True" : "False", AgxFormat::SpaceBefore);
                port.get()->savePropertySheet(link);
                AgxCloseNode(link, false, true, 2);
                constexpr char lineEnd[3] = { 0x0A,0x20,'\0' };
                input.append_child(pugi::node_pcdata).set_value(lineEnd);

            }
        }
    }

    for (auto& port : m_outPorts)
    {
        auto output = AgxAppend(nodeObject, "output", { AgxFormat::NewLine, AgxFormat::Indent }, 1);

        AgxAppendValue(output, "name", port.get()->caption(false), AgxFormat::SpaceBefore);
        AgxAppendValue(output, "id", QString("%1").arg(port.get()->getId()), AgxFormat::SpaceBefore);
        AgxAppendValue(output, "idx", QString("%1").arg(port.get()->getPortIndex()), AgxFormat::SpaceBefore);
        output.append_child(pugi::node_pcdata).set_value("\n");
        output.append_child(pugi::node_pcdata).set_value(" ");
    }

    FormatBasicPropertySheet(nodeObject, m_propertyEntries);

    for (auto& blockKey : m_blockOrder)
    {
        if (!m_PropertyBlocks[blockKey].IsEnabledState()) continue;
        
        FormatPropertyBlock(nodeObject, m_PropertyBlocks[blockKey]);
    }

    if (m_embeddedGraphModel.get())
    {
        auto graphObject = AgxAppend(nodeObject, "graph", AgxFormat::NewLine, 0);
        m_embeddedGraphModel.get()->save(graphObject);
        AgxCloseNode(graphObject, false, false, 0);
    }

    AgxCloseNode(nodeObject, false, false);
}

QJsonObject SFBGSNode::save() const
{
    QJsonObject output;
    const QJsonObject agxNode = AgxNode::save();
    output = JsonMerge(output, agxNode);

    for (int i = 0; i < m_sfbgsHidden.size(); i++)
    {
        auto key = m_sfbgsHidden.keys().at(i);
        output[key().tag] = m_sfbgsHidden[key].second;
    }

    for (int i = 0; i < m_sfbgsProperties.size(); i++)
    {
        QString key = m_sfbgsProperties.at(i).Tag();
        output[key] = m_sfbgsProperties[i].value;
    }

    if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) {
        output["divisions"] = std::to_string(m_divisions).c_str();
    }

    output["guid"] = getGuid().toString(QUuid::StringFormat::WithoutBraces);

    output["flags"] = m_flags.ToJson();

    QJsonObject userIdVal;
    userIdVal["value"] = GetPropertyValue(m_sfbgsProperties, AgxDictionary::GetInstance().UserId().tag);
    output[AgxDictionary::GetInstance().UserId().tag] = userIdVal;

    return output;
}

void SFBGSNode::load(QJsonObject const& data)
{
    AgxNode::load(data);

    m_flags.FromJson(data["flags"]);

    for (int i = 0; i < m_sfbgsHidden.size(); i++)
    {
        if (auto key = m_sfbgsHidden.keys().at(i); data.contains(key().tag) && key().tag != "guid")
            m_sfbgsHidden[key].second = data.value(key().tag).toString();
    }

    for (int i = 0; i < m_sfbgsProperties.size(); i++)
    {
        if(QString key = m_sfbgsProperties.at(i).Tag(); data.contains(key) && key != "guid")
            m_sfbgsProperties[i].value = data.value(key).toObject().value("value").toString();
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
        m_sfbgsHidden[&AgxDictionary::ExpPosX].second = xmlNode.child_value("expanded_pos_x");
        xmlNode.remove_child("expanded_pos_x");
    }
    if (xmlNode.child("expanded_pos_y")) {
        m_sfbgsHidden[&AgxDictionary::ExpPosY].second = xmlNode.child_value("expanded_pos_y");
        xmlNode.remove_child("expanded_pos_y");
    }
    if (xmlNode.child("pos_x")) {
        m_sfbgsHidden[&AgxDictionary::PosX].second = xmlNode.child_value("pos_x");
        xmlNode.remove_child("pos_x");
    }
    if(xmlNode.child("pos_y")) {
        m_sfbgsHidden[&AgxDictionary::PosY].second = xmlNode.child_value("pos_y");
        xmlNode.remove_child("pos_y");
    }

    if (xmlNode.child("divisions")) {
        bool ok = false;
        int divRes = QString(xmlNode.child_value("divisions")).toInt(&ok);
        divRes = ok ? divRes : 0;
        m_divisions = divRes;
    }

    if (xmlNode.child("flags")) {
        auto flags = xmlNode.child("flags");
        m_flags.FromXML(flags);
        if(flags.first_child()) {
            AmendValidationState("Flags Not Handled!", AgxNodeValidationState::State::Warning);
        }
        xmlNode.remove_child("flags");
    }

    blockSignals(false);
    Q_EMIT statusUpdate(0.05f);
    blockSignals(true);

    if (xmlNode.child("noninstanced"))
    {
        const QString nIns = xmlNode.child_value("noninstanced");
        m_sfbgsHidden[&AgxDictionary::noninstanced].second = nIns == "True" ? "True" : "False";
        if (nIns != "True" && nIns != "False")
            AmendValidationState(std::format("Non Instanced Disparity: {}", nIns.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        xmlNode.remove_child("noninstanced");
    }else { AmendValidationState("NonInstanced Not Found!", AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("use_color_2"))
    {
        const QString c2Str = xmlNode.child_value("use_color_2");
        m_sfbgsHidden[&AgxDictionary::UseColor].second = c2Str.compare("True",Qt::CaseInsensitive) == 0 ? "True" : "False";
        if (c2Str.compare("True",Qt::CaseInsensitive) != 0 && c2Str.compare("False",Qt::CaseInsensitive) != 0)
            AmendValidationState(std::format("Use Color Disparity: {}", c2Str.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        xmlNode.remove_child("use_color_2");
    } else { AmendValidationState("use_color_2 Not Found!", AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("default_state"))
    {
        const QString state = xmlNode.child_value("default_state");
        m_sfbgsHidden[&AgxDictionary::DefaultState].second = state.compare("True", Qt::CaseInsensitive) == 0 ? "True" : "False";
        if (state != "True" && state != "False")
            AmendValidationState(std::format("Default State Disparity: {}", state.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        
        xmlNode.remove_child("default_state");
    } else m_sfbgsHidden[&AgxDictionary::DefaultState].second = "NONE";

    if (xmlNode.child("user_id"))
    {
        const QString uIdStr = xmlNode.child_value("user_id");
        bool ok = false;
        auto value = uIdStr.toInt(&ok);
        if (!ok)
        {
            value = 0;
            AmendValidationState(std::format("UserId Disparity: {}", uIdStr.toStdString().c_str()).c_str(), AgxNodeValidationState::State::Error);
        }
        SetPropertyValue(m_sfbgsProperties, AgxDictionary::GetInstance().UserId().tag, QString("%1").arg(value));
        
        xmlNode.remove_child("user_id");
    } else { AmendValidationState(tr("User Id Not Found!"), AgxNodeValidationState::State::Warning); }

    if (xmlNode.child("guid"))
    {
        const QUuid guidOld = getGuid();
        QUuid guidNew;
        setGuid(guidNew.fromString(xmlNode.child_value("guid")));
        if (getGuid() == guidOld)
            AmendValidationState("GUID NOT SET!", AgxNodeValidationState::State::Error);
        xmlNode.remove_child("guid");
    }

    

    if (GetNodeType() == AgxNodeType::NT_BLEND_TREE_EMBEDDED || GetNodeType() == AgxNodeType::NT_STATE_MACHINE_EMBEDDED) {
        if (!xmlNode.child("graph")) AmendValidationState("EMBEDDED TYPE HAS NO GRAPH!", AgxNodeValidationState::State::Error);
        auto xmlGraph = xmlNode.child("graph");
        if (m_embeddedGraphModel) {

            const auto connectionToGraph = connect(m_embeddedGraphModel.get(), &AgxGraphModel::statusUpdate, this, [this](const float loadPercentage, const QString& message) { Q_EMIT statusUpdate(0.2 + 0.4 * loadPercentage, message); });

            blockSignals(false);
            Q_EMIT statusUpdate(0.2f, "Loading Embedded Graph Node");
            blockSignals(true);

            m_embeddedGraphModel->load(xmlGraph);
            
            disconnect(connectionToGraph);
            
        }
        xmlNode.remove_child("graph");
    }
    else
    {
        blockSignals(false);
        Q_EMIT statusUpdate(0.2f);
        blockSignals(true);

        if (xmlNode.child("graph")) AmendValidationState("NON EMBEDDED TYPE HAS GRAPH!", AgxNodeValidationState::State::Error);
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
    while(xmlNode.remove_child("blendInput")) {}

    for (auto& inputNode : xmlNode.children("input")) {

        QJsonObject jPortData;

        jPortData["agxPortId"] = inputNode.child_value("id");

        auto port = _AddPort(AgxPortType::In, m_inPorts.size(), jPortData);

        port->blockSignals(true);
        {
            bool ok = false;
            if (const auto idx = QString(inputNode.child_value("idx")).toUInt(&ok); ok && idx != GetPortIndex(port.get()))
            {
                AmendValidationState("Port Index Mismatch!", AgxNodeValidationState::State::Error);
            }
        }
        
        if (maxPortId <= port->getId()) maxPortId = port->getId() + 1;
        
        if (const auto agxPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if(inputNode.child("name"))
                agxPort->setName(inputNode.child_value("name"));
            else
                AmendValidationState("Missing InPort Name!", AgxNodeValidationState::State::Warning);

            {
                //blend input data transfer
                const auto portIdx = GetPortIndex(port.get());
                if (const auto blendModel = agxPort->getBlendInput(); portIdx < blendPointData.size() && blendModel)
                {
                    for (const auto& [sWeight, sRange] : blendPointData.at(portIdx))
                    {
                        blendModel->addDataRow(blendModel->rowCount(), sWeight, sRange);
                    }
                }
            }

        }
        port->load(inputNode);

        inputNode.remove_child("id");
        inputNode.remove_child("idx");
        inputNode.remove_child("name");

        //xmlNode.remove_child(inputNode);
        //Q_EMIT dataUpdated(GetPortIndex(port.get()));
        port->blockSignals(false);
        Q_EMIT port->propertySheetUpdated();
    }
    blockSignals(false);
    Q_EMIT statusUpdate(0.7f);
    blockSignals(true);

    for (auto& outputNode : xmlNode.children("output"))
    {
        auto port = _AddPort(AgxPortType::Out, m_outPorts.size());
        {
            bool ok = false;
            const auto id = QString(outputNode.child_value("id")).toUInt(&ok);
            if (ok)
                port->setId(id);
        }
        {
            bool ok = false;
            if (const auto idx = QString(outputNode.child_value("idx")).toUInt(&ok); ok && idx != GetPortIndex(port.get()))
            {
                AmendValidationState("Port Index Mismatch!", AgxNodeValidationState::State::Error);
            }
        }
        
        if (maxPortId <= port->getId()) maxPortId = port->getId() + 1;

        if (const auto agxPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (outputNode.child("name"))
                agxPort->setName(outputNode.child_value("name"));
            else
                AmendValidationState("Missing OutPort Name!", AgxNodeValidationState::State::Warning);
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

    for (auto& pEntry : m_propertyEntries) {
        pEntry.SetIsPresent(false);
    }

    for(auto& propertySheet : xmlNode.children("property_sheet")) {
        if (QString("Property").compare(propertySheet.child("column").child_value("header"), Qt::CaseInsensitive) == 0)
        {
            if (QString("2").compare(propertySheet.child_value("num_columns")) == 0)
                AmendValidationState("Main Property Sheet Disparity!", AgxNodeValidationState::State::Error);

            for (auto& row : propertySheet.children("row"))
            {
                auto prop = row.first_child();
                auto value = row.last_child();
                for (auto& entry : m_propertyEntries)
                {
                    if (entry.Tag() == prop.child_value("value"))
                    {
                        entry.value = value.child_value("value");
                        entry.SetIsPresent(true);
                        //propertySheet.remove_child(row);
                        break;
                    }
                }
            }

            if(propertySheet.child("row"))
                AmendValidationState("Rows Not Read In Main Property Sheet", AgxNodeValidationState::State::Error);
            //xmlNode.remove_child(propertySheet);
        }
        else {
            blockCount++;
            if (blockCount > m_PropertyBlocks.size())
                AmendValidationState("Block Count Mismatch", AgxNodeValidationState::State::Error);
            else {
                auto blockKey = m_blockOrder.at(blockCount - 1);
                auto& block = m_PropertyBlocks[blockKey];
                if (QString(propertySheet.child_value("num_columns")).compare(
                    QString("%1").arg(block.GetColumnCount())
                    ) != 0)
                {
                    AmendValidationState("Block Column Count Mismatch", AgxNodeValidationState::State::Error);
                }

                block.load(propertySheet);
                //for (auto& col : propertySheet.children("column")) propertySheet.remove_child(col);
                //propertySheet.remove_child("num_columns");

                if(!propertySheet.empty())
                    AmendValidationState("Property Sheet Disparity", AgxNodeValidationState::State::Error);

                Q_EMIT block.DataUpdated();
            }
        }
    }

    blockSignals(false);
    Q_EMIT statusUpdate(0.9f);
    blockSignals(true);

    m_nextPortId = maxPortId;

    blockSignals(false);
    AgxNode::load(xmlNode);
}

void SFBGSNode::insertPropertySheetData(const QJsonObject& data)
{
    //Hidden SFBGS Entries

    for (int i = 0; i < m_sfbgsHidden.size(); i++)
    {
        if (auto key = m_sfbgsHidden.keys().at(i); data.contains(key().tag) && key().tag != "guid")
            m_sfbgsHidden[key].second = data.value(key().tag).toString();
    }

    if (data.contains(AgxDictionary::UserId().tag)) {
        SetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, data.value(AgxDictionary::UserId().tag).toObject().value("value").toString());
    }

    //SFBGS Entries
    if (data.contains("flags"))
        m_flags.FromJson(data["flags"]);

    m_divisions = data["divisions"].toInt();

    AgxNode::insertPropertySheetData(data);
}

QJsonObject SFBGSNode::getPropertySheetData(const bool cleared) const
{
    QJsonObject output = AgxNode::getPropertySheetData(cleared);

    output["divisions"] = cleared ? 0 : m_divisions;

    output["flags"] = m_flags.ToJson();

    output[AgxDictionary::UserId().tag] = GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag);
    
    QJsonObject userIdObj;
    userIdObj["value"] = GetPropertyValue(m_sfbgsProperties, AgxDictionary::GetInstance().UserId().tag);
    output[AgxDictionary::GetInstance().UserId().tag] = userIdObj;

    for (int i = 0; i < m_sfbgsHidden.size(); i++)
    {
        if (auto key = m_sfbgsHidden.keys().at(i); key().tag != "guid" && key != &AgxDictionary::NodeType)
            output[key().tag] = m_sfbgsHidden[key].second;
    }

    return output;
}

QString SFBGSNode::SubCaption() const
{
    return QString("%1 (%2)").arg(GetPropertyValue(m_propertyEntries, AgxDictionary::Name().tag, "-")).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
}

bool SFBGSNode::AltState()
{
    return m_sfbgsHidden[&AgxDictionary::DefaultState].second == "True";
}

void SFBGSNode::SetAltState(const bool enabled)
{
    m_sfbgsHidden[&AgxDictionary::DefaultState].second = enabled ? "True" : "-";
    Q_EMIT PropertySheetUpdated();
}

SFBGSNode::SFBGSNode(AgxGraphModel* rootGraphRef) : AgxNode(rootGraphRef)
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

    m_PropertyBlocks.insert(&AgxDictionary::EnterEvents, enterBlockDef);
    m_PropertyBlocks.insert(&AgxDictionary::ExitEvents, exitBlockDef);

    connect(this, &AgxNode::ParentGraphTypeUpdated, this, [this](const AgxGraphType& type) {
        if (type == AgxGraphType::SFBGS_StateMachine) {
            m_PropertyBlocks[&AgxDictionary::EnterEvents].SetEnabledState(true);
            m_PropertyBlocks[&AgxDictionary::ExitEvents].SetEnabledState(true);
        } else {
            m_PropertyBlocks[&AgxDictionary::EnterEvents].SetEnabledState(false);
            m_PropertyBlocks[&AgxDictionary::ExitEvents].SetEnabledState(false);
        }

            });

    connect(this, &AgxNode::PropertySheetUpdated, this, [this] {
            m_sfbgsHidden[&AgxDictionary::NodeType].second = name();
            },Qt::SingleShotConnection);
}


std::shared_ptr<AgxPort> SFBGSNode::_AddPort(const AgxPortType portType, const AgxPortIndex index, const QJsonObject data)
{
    const unsigned int idxI = index > m_inPorts.count() ? m_inPorts.count() : index;
    const unsigned int idxO = index > m_outPorts.count() ? m_outPorts.count() : index;

    auto sfbgsPort = std::make_shared<AgxPort_SFBGS>(this);

    switch (portType)
    {
        case AgxPortType::In:
            sfbgsPort->addStandardPropertySheet();
            //sfbgsPort->SetName("In");
            if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) sfbgsPort->addBlendInput();
            m_inPorts.insert(idxI, sfbgsPort);
            break;
        case AgxPortType::Out:
            sfbgsPort->setName("Out");
            m_outPorts.insert(idxO, sfbgsPort);
            break;
        default:
            return nullptr;
    }

    sfbgsPort->load(data);

    if (sfbgsPort->getId() == InvalidAgxPortId)
    {
        sfbgsPort->setId(NewPortId());
    }

    m_nextPortId = sfbgsPort->getId() >= m_nextPortId ? sfbgsPort->getId() + 1 : m_nextPortId;

    if(m_sidebarContent && portType == AgxPortType::In)
    {
        if (const auto sfbgsSidebar = dynamic_cast<SFBGS_SidebarContent*>(m_sidebarContent.get()))
        {
            const auto widget = sfbgsPort->getEmbeddedWidget();
            sfbgsSidebar->AddContentItem(widget,idxI);
        }
    }

    return sfbgsPort;
}

void SFBGSNode::_RemovePort(const AgxPortType portType, const AgxPortIndex index, const bool preserve)
{
    AgxNode::_RemovePort(portType, index, preserve);
    if (auto sfbgsSidebar = dynamic_cast<SFBGS_SidebarContent*>(m_sidebarContent.get()))
    {
        QTimer::singleShot(1, sfbgsSidebar, [sfbgsSidebar] {Q_EMIT sfbgsSidebar->StateChanged(); });
    }
}

void SFBGSNode::SetPortData(const AgxPortType portType, const AgxPortIndex index, const QJsonObject& dataSet)
{
    AgxNode::SetPortData(portType, index, dataSet);
    if (const auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(m_outPorts.at(index).get()))
        m_nextPortId = sfbgsPort->getId() >= m_nextPortId ? sfbgsPort->getId() + 1 : m_nextPortId;
}

void SFBGSNode::InitializeWidget(const bool split)
{
    AgxNode::InitializeWidget();

    m_nodePropertiesWidget->CreateFlagEntry(AgxDictionary::Flags, this, &m_flags);

    m_nodePropertiesWidget->CreateReadOnlyEntries(&m_sfbgsHidden, this, true, m_hiddenOrder);
    for (auto sfbgsPropList = m_nodePropertiesWidget->CreatePropertyEntries(&m_sfbgsProperties, this, split); const auto sfbgsProp : sfbgsPropList)
    {
        sfbgsProp->setCheckbox(false);
    }
    m_nodePropertiesWidget->CreatePropertyEntries(&m_propertyEntries, this, split);
    m_nodePropertiesWidget->CreateGuidLabel(getGuidRef(), this, split);
}

QWidget* SFBGSNode::GetNodePropertyWidget()
{
    if (!m_nodePropertiesWidget) {
        InitializeWidget(false);

        for (auto key : m_blockOrder)
        {
            if (m_PropertyBlocks.contains(key))
                m_nodePropertiesWidget->CreatePropetryBlock(key, m_PropertyBlocks[key]);
        }

        //check for missing blocks
        for (auto& key : m_PropertyBlocks.keys()) {
            
            if (!m_blockOrder.contains(key)) {
                m_nodePropertiesWidget->CreatePropetryBlock(key, m_PropertyBlocks[key]);
                QMessageBox message(QMessageBox::Critical, AgxDictionary::ErrorTerm().translation, QString(tr("Missing Block In Block Order For %1 On Node %2, Please Copy This Message And Make A Bug Report.")).arg(key().tag).arg(name()), QMessageBox::Ok);
                message.exec();
            }
        }

        m_nodePropertiesWidget->FinalizeWidget();
    }

    return m_nodePropertiesWidget;
}

QWidget* SFBGSNode::GetSideBarContent()
{
    if (!m_sidebarContent)
    {
        auto sidebar = new SFBGS_SidebarContent();
        m_sidebarContent = sidebar;
        
        sidebar->SetTitle(std::format("{} [Node Id: {}]", m_nameProperty.toStdString().c_str(), m_nodeIdRef).c_str());
        connect(this, &AgxNode::PropertySheetUpdated, sidebar, [this, sidebar] {sidebar->SetTitle(std::format("{} [Node Id: {}]", m_nameProperty.toStdString().c_str(), m_nodeIdRef).c_str());});

        sidebar->AddMainItem(GetNodePropertyWidget(),0,0,Qt::AlignRight);

        for (unsigned int i = 0; i < m_inPorts.size(); i++)
        {
            sidebar->AddContentItem(m_inPorts.at(i)->getEmbeddedWidget(),i+1);
        }

        if (GetNodeType() == AgxNodeType::DEBUG || GetNodeType() == AgxNodeType::NT_BLEND_NODE) {
            const auto hBox = new QHBoxLayout();

            hBox->addWidget(new QLabel("Divisions: "), 1, Qt::AlignLeft);
            auto box = new QSpinBox();
            box->setValue(m_divisions);
            hBox->addWidget(box);
            const auto widget = new QWidget();
            widget->setLayout(hBox);
            sidebar->AddContentItem(widget, 0, 0, Qt::AlignRight, true);

            connect(box, &QSpinBox::editingFinished, sidebar, [this, sidebar, box] {
                        if(box->value() != m_divisions)
                            sidebar->SendInsertPropertySheetDataCommand(QStringListToQJsonObject({"divisions"}, box->value()));
                    });

            connect(this, &AgxNode::PropertySheetUpdated, box, [this,box] {
                        box->blockSignals(true);
                        box->setValue(m_divisions);
                        box->blockSignals(false);
                    });
        }
    }

    return m_sidebarContent;
}
