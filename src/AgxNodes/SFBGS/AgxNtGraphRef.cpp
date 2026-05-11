//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtGraphRef.h"

namespace SFBGS {
    AgxNtGraphRef::AgxNtGraphRef(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Graph Reference");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GraphRef, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WrappedGraph, "", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtGraphRef::name() const
    {
        return QStringLiteral("NT_GRAPH_REF");
    }

    QString AgxNtGraphRef::caption() const
    {
        return tr("Graph Reference");
    }

    QString AgxNtGraphRef::SubCaption() const
    {
        QString cap = m_propertyEntries.at(0).value.isEmpty() ? "-" : m_propertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtGraphRef::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtGraphRef::GetNodeType() const
    {
        return AgxNodeType::NT_GRAPH_REF;
    }

    std::shared_ptr<AgxPort> AgxNtGraphRef::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get())) {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->setPropertySheetEnabled(true);
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }
}