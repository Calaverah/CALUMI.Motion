//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtStaggerMeter.h"

namespace SFBGS {
    AgxNtStaggerMeter::AgxNtStaggerMeter(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Stagger Meter");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Stagger Meter", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ListenEvent,"StaggerStart", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StaggerForceVariable, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StaggerCooldown, "0.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityThreshold, "1.0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityDelayBeforeDecay, "0.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityDecayRate, "1.0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImmunityConsumeRate, "2.0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackThreshold, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackChance, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CounterAttackCooldown, "0", AgxColumnTypes::BasicFloat));

        {
            AgxPropertyBlockData blockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Event,"StaggerStart",AgxColumnTypes::Event),
                AgxPropertyEntryDefinition(&AgxDictionary::Threshold,"1",AgxColumnTypes::BasicFloat)
                                          }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, blockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    std::shared_ptr<AgxPort> AgxNtStaggerMeter::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    if(_In_Ports.size() == 1)
                        sfbgsPort->SetName("base animations");
                    if(_In_Ports.size() == 2)
                        sfbgsPort->SetName("stagger animations");
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
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
