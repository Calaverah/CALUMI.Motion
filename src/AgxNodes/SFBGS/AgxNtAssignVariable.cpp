//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtAssignVariable.h"

namespace SFBGS {
    AgxNtAssignVariable::AgxNtAssignVariable(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Assign Variable");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Assign Variable", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Operation, _dropDownList.at(0)().tag, AgxColumnTypes::CustomDropDown, _dropDownList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OnlyAssignOnce, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData multiVarBlockDef({
                                                    AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
                                                    AgxPropertyEntryDefinition(&AgxDictionary::VariableName,"",AgxColumnTypes::CustomMultiVar),
                                                    AgxPropertyEntryDefinition(&AgxDictionary::AssignedValue,"0",AgxColumnTypes::BasicMultiVar)
                                                  },nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::Entries, multiVarBlockDef);
        }

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtAssignVariable::name() const
    {
        return QStringLiteral("NT_ASSIGN_VARIABLE");
    }

    QString AgxNtAssignVariable::caption() const
    {
        return tr("Assign Variable");
    }

    unsigned int AgxNtAssignVariable::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtAssignVariable::GetNodeType() const
    {
        return AgxNodeType::NT_ASSIGN_VARIABLE;
    }

    std::shared_ptr<AgxPort> AgxNtAssignVariable::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setPropertySheetOptional(false);
                sfbgsPort->setName("");
                Q_EMIT sfbgsPort->propertySheetUpdated();
            }
        }

        return port;
    }
}