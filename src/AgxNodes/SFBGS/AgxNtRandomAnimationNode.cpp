//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRandomAnimationNode.h"

namespace SFBGS {
    AgxNtRandomAnimationNode::AgxNtRandomAnimationNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Random Animation");
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "", AgxColumnTypes::SyncSystem));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSlave, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnEnd, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariableRefCount, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateRefCountWhenInactive, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplier, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableName, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RepickAtEnd, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ChancetoRepick, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Crossblendtime, "0.5", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EnumerateTag, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::LimitToSubgraph, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData pickWgt({ 
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::PickWeight,"0",AgxColumnTypes::BasicFloat), 
                AgxPropertyEntryDefinition(&AgxDictionary::Tag_Name,"",AgxColumnTypes::BasicString)}, nullptr);

            m_PropertyBlocks.insert(&AgxDictionary::PickWeights, pickWgt);
            
        }

        m_blockOrder = { &AgxDictionary::PickWeights, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtRandomAnimationNode::name() const
    {
        return QStringLiteral("NT_RANDOM_ANIMATION_NODE");
    }

    QString AgxNtRandomAnimationNode::caption() const
    {
        return tr("Random Animation");
    }

    unsigned int AgxNtRandomAnimationNode::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtRandomAnimationNode::GetNodeType() const
    {
        return AgxNodeType::NT_RANDOM_ANIMATION_NODE;
    }

    std::shared_ptr<AgxPort> AgxNtRandomAnimationNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setPropertySheetOptional(true);
                sfbgsPort->setName("");
                Q_EMIT sfbgsPort->propertySheetUpdated();
            }
        }

        return port;
    }
}