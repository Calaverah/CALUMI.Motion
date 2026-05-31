//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtTimerEvent.h"

namespace SFBGS {
    AgxNtTimerEvent::AgxNtTimerEvent(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Timer Event");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Timer Event", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mintime, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Maxtime, "2", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventName, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ResetEventName, "", AgxColumnTypes::Event));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtTimerEvent::name() const
    {
        return QStringLiteral("NT_TIMER_EVENT");
    }

    QString AgxNtTimerEvent::caption() const
    {
        return tr("Timer Event");
    }

    unsigned int AgxNtTimerEvent::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtTimerEvent::GetNodeType() const
    {
        return AgxNodeType::NT_TIMER_EVENT;
    }

    std::shared_ptr<AgxPort> AgxNtTimerEvent::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->setName("passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
