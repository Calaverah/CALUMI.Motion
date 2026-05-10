//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtPairedAnimation.h"

namespace SFBGS {
    AgxNtPairedAnimation::AgxNtPairedAnimation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Paired Animation");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Paired Animation", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeadSuffix, "Killer", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VictimSuffix, "Victim", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData entryBlock({
            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::PairedAnimNameRoot,"",AgxColumnTypes::BasicString)
                                        }, nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtPairedAnimation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetPropertySheetOptional(true);
                sfbgsPort->SetName("");
            }
        }

        return port;
    }
}