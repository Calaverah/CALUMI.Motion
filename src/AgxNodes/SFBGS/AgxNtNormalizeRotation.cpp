//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtNormalizeRotation.h"

namespace SFBGS {
    AgxNtNormalizeRotation::AgxNtNormalizeRotation(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Normalize Rotation");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Normalize Rotation", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BoneName, "", AgxColumnTypes::BasicString));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NormalizeLength, "90", AgxColumnTypes::BasicInteger));

        m_blockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtNormalizeRotation::name() const
    {

        return QStringLiteral("NT_NORMALIZE_ROTATION");
    }

    QString AgxNtNormalizeRotation::caption() const
    {
        return tr("Normalize Rotation");
    }

    unsigned int AgxNtNormalizeRotation::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtNormalizeRotation::GetNodeType() const
    {
        return AgxNodeType::NT_NORMALIZE_ROTATION;
    }

    std::shared_ptr<AgxPort> AgxNtNormalizeRotation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    if (m_inPorts.size() == 1)
                        sfbgsPort->setName("Passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
