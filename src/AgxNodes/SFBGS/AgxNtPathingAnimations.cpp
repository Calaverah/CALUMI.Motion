//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtPathingAnimations.h"

namespace SFBGS {
    AgxNtPathingAnimations::AgxNtPathingAnimations(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Pathing Animations");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Pathing Animations", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedVar, "Speed", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NPCHeadingOffset_Var, "NPCHeadingOffset", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectionVar, "Direction", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMinMultiplier, "0.75", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMaxMultiplier, "1.15", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MoveStartCorrectionTime, "1.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RampInTime, "0.2", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WalkNormalized, "1.25", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RunNormalized, "1.25", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "SyncFootsteps", AgxColumnTypes::SyncSystem));

        {
            AgxPropertyBlockData standToMoveTags({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::x_StandtoMoveTags_x,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                               }, nullptr);

            AgxPropertyBlockData moveToStopTags({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::x_MovetoStopTags_x,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                                 }, nullptr);

            AgxPropertyBlockData whileMovingTags({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::x_WhileMovingTags_x,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                                }, nullptr);
            


            m_PropertyBlocks.insert(&AgxDictionary::StandToMove, standToMoveTags);
            m_PropertyBlocks.insert(&AgxDictionary::MoveToStop, moveToStopTags);
            m_PropertyBlocks.insert(&AgxDictionary::WhileMoving, whileMovingTags);

            m_blockOrder = { &AgxDictionary::StandToMove, &AgxDictionary::MoveToStop, &AgxDictionary::WhileMoving, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
        }

    }

    QString AgxNtPathingAnimations::name() const
    {
        return QStringLiteral("NT_PATHING_ANIMATIONS");
    }

    QString AgxNtPathingAnimations::caption() const
    {
        return tr("Pathing Animations");
    }

    unsigned int AgxNtPathingAnimations::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtPathingAnimations::GetNodeType() const
    {
        return AgxNodeType::NT_PATHING_ANIMATIONS;
    }

    std::shared_ptr<AgxPort> AgxNtPathingAnimations::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setName("passthrough");
                Q_EMIT sfbgsPort->propertySheetUpdated();
            }
        }

        return port;
    }
}