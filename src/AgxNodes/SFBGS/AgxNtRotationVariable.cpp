//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRotationVariable.h"

namespace SFBGS {
    AgxNtRotationVariable::AgxNtRotationVariable(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Rotation Variable");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Rotation Variable", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredValue, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Speed, "CurrentSpeed", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RotationType, _RotationList.at(0)().tag, AgxColumnTypes::CustomDropDown, _RotationList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UseRadians, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtRotationVariable::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setName(std::format("input{}",m_inPorts.size()).c_str());
            }
        }

        return port;
    }
}