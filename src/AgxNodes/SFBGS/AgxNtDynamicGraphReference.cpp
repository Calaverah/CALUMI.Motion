//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtDynamicGraphReference.h"

namespace SFBGS {
    AgxNtDynamicGraphReference::AgxNtDynamicGraphReference(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Dynamic Graph Reference");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Dynamic Graph Reference", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SwapGraphEvent, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartingGraph, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTime, "0.3", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData graphBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::GraphName,"graph_name.agx",AgxColumnTypes::BasicString) }, nullptr);

        m_PropertyBlocks.insert(&AgxDictionary::Graphs, graphBlock);

        m_blockOrder = { &AgxDictionary::Graphs, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtDynamicGraphReference::name() const
    {
        return QStringLiteral("NT_DYNAMIC_GRAPH_REFERENCE");
    }

    QString AgxNtDynamicGraphReference::caption() const
    {
        return tr("Dynamic Graph Reference");
    }

    unsigned int AgxNtDynamicGraphReference::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 2;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtDynamicGraphReference::GetNodeType() const
    {
        return AgxNodeType::NT_DYNAMIC_GRAPH_REFERENCE;
    }

    std::shared_ptr<AgxPort> AgxNtDynamicGraphReference::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setPropertySheetOptional(true);
                sfbgsPort->setName("");
            }
        }

        return port;
    }
}