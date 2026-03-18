//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEventController.h"

namespace SFBGS {
    AgxNtEventController::AgxNtEventController(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Event Controller");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Event Controller", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LogicVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ReleaseEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnCatch, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NegateVariable, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DiscardEvents, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData entryBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::ListenEvent, "", AgxColumnTypes::Event),
                AgxPropertyEntryDefinition(&AgxDictionary::CancelEvent,"",AgxColumnTypes::Event)
                                            }, nullptr);


            _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);


        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtEventController::name() const
    {
        return QStringLiteral("NT_EVENT_CONTROLLER");
    }

    QString AgxNtEventController::caption() const
    {
        return tr("Event Controller");
    }

    unsigned int AgxNtEventController::nPorts(AgxPortType portType) const
    {
        if (portType == AgxPortType::Out) return 1;

        return 0;
    }

    AgxNodeType AgxNtEventController::GetNodeType() const
    {
        return AgxNodeType::NT_EVENT_CONTROLLER;
    }

    std::shared_ptr<AgxPort> AgxNtEventController::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        //if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        //{
        //    if (portType == AgxPortType::In) {
        //        sfbgsPort->SetName("passthrough");
        //    }
        //}

        return port;
    }
}