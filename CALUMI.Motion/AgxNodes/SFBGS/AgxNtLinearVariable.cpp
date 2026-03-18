//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtLinearVariable.h"

namespace SFBGS {
    AgxNtLinearVariable::AgxNtLinearVariable(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Linear Variable");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Linear Variable", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredValue, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Speed, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtLinearVariable::name() const
    {
        return QStringLiteral("NT_LINEAR_VARIABLE");
    }

    QString AgxNtLinearVariable::caption() const
    {
        return tr("Linear Variable");
    }

    unsigned int AgxNtLinearVariable::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtLinearVariable::GetNodeType() const
    {
        return AgxNodeType::NT_LINEAR_VARIABLE;
    }

    std::shared_ptr<AgxPort> AgxNtLinearVariable::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
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
