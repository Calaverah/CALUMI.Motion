#include "stdafx.h"
#include "AgxNtDualDirectAt.h"

namespace SFBGS {
    AgxNtDualDirectAt::AgxNtDualDirectAt(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Dual Direct At");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Dual Direct At", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBone, "DirectAt", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestLocationBone, "C_Chest", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestAxisIndex, "1", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadLocationBone, "C_Head", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimVectorVariable, "AimAtLocation", AgxColumnTypes::CustomVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LookVectorVariable, "LookAtLocation", AgxColumnTypes::CustomVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingDeltaVariable, "DirectAt Heading Delta", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimHeadingOffsetOutput, "DirectAtHeadingVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimPitchOffsetOutput, "DirectAtPitchVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingOffsetOutput, "LookAtHeadingVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PitchOffsetOutput, "LookAtPitchVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NoHeadTrackVariable, "bNoHeadTrack", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsHeadTrackingActiveVariable, "bHeadTrackingActive", AgxColumnTypes::CustomBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NoAimVariable, "bNoAim", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsAimActiveVariable, "bAimActive", AgxColumnTypes::CustomBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LagDegrees, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadLagDegrees, "", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.25", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadBlendRate, "", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestFullBlendMaxAngle, "90", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RotateHeadBeforeChest, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMin, "DisableHeadingMin", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMax, "DisableHeadingMax", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingHysteresis, "10", AgxColumnTypes::BasicFloat));

        AgxPropertyBlockData boneBlock({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::CHeadingMin, "", AgxColumnTypes::CustomFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::CHeadingMax, "", AgxColumnTypes::CustomFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::CPitchMin, "", AgxColumnTypes::CustomFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::CPitchMax, "", AgxColumnTypes::CustomFloat)
                                           }, nullptr);

        AgxPropertyBlockData headingBlock({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BoneName, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MinX, "", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MaxX, "", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MinY, "", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MaxY, "", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MinZ, "", AgxColumnTypes::BasicFloat),
                                        AgxPropertyEntryDefinition(&AgxDictionary::MaxZ, "", AgxColumnTypes::BasicFloat)
                                            }, nullptr);


        boneBlock.AddRow();
        boneBlock.AddRow();
        boneBlock.SetRow(0, {"Chest","ChestHeadingMin","ChestHeadingMax","",""});
        boneBlock.SetRow(1, {"Head","HeadHeadingMin","HeadHeadingMax","",""});

        _PropertyBlocks.insert(&AgxDictionary::Headings, headingBlock);
        _PropertyBlocks.insert(&AgxDictionary::Bones, boneBlock);

        _BlockOrder = { &AgxDictionary::Bones, &AgxDictionary::Headings, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};

    }

    QString AgxNtDualDirectAt::name() const
    {

        return QStringLiteral("NT_DUAL_DIRECT_AT");
    }

    QString AgxNtDualDirectAt::caption() const
    {
        return tr("Dual Direct At");
    }

    unsigned int AgxNtDualDirectAt::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 3;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtDualDirectAt::GetNodeType() const
    {
        return AgxNodeType::NT_DUAL_DIRECT_AT;
    }

    std::shared_ptr<AgxPort> AgxNtDualDirectAt::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    switch (_In_Ports.size())
                    {
                        case 1: sfbgsPort->SetName("passthrough");
                            break;
                        case 2: sfbgsPort->SetName("Chest");
                            break;
                        case 3: sfbgsPort->SetName("Head");
                            break;
                        default:
                            sfbgsPort->SetName("");
                            break;
                    }
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
