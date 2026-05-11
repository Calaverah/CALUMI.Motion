//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtLocomotionBlend.h"

namespace SFBGS {
    AgxNtLocomotionBlend::AgxNtLocomotionBlend(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Locomotion Blend");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedParameter, "Speed", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxBlendTime, "0.3", AgxColumnTypes::BasicFloat));

        AgxPropertyBlockData entryBlock({
                                            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"", AgxColumnTypes::BasicString),
                                            AgxPropertyEntryDefinition(&AgxDictionary::MaxSpeedMult,"1.5", AgxColumnTypes::BasicFloat),
                                            AgxPropertyEntryDefinition(&AgxDictionary::BlendwithNext,"False", AgxColumnTypes::BasicBool),
                                            AgxPropertyEntryDefinition(&AgxDictionary::BlendNextMult,"1", AgxColumnTypes::BasicFloat)
                                        }, nullptr);

        entryBlock.AddRow();
        entryBlock.AddRow();
        entryBlock.AddRow();

        entryBlock.SetRow(0, {"Walk","1.5","False","1"});
        entryBlock.SetRow(1, {"Jog","1.5","False","1"});
        entryBlock.SetRow(2, {"Run","1.5","False","1"});

        m_PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtLocomotionBlend::name() const
    {

        return QStringLiteral("NT_LOCOMOTION_BLEND");
    }

    QString AgxNtLocomotionBlend::caption() const
    {
        return tr("Locomotion Blend");
    }

    unsigned int AgxNtLocomotionBlend::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 3;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtLocomotionBlend::GetNodeType() const
    {
        return AgxNodeType::NT_LOCOMOTION_BLEND;
    }

    std::shared_ptr<AgxPort> AgxNtLocomotionBlend::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    switch (m_inPorts.size())
                    {
                        case 1:
                            sfbgsPort->setName("Walk");
                            break;
                        case 2:
                            sfbgsPort->setName("Jog");
                            break;
                        case 3:
                            sfbgsPort->setName("Run");
                            break;
                    }
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
