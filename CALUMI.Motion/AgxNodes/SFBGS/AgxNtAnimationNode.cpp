#include "stdafx.h"
#include "AgxNtAnimationNode.h"

namespace SFBGS {
    AgxNtAnimationNode::AgxNtAnimationNode(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Animation");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TagName, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem, "", AgxColumnTypes::SyncSystem));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncSlave, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SendEventOnEnd, "", AgxColumnTypes::Event));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariableRefCount, "", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateRefCountWhenInactive, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplier, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SpeedMultiplierIsDuration, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CropStartFrames, "0", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::CropEndFrames, "0", AgxColumnTypes::BasicInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RandomizeStartPoint, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Mirror, "False", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ProcessInactiveEvents, "True", AgxColumnTypes::BasicBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::VariableControl, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
    }

    QString AgxNtAnimationNode::name() const
    {
        return QStringLiteral("NT_ANIMATION_NODE");
    }

    QString AgxNtAnimationNode::caption() const
    {
        //return _nameProperty;
        return tr("Animation");
    }

    QString AgxNtAnimationNode::SubCaption() const
    {
        QString cap = _PropertyEntries.at(0).value.isEmpty() ? "-" : _PropertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
    }
    

    unsigned int AgxNtAnimationNode::nPorts(AgxPortType portType) const
    {
        switch (portType)
        {
            case AgxPortType::Out:
                return 1;
            default:
                return 0;
        }
    }

    AgxNodeType AgxNtAnimationNode::GetNodeType() const
    {
        return AgxNodeType::NT_ANIMATION_NODE;
    }

    std::shared_ptr<AgxPort> AgxNtAnimationNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetPropertySheetOptional(true);
                sfbgsPort->SetName("");
            }
        }

        return port;
    }
}