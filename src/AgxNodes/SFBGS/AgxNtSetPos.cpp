//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtSetPos.h"

namespace SFBGS {
    AgxNtSetPos::AgxNtSetPos(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Set Pos");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Set Pos", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Type, _VariableTypeList.at(0)().tag, AgxColumnTypes::CustomDropDown, _VariableTypeList));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Time, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PositionVar, "DesiredLocation", AgxColumnTypes::CustomVector));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OrientedOffset, "0;0;0", AgxColumnTypes::BasicVector));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxCorrectionVel, "100000", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxPostCorrectionVel, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::AlignmentBone, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableWhenInactive, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    std::shared_ptr<AgxPort> AgxNtSetPos::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    //sfbgsPort->SetPropertySheetEnabled(false);
                    sfbgsPort->setName("Input");
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

    QString AgxNtSetPos::name() const
    {
        return QStringLiteral("NT_SET_POS");
    }

    QString AgxNtSetPos::caption() const
    {
        return tr("Set Pos");
    }

    unsigned int AgxNtSetPos::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtSetPos::GetNodeType() const
    {
        return AgxNodeType::NT_SET_POS;
    }
}
