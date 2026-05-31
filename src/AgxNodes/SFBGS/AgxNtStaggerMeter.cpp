//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtStaggerMeter.h"

namespace SFBGS {
    AgxNtStaggerMeter::AgxNtStaggerMeter(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Stagger Meter");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Stagger Meter", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ListenEvent,"StaggerStart", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StaggerForceVariable, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StaggerCooldown, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityThreshold, "1.0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityDelayBeforeDecay, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityDecayRate, "1.0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityConsumeRate, "2.0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackEvent, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackThreshold, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackChance, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackCooldown, "0", AgxColumnTypes::BasicFloat));

        {
            AgxPropertyBlockData blockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Event,"StaggerStart",AgxColumnTypes::Event),
                AgxPropertyEntryDefinition(&AgxDictionary::Threshold,"1",AgxColumnTypes::BasicFloat)
                                          }, nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::Entries, blockDef);
        }

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    std::shared_ptr<AgxPort> AgxNtStaggerMeter::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    if(m_inPorts.size() == 1)
                        sfbgsPort->setName("base animations");
                    if(m_inPorts.size() == 2)
                        sfbgsPort->setName("stagger animations");
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

    QString AgxNtStaggerMeter::name() const
    {
        return QStringLiteral("NT_STAGGER_METER");
    }

    QString AgxNtStaggerMeter::caption() const
    {
        return tr("Stagger Meter");
    }

    unsigned int AgxNtStaggerMeter::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 2;
        }
        return 1;
    }

    AgxNodeType AgxNtStaggerMeter::GetNodeType() const
    {
        return AgxNodeType::NT_STAGGER_METER;
    }

}
