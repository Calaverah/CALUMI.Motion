//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtSwitchNode.h"

namespace SFBGS {
    AgxNtSwitchNode::AgxNtSwitchNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Switch");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableName, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableType, _eventList.at(0)().tag, AgxColumnTypes::CustomDropDown, _eventList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBlendTime, "0.3", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RandomizeIfInvalid, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TagAlteration, _fixList.at(0)().tag, AgxColumnTypes::CustomDropDown, _fixList));

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