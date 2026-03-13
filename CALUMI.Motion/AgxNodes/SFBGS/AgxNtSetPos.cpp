#include "stdafx.h"
#include "AgxNtSetPos.h"

namespace SFBGS {
    AgxNtSetPos::AgxNtSetPos(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Set Pos");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Set Pos", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Type, _VariableTypeList.at(0)().tag, AgxColumnTypes::CustomDropDown, _VariableTypeList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Time, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PositionVar, "DesiredLocation", AgxColumnTypes::CustomVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OrientedOffset, "0;0;0", AgxColumnTypes::BasicVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxCorrectionVel, "100000", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxPostCorrectionVel, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AlignmentBone, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableWhenInactive, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    std::shared_ptr<AgxPort> AgxNtSetPos::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    //sfbgsPort->SetPropertySheetEnabled(false);
                    sfbgsPort->SetName("Input");
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtSetPos::name() const
    {
        return QStringLiteral("NT_SET_POS");
    }

    QString AgxNtSetPos::caption() const
    {
        return tr("Set Pos");
    }

    unsigned int AgxNtSetPos::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtSetPos::GetNodeType() const
    {
        return AgxNodeType::NT_SET_POS;
    }
}
