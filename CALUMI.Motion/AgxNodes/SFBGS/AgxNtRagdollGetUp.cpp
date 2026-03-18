//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRagdollGetUp.h"

namespace SFBGS {
    AgxNtRagdollGetUp::AgxNtRagdollGetUp(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Ragdoll Get Up");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Ragdoll Get Up", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GetUpEvent, "GetUp", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GetUpStartEvent, "GetUpStart", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendFromRagdollTime, "0.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CoreBodyBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RightHandBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeftHandBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RightLegBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeftLegBoneName, "", AgxColumnTypes::BasicString));

        AgxPropertyBlockData blockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::AnimTag, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Orientation, "0", AgxColumnTypes::BasicFloat)
                                       },nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Directions, blockData);

        _BlockOrder = { &AgxDictionary::Directions, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtRagdollGetUp::name() const
    {

        return QStringLiteral("NT_RAGDOLL_GET_UP");
    }

    QString AgxNtRagdollGetUp::caption() const
    {
        return tr("Ragdoll Get Up");
    }

    unsigned int AgxNtRagdollGetUp::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtRagdollGetUp::GetNodeType() const
    {
        return AgxNodeType::NT_RAGDOLL_GET_UP;
    }

    std::shared_ptr<AgxPort> AgxNtRagdollGetUp::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
