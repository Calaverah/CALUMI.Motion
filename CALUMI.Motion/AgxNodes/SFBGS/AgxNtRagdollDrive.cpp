#include "stdafx.h"
#include "AgxNtRagdollDrive.h"

namespace SFBGS {
    AgxNtRagdollDrive::AgxNtRagdollDrive(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Ragdoll Drive");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Ragdoll Drive", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendForces, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableLimits, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InertiaSpring, "20", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AngleSpring, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DeadZone, "1", AgxColumnTypes::BasicFloat));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtRagdollDrive::name() const
    {
        return QStringLiteral("NT_RAGDOLL_DRIVE");
    }

    QString AgxNtRagdollDrive::caption() const
    {
        return tr("Ragdoll Drive");
    }

    unsigned int AgxNtRagdollDrive::nPorts(AgxPortType portType) const
    {
        if (portType == AgxPortType::In) return 2;

        return 1;
    }

    AgxNodeType AgxNtRagdollDrive::GetNodeType() const
    {
        return AgxNodeType::NT_RAGDOLL_DRIVE;
    }

    std::shared_ptr<AgxPort> AgxNtRagdollDrive::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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