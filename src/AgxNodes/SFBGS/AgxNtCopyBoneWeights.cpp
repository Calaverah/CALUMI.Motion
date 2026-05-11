//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtCopyBoneWeights.h"

namespace SFBGS {
    AgxNtCopyBoneWeights::AgxNtCopyBoneWeights(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Copy Bone Weights");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Copy Bone Weights", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ControlVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActivateBaseEventList, "", AgxColumnTypes::BasicString));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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
                if (m_inPorts.size() == 1)
                    sfbgsPort->setName("base animations");
                else
                    sfbgsPort->setName(std::format("input{}", m_inPorts.size() - 1).c_str());
                Q_EMIT sfbgsPort->propertySheetUpdated();
            }
        }

        return port;
    }
}