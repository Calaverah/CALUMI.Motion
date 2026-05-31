//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtCumulativeAnimation.h"

namespace SFBGS {
    AgxNtCumulativeAnimation::AgxNtCumulativeAnimation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Cumulative Animation");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Cumulative Animation", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CurrentCumulativeCount, "CumulativeAnimationsCount", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxCumulativeAnimation, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ResetAfterMaxIterations, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CleanUpOnly, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventSentAfterMaxIterations, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EventtoResetAllCumulativeAnimations, "", AgxColumnTypes::Event));

        {
            AgxPropertyBlockData entriesBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BoneNames,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"0",AgxColumnTypes::BasicInteger) }, nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::Entries, entriesBlock);
        }

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};

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

    std::shared_ptr<AgxPort> AgxNtCumulativeAnimation::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setName("default");
            }
        }

        return port;
    }
}