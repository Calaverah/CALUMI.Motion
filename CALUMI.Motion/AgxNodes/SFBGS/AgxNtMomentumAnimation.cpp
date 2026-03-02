#include "stdafx.h"
#include "AgxNtMomentumAnimation.h"

namespace SFBGS {
    AgxNtMomentumAnimation::AgxNtMomentumAnimation(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Momentum Animation");



        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CurrentSpeed, "CurrentSpeed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CurrentDirection, "DirectionSmoothed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredSpeed, "Speed", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DesiredDirection, "Direction", AgxColumnTypes::CustomFloat));

        {
            AgxPropertyBlockData entryBlockDef({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::AnimTag,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::Angle0to1,"0",AgxColumnTypes::BasicInteger)
                                               }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlockDef);
        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtMomentumAnimation::name() const
    {

        return QStringLiteral("NT_MOMENTUM_ANIMATION");
    }

    QString AgxNtMomentumAnimation::caption() const
    {
        return tr("Momentum Animation");
    }

    unsigned int AgxNtMomentumAnimation::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtMomentumAnimation::GetNodeType() const
    {
        return AgxNodeType::NT_MOMENTUM_ANIMATION;
    }

    std::shared_ptr<AgxPort> AgxNtMomentumAnimation::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        //if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        //{
        //    switch (portType)
        //    {
        //        case AgxPortType::In:
        //            sfbgsPort->SetName("passthrough");
        //            break;
        //    }
        //    Q_EMIT sfbgsPort->PropertySheetUpdated();
        //}

        return port;
    }

}
