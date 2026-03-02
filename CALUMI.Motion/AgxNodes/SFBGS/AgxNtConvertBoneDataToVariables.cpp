#include "stdafx.h"
#include "AgxNtConvertBoneDataToVariables.h"

namespace SFBGS {
    AgxNtConvertBoneDataToVariables::AgxNtConvertBoneDataToVariables(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Copy Bone To Vars");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Convert Bone to Var", AgxColumnTypes::BasicString));

        {
            AgxPropertyBlockData blockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BoneName,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Axis,"X",AgxColumnTypes::CustomDropDown, _DropDown),
                AgxPropertyEntryDefinition(&AgxDictionary::VariableOut,"",AgxColumnTypes::CustomFloat)
                                          }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, blockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    std::shared_ptr<AgxPort> AgxNtConvertBoneDataToVariables::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                        sfbgsPort->SetName("Passthrough");
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtConvertBoneDataToVariables::name() const
    {
        return QStringLiteral("NT_CONVERT_BONE_DATA_TO_VARIABLES");
    }

    QString AgxNtConvertBoneDataToVariables::caption() const
    {
        return tr("Copy Bone To Vars");
    }

    unsigned int AgxNtConvertBoneDataToVariables::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtConvertBoneDataToVariables::GetNodeType() const
    {
        return AgxNodeType::NT_CONVERT_BONE_DATA_TO_VARIABLES;
    }

}
