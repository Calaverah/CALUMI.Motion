//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtPostBoneModifierControl.h"

namespace SFBGS {
    AgxNtPostBoneModifierControl::AgxNtPostBoneModifierControl(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Post Bone Modifier Control");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Bone Modifier Control", AgxColumnTypes::BasicString));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtPostBoneModifierControl::name() const
    {
        return QStringLiteral("NT_POST_BONE_MODIFIER_CONTROL");
    }

    QString AgxNtPostBoneModifierControl::caption() const
    {
        return tr("Post Bone Modifier Control");
    }

    unsigned int AgxNtPostBoneModifierControl::nPorts(AgxPortType portType) const
    {
        if (portType == AgxPortType::In) return 2;

        return 1;
    }

    AgxNodeType AgxNtPostBoneModifierControl::GetNodeType() const
    {
        return AgxNodeType::NT_POST_BONE_MODIFIER_CONTROL;
    }

    std::shared_ptr<AgxPort> AgxNtPostBoneModifierControl::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                if(_In_Ports.size() == 1)
                    sfbgsPort->SetName("Pose Input");
                if(_In_Ports.size() == 2)
                    sfbgsPort->SetName("Pose Output");
            }
        }

        return port;
    }
}