#include "stdafx.h"
#include "AgxNtParticleSequence.h"

namespace SFBGS {
    AgxNtParticleSequence::AgxNtParticleSequence(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Particle Sequence");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Particle Sequence", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SequenceName, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PlaybackSpeed, "1", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendOutTime, "0.3", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        {
            AgxPropertyBlockData blockDef1({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BindingType,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BindingName,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BindingValue,"",AgxColumnTypes::BasicString)
                                          }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::Bindings, blockDef1);
        }

        {
            AgxPropertyBlockData blockDef2({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BindingName,"",AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::VariableName,"",AgxColumnTypes::CustomFloat)
                                          }, nullptr);

            _PropertyBlocks.insert(&AgxDictionary::BindingVariables, blockDef2);
        }

        _BlockOrder = { &AgxDictionary::Bindings, &AgxDictionary::BindingVariables, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtParticleSequence::name() const
    {
        return QStringLiteral("NT_PARTICLE_SEQUENCE");
    }

    QString AgxNtParticleSequence::caption() const
    {
        return tr("Particle Sequence");
    }

    QString AgxNtParticleSequence::SubCaption() const
    {
        QString cap = _PropertyEntries.at(1).value.isEmpty() ? _PropertyEntries.at(0).value : _PropertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtParticleSequence::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::In:
                return 0;
            default:
                return 1;
        }
    }

    AgxNodeType AgxNtParticleSequence::GetNodeType() const
    {
        return AgxNodeType::NT_PARTICLE_SEQUENCE;
    }

    std::shared_ptr<AgxPort> AgxNtParticleSequence::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetPropertySheetEnabled(true);
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
