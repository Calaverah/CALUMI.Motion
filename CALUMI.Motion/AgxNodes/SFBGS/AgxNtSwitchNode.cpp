#include "stdafx.h"
#include "AgxNtSwitchNode.h"

namespace SFBGS {
    AgxNtSwitchNode::AgxNtSwitchNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Switch");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableName, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableType, "Event", AgxColumnTypes::CustomDropDown, {"Event","Variable","Event and Variable"}));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBlendTime, "0.3", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RandomizeIfInvalid, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TagAlteration, "Do Nothing", AgxColumnTypes::CustomDropDown, {"Do Nothing","Prefix","Suffix"}));

        {
            AgxPropertyBlockData entryBlockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Def_,"False",AgxColumnTypes::BasicBool),
                AgxPropertyEntryDefinition(&AgxDictionary::StoryManagerEvent,"False",AgxColumnTypes::BasicBool),
                AgxPropertyEntryDefinition(&AgxDictionary::Param,"0",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::Tag,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::EventList,"",AgxColumnTypes::BasicString)
                                               }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtSwitchNode::name() const
    {
        return QStringLiteral("NT_SWITCH_NODE");
    }

    QString AgxNtSwitchNode::caption() const
    {
        return tr("Switch");
    }

    unsigned int AgxNtSwitchNode::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtSwitchNode::GetNodeType() const
    {
        return AgxNodeType::NT_SWITCH_NODE;
    }

    std::shared_ptr<AgxPort> AgxNtSwitchNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName("");
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}