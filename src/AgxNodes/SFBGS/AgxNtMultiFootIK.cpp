//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtMultiFootIK.h"

namespace SFBGS {
    AgxNtMultiFootIK::AgxNtMultiFootIK(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("MultiLeg Foot IK");


        
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BendAxis, _AxisList.at(0)().tag, AgxColumnTypes::CustomDropDown, _AxisList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MinBendAngle, "30", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBendAngle, "90", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::FinalBoneAxisIndex, "0", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CastDistanceUp, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CastDistanceDown, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DownErrorBias, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RootFeedbackWeight, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TargetBlendTime, "0.3", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::FootUnlockBlendScalar, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::XYTransLockTolerance, "0;0;0", AgxColumnTypes::BasicVector));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableVariable, "bNoFootIK", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendOnOfftime, "0.3", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AlignOrientPercent, "0.1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PositionChangeEvent, "PositionChange", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxControllerSeperation, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ContactNormalBlendRate, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ScaleSetVariable, "", AgxColumnTypes::CustomInteger));

        {
            AgxPropertyBlockData entryBlockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::ExtraName,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::HipName,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::KneeName,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::AnkleName,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::AngMult,"-1",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"0",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"0",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"0",AgxColumnTypes::BasicInteger)
                                               }, nullptr);

            AgxPropertyBlockData unk1({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Set,"",AgxColumnTypes::CustomInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::LegsAExtra,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Hip,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Knee,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Ankle,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::ComHghtMin,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Max,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::BoneVariance,"",AgxColumnTypes::CustomFloat)
                                               }, nullptr);

            AgxPropertyBlockData unk2({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Set,"",AgxColumnTypes::CustomInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::LegsBExtra,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Hip,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Knee,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Ankle,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::LegsCExtra,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::CHip,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::CKnee,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::CAnkle,"",AgxColumnTypes::CustomFloat)
                                      }, nullptr);

            AgxPropertyBlockData unk3({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Set,"",AgxColumnTypes::CustomInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::LegsDExtra,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Hip,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Knee,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::Ankle,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::LegsEExtra,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::EHip,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::EKnee,"",AgxColumnTypes::CustomFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::EAnkle,"",AgxColumnTypes::CustomFloat)
                                      }, nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::Entries, entryBlockDef);
            m_PropertyBlocks.insert(&AgxDictionary::Unknown1, unk1);
            m_PropertyBlocks.insert(&AgxDictionary::Unknown2, unk2);
            m_PropertyBlocks.insert(&AgxDictionary::Unknown3, unk3);
            
            m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::Unknown1, &AgxDictionary::Unknown2, &AgxDictionary::Unknown3, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
        }

    }

    QString AgxNtMultiFootIK::name() const
    {

        return QStringLiteral("NT_MULTI_FOOT_IK");
    }

    QString AgxNtMultiFootIK::caption() const
    {
        return tr("MultiLeg Foot IK");
    }

    unsigned int AgxNtMultiFootIK::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtMultiFootIK::GetNodeType() const
    {
        return AgxNodeType::NT_MULTI_FOOT_IK;
    }

    std::shared_ptr<AgxPort> AgxNtMultiFootIK::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
