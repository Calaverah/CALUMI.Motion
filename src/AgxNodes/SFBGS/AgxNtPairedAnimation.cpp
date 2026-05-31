//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtPairedAnimation.h"

namespace SFBGS {
    AgxNtPairedAnimation::AgxNtPairedAnimation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Paired Animation");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Paired Animation", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeadSuffix, "Killer", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VictimSuffix, "Victim", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData entryBlock({
            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::PairedAnimNameRoot,"",AgxColumnTypes::BasicString)
                                        }, nullptr);

        m_PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtPairedAnimation::name() const
    {
        return QStringLiteral("NT_PAIRED_ANIMATION");
    }

    QString AgxNtPairedAnimation::caption() const
    {
        return tr("Paired Animation");
    }

    unsigned int AgxNtPairedAnimation::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtPairedAnimation::GetNodeType() const
    {
        return AgxNodeType::NT_PAIRED_ANIMATION;
    }

    std::shared_ptr<AgxPort> AgxNtPairedAnimation::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setPropertySheetOptional(true);
                sfbgsPort->setName("");
            }
        }

        return port;
    }
}