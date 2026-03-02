#include "stdafx.h"
#include "AgxNtCopyBoneWeights.h"

namespace SFBGS {
    AgxNtCopyBoneWeights::AgxNtCopyBoneWeights(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Copy Bone Weights");
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Copy Bone Weights", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActivateBaseEventList, "", AgxColumnTypes::BasicString));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtCopyBoneWeights::name() const
    {
        return QStringLiteral("NT_COPY_BONE_WEIGHTS");
    }

    QString AgxNtCopyBoneWeights::caption() const
    {
        return tr("Copy Bone Weights");
    }

    unsigned int AgxNtCopyBoneWeights::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 3;
        }
    }

    AgxNodeType AgxNtCopyBoneWeights::GetNodeType() const
    {
        return AgxNodeType::NT_COPY_BONE_WEIGHTS;
    }

    std::shared_ptr<AgxPort> AgxNtCopyBoneWeights::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                if (_In_Ports.size() == 1)
                    sfbgsPort->SetName("base animations");
                else
                    sfbgsPort->SetName(std::format("input{}", _In_Ports.size() - 1).c_str());
                Q_EMIT sfbgsPort->PropertySheetUpdated();
            }
        }

        return port;
    }
}