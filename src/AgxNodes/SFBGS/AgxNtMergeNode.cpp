//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtMergeNode.h"

namespace SFBGS {
    AgxNtMergeNode::AgxNtMergeNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Merge");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Merge Node", AgxColumnTypes::BasicString));
        
        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtMergeNode::name() const
    {
        return QStringLiteral("NT_MERGE_NODE");
    }

    QString AgxNtMergeNode::caption() const
    {
        return tr("Merge");
    }

    unsigned int AgxNtMergeNode::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 2;
        }
    }

    AgxNodeType AgxNtMergeNode::GetNodeType() const
    {
        return AgxNodeType::NT_MERGE_NODE;
    }

    std::shared_ptr<AgxPort> AgxNtMergeNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName(std::format("Input{}", _In_Ports.size()).c_str());
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}