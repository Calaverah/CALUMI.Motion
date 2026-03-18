//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEveryNEventsModifier.h"

namespace SFBGS {
    AgxNtEveryNEventsModifier::AgxNtEveryNEventsModifier(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Every N Events");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Every N Events", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ListenEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MinTimes, "0", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxTimes, "1", AgxColumnTypes::BasicInteger));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtEveryNEventsModifier::name() const
    {
        return QStringLiteral("NT_EVERY_N_EVENTS_MODIFIER");
    }

    QString AgxNtEveryNEventsModifier::caption() const
    {
        return tr("Every N Events");
    }

    unsigned int AgxNtEveryNEventsModifier::nPorts(AgxPortType portType) const
    {
        if (portType == AgxPortType::Out) return 1;
        
        return 0;
    }

    AgxNodeType AgxNtEveryNEventsModifier::GetNodeType() const
    {
        return AgxNodeType::NT_EVERY_N_EVENTS_MODIFIER;
    }

}