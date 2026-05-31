//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtGamebryoSequence.h"

namespace SFBGS {
    AgxNtGamebryoSequence::AgxNtGamebryoSequence(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Gamebryo Sequence");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SequenceName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::WeightorVariable, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendModeFunction, _BlendList.at(0)().tag, AgxColumnTypes::CustomDropDown, _BlendList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TimePercent, "", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendOutFrames, "0", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtGamebryoSequence::name() const
    {

        return QStringLiteral("NT_GAMEBRYO_SEQUENCE");
    }

    QString AgxNtGamebryoSequence::caption() const
    {
        return tr("Gamebryo Sequence");
    }

    unsigned int AgxNtGamebryoSequence::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    QString AgxNtGamebryoSequence::SubCaption() const
    {
        QString name = m_propertyEntries.at(1).value.isEmpty() ? "-" : m_propertyEntries.at(1).value;

        return QString("%1 (%2)").arg(name).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
    }

    AgxNodeType AgxNtGamebryoSequence::GetNodeType() const
    {
        return AgxNodeType::NT_GAMEBRYO_SEQUENCE;
    }

    std::shared_ptr<AgxPort> AgxNtGamebryoSequence::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->setPropertySheetOptional(false);
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
