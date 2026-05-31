//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtMergeNode.h"

namespace SFBGS {
    AgxNtMergeNode::AgxNtMergeNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Merge");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Merge Node", AgxColumnTypes::BasicString));
        
        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtMergeNode::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setName(std::format("Input{}", m_inPorts.size()).c_str());
                Q_EMIT sfbgsPort->propertySheetUpdated();
            }
        }

        return port;
    }
}