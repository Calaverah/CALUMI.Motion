//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtBlendNode.h"

namespace SFBGS {
	AgxNtBlendNode::AgxNtBlendNode(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
	{
        m_nameProperty = QStringLiteral("Blend");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableName, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableType, _VariableTypeList.at(0)().tag, AgxColumnTypes::CustomDropDown, _VariableTypeList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InputLockTime, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMin, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMax, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WrapToRange, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBlendTime, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NormalizeWeights, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OnlyHighestWeightActive, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData blockDef({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntryE,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Param,"0",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::StoryManagerEvent,"False",AgxColumnTypes::BasicBool),
                AgxPropertyEntryDefinition(&AgxDictionary::EnableEvents,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::DisableEvents,"",AgxColumnTypes::BasicString)
                                          }, nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::Entries, blockDef);
        }

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
	}

    std::shared_ptr<AgxPort> AgxNtBlendNode::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    //sfbgsPort->SetPropertySheetEnabled(false);
                    sfbgsPort->setName(std::format("input{}",m_inPorts.size()).c_str());
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }
        
        return port;
    }

    QString AgxNtBlendNode::name() const
    {
        return QStringLiteral("NT_BLEND_NODE");
    }

    QString AgxNtBlendNode::caption() const
    {
        return tr("Blend");
    }

    unsigned int AgxNtBlendNode::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 2;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtBlendNode::GetNodeType() const
    {
        return AgxNodeType::NT_BLEND_NODE;
    }

}
