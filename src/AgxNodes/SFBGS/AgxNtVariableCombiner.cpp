//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtVariableCombiner.h"

namespace SFBGS {
    AgxNtVariableCombiner::AgxNtVariableCombiner(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Variable Combiner");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Variable Combiner", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Variable1, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Variable2, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Variable3, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OutputVariable, "", AgxColumnTypes::CustomInteger));

        {
            AgxPropertyBlockData varBlock1({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Name,"",AgxColumnTypes::BasicString), 
                AgxPropertyEntryDefinition(&AgxDictionary::Value,"1",AgxColumnTypes::BasicInteger), 
                AgxPropertyEntryDefinition(&AgxDictionary::Output,"0",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::Priority,"-1",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::EventList,"",AgxColumnTypes::BasicString) 
                                               }, nullptr);

            AgxPropertyBlockData varBlock2({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Name,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Value,"1",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::Output,"0",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::Priority,"-1",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::EventList,"",AgxColumnTypes::BasicString)
                                           }, nullptr);

            AgxPropertyBlockData varBlock3({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Name,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Value,"1",AgxColumnTypes::BasicInteger),
                AgxPropertyEntryDefinition(&AgxDictionary::Output,"0",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::Priority,"-1",AgxColumnTypes::BasicInteger) ,
                AgxPropertyEntryDefinition(&AgxDictionary::EventList,"",AgxColumnTypes::BasicString)
                                           }, nullptr);
            

            m_PropertyBlocks.insert(&AgxDictionary::Variable1, varBlock1);
            m_PropertyBlocks.insert(&AgxDictionary::Variable2, varBlock2);
            m_PropertyBlocks.insert(&AgxDictionary::Variable3, varBlock3);
        }
        m_blockOrder = { &AgxDictionary::Variable1,&AgxDictionary::Variable2,&AgxDictionary::Variable3, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtVariableCombiner::name() const
    {

        return QStringLiteral("NT_VARIABLE_COMBINER");
    }

    QString AgxNtVariableCombiner::caption() const
    {
        return tr("Variable Combiner");
    }

    unsigned int AgxNtVariableCombiner::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 0;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtVariableCombiner::GetNodeType() const
    {
        return AgxNodeType::NT_VARIABLE_COMBINER;
    }

}
