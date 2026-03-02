#include "stdafx.h"
#include "AgxNtClonePose.h"

namespace SFBGS {
    AgxNtClonePose::AgxNtClonePose(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Clone Pose");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Clone Pose", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Event, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ResumeEvent, "", AgxColumnTypes::Event));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    std::shared_ptr<AgxPort> AgxNtClonePose::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName(std::format("Input{}",_In_Ports.size()).c_str());
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtClonePose::name() const
    {
        return QStringLiteral("NT_CLONE_POSE");
    }

    QString AgxNtClonePose::caption() const
    {
        return tr("Clone Pose");
    }

    unsigned int AgxNtClonePose::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtClonePose::GetNodeType() const
    {
        return AgxNodeType::NT_CLONE_POSE;
    }

}
