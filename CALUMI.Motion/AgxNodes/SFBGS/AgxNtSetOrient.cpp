//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtSetOrient.h"

namespace SFBGS {
    AgxNtSetOrient::AgxNtSetOrient(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Set Orient");



        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Set Orient", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Type, _TypeList.at(0)().tag, AgxColumnTypes::CustomDropDown, _TypeList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OrientVar, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TurnAnimCameraVar, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsUsingCodeDrivenRotation, "", AgxColumnTypes::CustomBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxCorrectionSpeed, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxPostCorrectionSpeed, "0.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ClampedBlendTime, "0.1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsActiveBool, "1", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Time, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableWhenInactive, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EnforceDirectAtLimits, "", AgxColumnTypes::CustomBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtSetOrient::name() const
    {

        return QStringLiteral("NT_SET_ORIENT");
    }

    QString AgxNtSetOrient::caption() const
    {
        return tr("Set Orient");
    }

    unsigned int AgxNtSetOrient::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtSetOrient::GetNodeType() const
    {
        return AgxNodeType::NT_SET_ORIENT;
    }

    std::shared_ptr<AgxPort> AgxNtSetOrient::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        //if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        //{
        //    switch (portType)
        //    {
        //        case AgxPortType::In:
        //            sfbgsPort->SetName("passthrough");
        //            break;
        //    }
        //    Q_EMIT sfbgsPort->PropertySheetUpdated();
        //}

        return port;
    }

}
