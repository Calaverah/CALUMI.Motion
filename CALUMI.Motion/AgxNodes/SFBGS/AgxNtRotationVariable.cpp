#include "stdafx.h"
#include "AgxNtRotationVariable.h"

namespace SFBGS {
    AgxNtRotationVariable::AgxNtRotationVariable(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Rotation Variable");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Rotation Variable", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredValue, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Speed, "CurrentSpeed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RotationType, _RotationList.at(0)().tag, AgxColumnTypes::CustomDropDown, _RotationList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UseRadians, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtRotationVariable::name() const
    {
        return QStringLiteral("NT_ROTATION_VARIABLE");
    }

    QString AgxNtRotationVariable::caption() const
    {
        return tr("Rotation Variable");
    }

    unsigned int AgxNtRotationVariable::nPorts(AgxPortType portType) const
    {
        if (portType == AgxPortType::Out) return 1;

        return 0;
    }

    AgxNodeType AgxNtRotationVariable::GetNodeType() const
    {
        return AgxNodeType::NT_ROTATION_VARIABLE;
    }

    std::shared_ptr<AgxPort> AgxNtRotationVariable::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName(std::format("input{}",_In_Ports.size()).c_str());
            }
        }

        return port;
    }
}