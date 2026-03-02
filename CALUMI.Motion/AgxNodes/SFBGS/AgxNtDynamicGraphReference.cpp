#include "stdafx.h"
#include "AgxNtDynamicGraphReference.h"

namespace SFBGS {
    AgxNtDynamicGraphReference::AgxNtDynamicGraphReference(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Dynamic Graph Reference");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Dynamic Graph Reference", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SwapGraphEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartingGraph, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTime, "0.3", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData graphBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::GraphName,"graph_name.agx",AgxColumnTypes::BasicString) }, nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Graphs, graphBlock);

        _BlockOrder = { &AgxDictionary::Graphs, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

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

    std::shared_ptr<AgxPort> AgxNtDynamicGraphReference::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetPropertySheetOptional(true);
                sfbgsPort->SetName("");
            }
        }

        return port;
    }
}