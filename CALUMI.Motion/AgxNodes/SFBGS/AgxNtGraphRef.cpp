#include "stdafx.h"
#include "AgxNtGraphRef.h"

namespace SFBGS {
    AgxNtGraphRef::AgxNtGraphRef(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Graph Reference");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::GraphRef, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WrappedGraph, "", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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
        QString cap = _PropertyEntries.at(0).value.isEmpty() ? "-" : _PropertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
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
                    sfbgsPort->SetPropertySheetEnabled(true);
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }
}