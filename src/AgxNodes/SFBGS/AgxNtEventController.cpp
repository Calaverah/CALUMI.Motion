//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEventController.h"

namespace SFBGS {
    AgxNtEventController::AgxNtEventController(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Event Controller");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Event Controller", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LogicVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ReleaseEvent, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnCatch, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NegateVariable, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DiscardEvents, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData entryBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::ListenEvent, "", AgxColumnTypes::Event),
                AgxPropertyEntryDefinition(&AgxDictionary::CancelEvent,"",AgxColumnTypes::Event)
                                            }, nullptr);


            m_PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);


        }

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
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

    std::shared_ptr<AgxPort> AgxNtEventController::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        //if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        //{
        //    if (portType == AgxPortType::In) {
        //        sfbgsPort->SetName("passthrough");
        //    }
        //}

        return port;
    }
}