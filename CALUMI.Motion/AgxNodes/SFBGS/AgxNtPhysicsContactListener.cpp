//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtPhysicsContactListener.h"

namespace SFBGS {
    AgxNtPhysicsContactListener::AgxNtPhysicsContactListener(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Physics Contact Listener");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Physics Contact Listener", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEvent, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GoDynamicOnContact, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IgnoreContactBones, "", AgxColumnTypes::BasicString));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtPhysicsContactListener::name() const
    {
        return QStringLiteral("NT_PHYSICS_CONTACT_LISTENER");
    }

    QString AgxNtPhysicsContactListener::caption() const
    {
        return tr("Physics Contact Listener");
    }

    unsigned int AgxNtPhysicsContactListener::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtPhysicsContactListener::GetNodeType() const
    {
        return AgxNodeType::NT_PHYSICS_CONTACT_LISTENER;
    }

    std::shared_ptr<AgxPort> AgxNtPhysicsContactListener::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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