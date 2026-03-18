//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtBoneConstraint.h"

namespace SFBGS {
    SFBGS::AgxNtBoneConstraint::AgxNtBoneConstraint(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Bone Constraint");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Bone Constraint", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBone, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TargetBone, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActivateVariable, "", AgxColumnTypes::CustomInteger));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

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

    std::shared_ptr<AgxPort> AgxNtBoneConstraint::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName("passthrough");
            }
        }

        return port;
    }
}