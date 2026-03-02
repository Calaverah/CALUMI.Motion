#include "stdafx.h"
#include "AgxNtAssignIState.h"
#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/AgxJsonHelper.h>

namespace SFBGS {

    AgxNtAssignIState::AgxNtAssignIState(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Assign iStates");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Assign iStates", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::iStateVariable, "iState", AgxColumnTypes::CustomInteger));

        {
            AgxPropertyBlockData enterBlockDef({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::iStateName,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::iStateValue,"0",AgxColumnTypes::BasicInteger) }, nullptr);
            //enterBlockDef.SetEnabledState(true);

            _PropertyBlocks.insert(&AgxDictionary::iStateAssignments, enterBlockDef);
        }

        //_BlockOrder = { "iState Assignments", "Enter Events", "Exit Events" };
        _BlockOrder = { &AgxDictionary::iStateAssignments, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtAssignIState::name() const
    {
        return QStringLiteral("NT_ASSIGN_ISTATE");
    }

    QString AgxNtAssignIState::caption() const
    {
        return tr("Assign iStates");
    }

    QString AgxNtAssignIState::SubCaption() const
    {
        QString cap = _PropertyEntries.at(0).value.isEmpty() ? "-" : _PropertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtAssignIState::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtAssignIState::GetNodeType() const
    {
        return AgxNodeType::NT_ASSIGN_ISTATE;
    }
}