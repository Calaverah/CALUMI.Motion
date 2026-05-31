//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtMomentumSwitchback.h"

namespace SFBGS {
    AgxNtMomentumSwitchback::AgxNtMomentumSwitchback(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Momentum Switchback");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Momentum Switchback", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "SyncFootsteps", AgxColumnTypes::SyncSystem));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredSpeed, "Speed", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredDirection, "Direction", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WalkRunSpeed, "1.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RootTwistBlendRate, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MinimumActiveTime, "0.5", AgxColumnTypes::BasicFloat));

        AgxPropertyBlockData blockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::AnimTag, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MinTime, "0.33", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Angle0to1, "0", AgxColumnTypes::BasicFloat)
                                       }, nullptr);

        m_PropertyBlocks.insert(&AgxDictionary::Entries, blockData);

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtMomentumSwitchback::name() const
    {

        return QStringLiteral("NT_MOMENTUM_SWITCHBACK");
    }

    QString AgxNtMomentumSwitchback::caption() const
    {
        return tr("Momentum Switchback");
    }

    unsigned int AgxNtMomentumSwitchback::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtMomentumSwitchback::GetNodeType() const
    {
        return AgxNodeType::NT_MOMENTUM_SWITCHBACK;
    }

    std::shared_ptr<AgxPort> AgxNtMomentumSwitchback::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
