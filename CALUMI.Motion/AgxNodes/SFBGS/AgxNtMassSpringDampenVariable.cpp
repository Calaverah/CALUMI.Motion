#include "stdafx.h"
#include "AgxNtMassSpringDampenVariable.h"

namespace SFBGS {
    AgxNtMassSpringDampenVariable::AgxNtMassSpringDampenVariable(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Mass Spring Dampen Variable");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Timer Event", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Variable, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OutputVariable, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MassOverride, "", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpringCoefficientOverride, "", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DampenOverride, "", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mass, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpringCoefficient, "90", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Dampen, "5", AgxColumnTypes::BasicFloat));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtMassSpringDampenVariable::name() const
    {
        return QStringLiteral("NT_MASS_SPRING_DAMPEN_VARIABLE");
    }

    QString AgxNtMassSpringDampenVariable::caption() const
    {
        return tr("Mass Spring Dampen Variable");
    }

    unsigned int AgxNtMassSpringDampenVariable::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtMassSpringDampenVariable::GetNodeType() const
    {
        return AgxNodeType::NT_MASS_SPRING_DAMPEN_VARIABLE;
    }

    std::shared_ptr<AgxPort> AgxNtMassSpringDampenVariable::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName("passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
