#include "stdafx.h"
#include "AgxNtTimerEvent.h"

namespace SFBGS {
    AgxNtTimerEvent::AgxNtTimerEvent(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Timer Event");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Timer Event", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mintime, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Maxtime, "2", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventName, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ResetEventName, "", AgxColumnTypes::Event));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtTimerEvent::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName("passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
