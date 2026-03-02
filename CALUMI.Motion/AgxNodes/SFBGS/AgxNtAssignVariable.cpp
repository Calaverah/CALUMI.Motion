#include "stdafx.h"
#include "AgxNtAssignVariable.h"

namespace SFBGS {
    AgxNtAssignVariable::AgxNtAssignVariable(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Assign Variable");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Assign Variable", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Operation, "Assign", AgxColumnTypes::CustomDropDown,{"Assign","Increment","Decrement"}));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OnlyAssignOnce, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData multiVarBlockDef({
                                                    AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
                                                    AgxPropertyEntryDefinition(&AgxDictionary::VariableName,"",AgxColumnTypes::CustomMultiVar),
                                                    AgxPropertyEntryDefinition(&AgxDictionary::AssignedValue,"0",AgxColumnTypes::BasicMultiVar)
                                                  },nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, multiVarBlockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
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

    std::shared_ptr<AgxPort> AgxNtAssignVariable::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetPropertySheetOptional(false);
                sfbgsPort->SetName("");
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}