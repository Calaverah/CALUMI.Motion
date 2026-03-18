//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtStateVariableControl.h"

namespace SFBGS {
    AgxNtStateVariableControl::AgxNtStateVariableControl(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("State Variable Control");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Variable Control", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartValue, "1", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::EndValue, "1", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariableRefCount, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateRefCountWhenInactive, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData blockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Event, "", AgxColumnTypes::Event),
                                        AgxPropertyEntryDefinition(&AgxDictionary::AssignValue, "0", AgxColumnTypes::BasicInteger)
                                       }, nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Entries, blockData);

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
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
                    sfbgsPort->SetName(std::format("input{}",_In_Ports.size()).c_str());
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
