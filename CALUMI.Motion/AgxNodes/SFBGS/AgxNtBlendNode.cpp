#include "stdafx.h"
#include "AgxNtBlendNode.h"

namespace SFBGS {
	AgxNtBlendNode::AgxNtBlendNode(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
	{
        _nameProperty = QStringLiteral("Blend");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableName, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableType, _VariableTypeList.at(0), AgxColumnTypes::CustomDropDown,_VariableTypeList));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InputLockTime, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMin, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RangeMax, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WrapToRange, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBlendTime, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NormalizeWeights, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OnlyHighestWeightActive, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData blockDef({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntryE,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Param,"0",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::StoryManagerEvent,"False",AgxColumnTypes::BasicBool),
                AgxPropertyEntryDefinition(&AgxDictionary::EnableEvents,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::DisableEvents,"",AgxColumnTypes::BasicString)
                                          }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, blockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
	}

    std::shared_ptr<AgxPort> AgxNtBlendNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    //sfbgsPort->SetPropertySheetEnabled(false);
                    sfbgsPort->SetName(std::format("input{}",_In_Ports.size()).c_str());
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
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
