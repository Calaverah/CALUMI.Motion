//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtBoneConstraint.h"

namespace SFBGS {
    SFBGS::AgxNtBoneConstraint::AgxNtBoneConstraint(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Bone Constraint");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Bone Constraint", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBone, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TargetBone, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActivateVariable, "", AgxColumnTypes::CustomInteger));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtBoneConstraint::name() const
    {
        return QStringLiteral("NT_BONE_CONSTRAINT");
    }

    QString AgxNtBoneConstraint::caption() const
    {
        return tr("Bone Constraint");
    }

    unsigned int AgxNtBoneConstraint::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtBoneConstraint::GetNodeType() const
    {
        return AgxNodeType::NT_BONE_CONSTRAINT;
    }

    std::shared_ptr<AgxPort> AgxNtBoneConstraint::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setName("passthrough");
            }
        }

        return port;
    }
}