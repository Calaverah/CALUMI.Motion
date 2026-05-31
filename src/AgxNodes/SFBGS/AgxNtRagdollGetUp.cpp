//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRagdollGetUp.h"

namespace SFBGS {
    AgxNtRagdollGetUp::AgxNtRagdollGetUp(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Ragdoll Get Up");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Ragdoll Get Up", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GetUpEvent, "GetUp", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GetUpStartEvent, "GetUpStart", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendFromRagdollTime, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CoreBodyBoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadBoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RightHandBoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeftHandBoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RightLegBoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LeftLegBoneName, "", AgxColumnTypes::BasicString));

        AgxPropertyBlockData blockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::AnimTag, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Orientation, "0", AgxColumnTypes::BasicFloat)
                                       },nullptr);

        m_PropertyBlocks.insert(&AgxDictionary::Directions, blockData);

        m_blockOrder = { &AgxDictionary::Directions, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtRagdollGetUp::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
