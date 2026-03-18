//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtDynamicAnimation.h"

namespace SFBGS {
    AgxNtDynamicAnimation::AgxNtDynamicAnimation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Dynamic Animation");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Dynamic Animation", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "", AgxColumnTypes::SyncSystem));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnEnd, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mirror, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsActionAnimation, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtDynamicAnimation::name() const
    {
        return QStringLiteral("NT_DYNAMIC_ANIMATION");
    }

    QString AgxNtDynamicAnimation::caption() const
    {
        return tr("Dynamic Animation");
    }

    unsigned int AgxNtDynamicAnimation::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtDynamicAnimation::GetNodeType() const
    {
        return AgxNodeType::NT_DYNAMIC_ANIMATION;
    }

    std::shared_ptr<AgxPort> AgxNtDynamicAnimation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetPropertySheetOptional(true);
                sfbgsPort->SetName("");
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}