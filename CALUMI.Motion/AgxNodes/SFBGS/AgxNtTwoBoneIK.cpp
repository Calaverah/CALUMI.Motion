//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtTwoBoneIK.h"

namespace SFBGS {
    AgxNtTwoBoneIK::AgxNtTwoBoneIK(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Two Bone IK");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ParentBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BendBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Axis, _AxisList.at(0)().tag, AgxColumnTypes::CustomDropDown, _AxisList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AngleMultiplier, "1", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TargetBoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendOnOfftime, "0.1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AlignOrient, "True", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtTwoBoneIK::name() const
    {

        return QStringLiteral("NT_TWO_BONE_IK");
    }

    QString AgxNtTwoBoneIK::caption() const
    {
        return tr("Two Bone IK");
    }

    unsigned int AgxNtTwoBoneIK::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 2;
        }
    }

    AgxNodeType AgxNtTwoBoneIK::GetNodeType() const
    {
        return AgxNodeType::NT_TWO_BONE_IK;
    }

    std::shared_ptr<AgxPort> AgxNtTwoBoneIK::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    if(_In_Ports.size() == 1)
                        sfbgsPort->SetName("passthrough");
                    if(_In_Ports.size() == 2)
                        sfbgsPort->SetName("merge");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
