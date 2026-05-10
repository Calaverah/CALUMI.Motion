//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtCumulativeAnimation.h"

namespace SFBGS {
    AgxNtCumulativeAnimation::AgxNtCumulativeAnimation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Cumulative Animation");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Cumulative Animation", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CurrentCumulativeCount, "CumulativeAnimationsCount", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxCumulativeAnimation, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ResetAfterMaxIterations, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CleanUpOnly, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventSentAfterMaxIterations, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventtoResetAllCumulativeAnimations, "", AgxColumnTypes::Event));

        {
            AgxPropertyBlockData entriesBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BoneNames,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"0",AgxColumnTypes::BasicInteger) }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, entriesBlock);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};

    }

    QString AgxNtCumulativeAnimation::name() const
    {
        return QStringLiteral("NT_CUMULATIVE_ANIMATION");
    }

    QString AgxNtCumulativeAnimation::caption() const
    {
        return tr("Cumulative Animation");
    }

    unsigned int AgxNtCumulativeAnimation::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtCumulativeAnimation::GetNodeType() const
    {
        return AgxNodeType::NT_CUMULATIVE_ANIMATION;
    }

    std::shared_ptr<AgxPort> AgxNtCumulativeAnimation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName("default");
            }
        }

        return port;
    }
}