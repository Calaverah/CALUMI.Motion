//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRagdollDrive.h"

namespace SFBGS {
    AgxNtRagdollDrive::AgxNtRagdollDrive(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Ragdoll Drive");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Ragdoll Drive", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendForces, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableLimits, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InertiaSpring, "20", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AngleSpring, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DeadZone, "1", AgxColumnTypes::BasicFloat));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtRagdollDrive::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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