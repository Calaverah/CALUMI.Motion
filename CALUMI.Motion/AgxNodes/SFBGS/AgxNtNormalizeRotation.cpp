#include "stdafx.h"
#include "AgxNtNormalizeRotation.h"

namespace SFBGS {
    AgxNtNormalizeRotation::AgxNtNormalizeRotation(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Normalize Rotation");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Normalize Rotation", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BoneName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::NormalizeLength, "90", AgxColumnTypes::BasicInteger));

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
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
                    if (_In_Ports.size() == 1)
                        sfbgsPort->SetName("Passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
