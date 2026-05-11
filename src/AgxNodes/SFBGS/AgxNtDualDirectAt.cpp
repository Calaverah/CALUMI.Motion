//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtDualDirectAt.h"

namespace SFBGS {
    AgxNtDualDirectAt::AgxNtDualDirectAt(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Dual Direct At");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Dual Direct At", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBone, "DirectAt", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestLocationBone, "C_Chest", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestAxisIndex, "1", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadLocationBone, "C_Head", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimVectorVariable, "AimAtLocation", AgxColumnTypes::CustomVector));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LookVectorVariable, "LookAtLocation", AgxColumnTypes::CustomVector));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingDeltaVariable, "DirectAt Heading Delta", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimHeadingOffsetOutput, "DirectAtHeadingVar", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AimPitchOffsetOutput, "DirectAtPitchVar", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingOffsetOutput, "LookAtHeadingVar", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PitchOffsetOutput, "LookAtPitchVar", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NoHeadTrackVariable, "bNoHeadTrack", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsHeadTrackingActiveVariable, "bHeadTrackingActive", AgxColumnTypes::CustomBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NoAimVariable, "bNoAim", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsAimActiveVariable, "bAimActive", AgxColumnTypes::CustomBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LagDegrees, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadLagDegrees, "", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.25", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadBlendRate, "", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChestFullBlendMaxAngle, "90", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RotateHeadBeforeChest, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMin, "DisableHeadingMin", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMax, "DisableHeadingMax", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingHysteresis, "10", AgxColumnTypes::BasicFloat));

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

        m_PropertyBlocks.insert(&AgxDictionary::Headings, headingBlock);
        m_PropertyBlocks.insert(&AgxDictionary::Bones, boneBlock);

        m_blockOrder = { &AgxDictionary::Bones, &AgxDictionary::Headings, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};

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
                    switch (m_inPorts.size())
                    {
                        case 1: sfbgsPort->setName("passthrough");
                            break;
                        case 2: sfbgsPort->setName("Chest");
                            break;
                        case 3: sfbgsPort->setName("Head");
                            break;
                        default:
                            sfbgsPort->setName("");
                            break;
                    }
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
