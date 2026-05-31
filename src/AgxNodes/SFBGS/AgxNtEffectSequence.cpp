//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEffectSequence.h"

namespace SFBGS {
    AgxNtEffectSequence::AgxNtEffectSequence(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Effect Sequence");
        
        
        
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Effect Sequence", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SequenceName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnEnd, "", AgxColumnTypes::Event));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TimePercent, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TimePercentMin, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TimePercentMax, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SequenceCurrentFrameIndex, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplier, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Weight, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendModeFunction, _ModeList.at(0)().tag, AgxColumnTypes::CustomDropDown, _ModeList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendOutFrames, "0", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HasLoopingSegment, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InitializeSequenceOnLoad, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AllowNoEffect, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtEffectSequence::name() const
    {
        return QStringLiteral("NT_EFFECT_SEQUENCE");
    }

    QString AgxNtEffectSequence::caption() const
    {
        return tr("Effect Sequence");
    }

    unsigned int AgxNtEffectSequence::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    QString AgxNtEffectSequence::SubCaption() const
    {
        QString name = m_propertyEntries.at(1).value.isEmpty() ? "-" : m_propertyEntries.at(1).value;

        return QString("%1 (%2)").arg(name).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
    }

    AgxNodeType AgxNtEffectSequence::GetNodeType() const
    {
        return AgxNodeType::NT_EFFECT_SEQUENCE;
    }

    AgxNodeValidationState AgxNtEffectSequence::validationState() const
    {
        auto validState = SFBGSNode::validationState();

        for (auto& entry : m_propertyEntries)
        {
            if ((entry.Tag().compare("Blend Mode Function") == 0) && entry.value != _ModeList.at(0)().tag)
            {
                if (validState.state() == AgxNodeValidationState::State::Valid) validState.m_state = AgxNodeValidationState::State::Warning;

                QString message = validState.m_stateMessage.isEmpty() ? "" : "\n";
                message += "Blend Mode Function is untested. Use with caution!";
                validState.m_stateMessage += message;
                break;
            }
        }

        return validState;
    }

    std::shared_ptr<AgxPort> AgxNtEffectSequence::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

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