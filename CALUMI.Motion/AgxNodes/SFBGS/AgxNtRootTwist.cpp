#include "stdafx.h"
#include "AgxNtRootTwist.h"

namespace SFBGS {
    AgxNtRootTwist::AgxNtRootTwist(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Root Twist");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Direction, "Direction", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectionIsRadians, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RootBlendAmount, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::COMBlendAmount, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HipsBlendAmount, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ThighsBlendAmount, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActiveBlendRate, "1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InactiveBlendRate, "0", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableBefore, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableAfter, "-1", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OverrideAnimDirection, "-1", AgxColumnTypes::BasicFloat));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtRootTwist::name() const
    {

        return QStringLiteral("NT_ROOT_TWIST");
    }

    QString AgxNtRootTwist::caption() const
    {
        return tr("Root Twist");
    }

    unsigned int AgxNtRootTwist::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtRootTwist::GetNodeType() const
    {
        return AgxNodeType::NT_ROOT_TWIST;
    }

    std::shared_ptr<AgxPort> AgxNtRootTwist::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
