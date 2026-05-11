//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtAnimationNode.h"

namespace SFBGS {
    AgxNtAnimationNode::AgxNtAnimationNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Animation");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TagName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "", AgxColumnTypes::SyncSystem));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSlave, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnEnd, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariableRefCount, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateRefCountWhenInactive, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplier, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplierIsDuration, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CropStartFrames, "0", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CropEndFrames, "0", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RandomizeStartPoint, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mirror, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ProcessInactiveEvents, "True", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableControl, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtAnimationNode::name() const
    {
        return QStringLiteral("NT_ANIMATION_NODE");
    }

    QString AgxNtAnimationNode::caption() const
    {
        //return _nameProperty;
        return tr("Animation");
    }

    QString AgxNtAnimationNode::SubCaption() const
    {
        QString cap = m_propertyEntries.at(0).value.isEmpty() ? "-" : m_propertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
    }
    

    unsigned int AgxNtAnimationNode::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtAnimationNode::GetNodeType() const
    {
        return AgxNodeType::NT_ANIMATION_NODE;
    }

    std::shared_ptr<AgxPort> AgxNtAnimationNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->setPropertySheetOptional(true);
                sfbgsPort->setName("");
            }
        }

        return port;
    }
}