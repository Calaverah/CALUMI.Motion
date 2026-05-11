//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtStateVariableControl.h"

namespace SFBGS {
    AgxNtStateVariableControl::AgxNtStateVariableControl(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("State Variable Control");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Variable Control", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartValue, "1", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EndValue, "1", AgxColumnTypes::BasicInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariableRefCount, "", AgxColumnTypes::CustomInteger));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateRefCountWhenInactive, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData blockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Event, "", AgxColumnTypes::Event),
                                        AgxPropertyEntryDefinition(&AgxDictionary::AssignValue, "0", AgxColumnTypes::BasicInteger)
                                       }, nullptr);

        m_PropertyBlocks.insert(&AgxDictionary::Entries, blockData);

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtStateVariableControl::name() const
    {

        return QStringLiteral("NT_STATE_VARIABLE_CONTROL");
    }

    QString AgxNtStateVariableControl::caption() const
    {
        return tr("State Variable Control");
    }

    unsigned int AgxNtStateVariableControl::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtStateVariableControl::GetNodeType() const
    {
        return AgxNodeType::NT_STATE_VARIABLE_CONTROL;
    }

    std::shared_ptr<AgxPort> AgxNtStateVariableControl::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->setName(std::format("input{}",m_inPorts.size()).c_str());
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
