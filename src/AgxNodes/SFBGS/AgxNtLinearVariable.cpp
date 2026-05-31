//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtLinearVariable.h"

namespace SFBGS {
    AgxNtLinearVariable::AgxNtLinearVariable(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Linear Variable");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Linear Variable", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredValue, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Speed, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

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

    std::shared_ptr<AgxPort> AgxNtLinearVariable::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
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
