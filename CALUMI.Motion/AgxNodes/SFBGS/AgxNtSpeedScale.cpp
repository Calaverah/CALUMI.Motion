//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtSpeedScale.h"

namespace SFBGS {
    AgxNtSpeedScale::AgxNtSpeedScale(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Speed Scale");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredSpeedVar, "Speed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MinScale, "1.0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaxScale, "1.5", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Blendratewhenactive, "1.0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Blendratewheninactive, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableBefore, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableAfter, "-1", AgxColumnTypes::BasicFloat));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtSpeedScale::name() const
    {

        return QStringLiteral("NT_SPEED_SCALE");
    }

    QString AgxNtSpeedScale::caption() const
    {
        return tr("Speed Scale");
    }

    unsigned int AgxNtSpeedScale::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtSpeedScale::GetNodeType() const
    {
        return AgxNodeType::NT_SPEED_SCALE;
    }

    std::shared_ptr<AgxPort> AgxNtSpeedScale::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName("passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
