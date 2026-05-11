//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtAssignIState.h"
#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include <Utilities/AgxJsonHelper.h>

namespace SFBGS {

    AgxNtAssignIState::AgxNtAssignIState(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Assign iStates");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Assign iStates", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::iStateVariable, "iState", AgxColumnTypes::CustomInteger));

        {
            AgxPropertyBlockData enterBlockDef({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::iStateName,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::iStateValue,"0",AgxColumnTypes::BasicInteger) }, nullptr);
            //enterBlockDef.SetEnabledState(true);

            m_PropertyBlocks.insert(&AgxDictionary::iStateAssignments, enterBlockDef);
        }

        //_BlockOrder = { "iState Assignments", "Enter Events", "Exit Events" };
        m_blockOrder = { &AgxDictionary::iStateAssignments, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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
        QString cap = m_propertyEntries.at(0).value.isEmpty() ? "-" : m_propertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
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