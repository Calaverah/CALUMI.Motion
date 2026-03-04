#include "stdafx.h"
#include "AgxNtPathingAnimations.h"

namespace SFBGS {
    AgxNtPathingAnimations::AgxNtPathingAnimations(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Pathing Animations");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Pathing Animations", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedVar, "Speed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NPCHeadingOffset_Var, "NPCHeadingOffset", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectionVar, "Direction", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMinMultiplier, "0.75", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMaxMultiplier, "1.15", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MoveStartCorrectionTime, "1.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RampInTime, "0.2", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WalkNormalized, "1.25", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RunNormalized, "1.25", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "SyncFootsteps", AgxColumnTypes::SyncSystem));

        {
            AgxPropertyBlockData whileMovingTags({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Tags,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                               }, nullptr);

            AgxPropertyBlockData moveToStopTags({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Tags,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                                 }, nullptr);

            AgxPropertyBlockData standToMoveTags({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Tags,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::SpeedClassification,_dropdown.at(0)().tag,AgxColumnTypes::CustomDropDown,_dropdown) 
                                                }, nullptr);
            


            _PropertyBlocks.insert(&AgxDictionary::WhileMoving, whileMovingTags);
            _PropertyBlocks.insert(&AgxDictionary::MoveToStop, moveToStopTags);
            _PropertyBlocks.insert(&AgxDictionary::StandToMove, standToMoveTags);

            _BlockOrder = { &AgxDictionary::WhileMoving, &AgxDictionary::MoveToStop, &AgxDictionary::StandToMove, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
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
                sfbgsPort->SetName("passthrough");
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}