//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtTagPropagation.h"

namespace SFBGS {
    AgxNtTagPropagation::AgxNtTagPropagation(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Tag Propagation Node");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Tag Propagation", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TagSyncSystem, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TypeofPropagation, _PropList.at(1)().tag, AgxColumnTypes::CustomDropDown, _PropList));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtTagPropagation::name() const
    {
        return QStringLiteral("NT_TAG_PROPAGATION");
    }

    QString AgxNtTagPropagation::caption() const
    {
        return tr("Tag Propagation Node");
    }

    unsigned int AgxNtTagPropagation::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtTagPropagation::GetNodeType() const
    {
        return AgxNodeType::NT_TAG_PROPAGATION;
    }

    std::shared_ptr<AgxPort> AgxNtTagPropagation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName("Passthrough");
            }
        }

        return port;
    }
}