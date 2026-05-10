//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtCurvedPathBlender.h"

namespace SFBGS {
    AgxNtCurvedPathBlender::AgxNtCurvedPathBlender(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Curved Path Blender");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Curved Path Blender", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.05", AgxColumnTypes::BasicFloat));
        
        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtCurvedPathBlender::name() const
    {

        return QStringLiteral("NT_CURVED_PATH_BLENDER");
    }

    QString AgxNtCurvedPathBlender::caption() const
    {
        return tr("Curved Path Blender");
    }

    unsigned int AgxNtCurvedPathBlender::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtCurvedPathBlender::GetNodeType() const
    {
        return AgxNodeType::NT_CURVED_PATH_BLENDER;
    }

    std::shared_ptr<AgxPort> AgxNtCurvedPathBlender::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    if (_In_Ports.size() == 1)
                        sfbgsPort->SetName("default");
                    else
                        sfbgsPort->SetName(std::format("Input{}", _In_Ports.size()).c_str());
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
