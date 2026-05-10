//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtAnimationImpactSelector.h"

namespace SFBGS {
    SFBGS::AgxNtAnimationImpactSelector::AgxNtAnimationImpactSelector(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Animation Impact Selector");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Impact Selector", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BodyPartVar, "cHitReactionBodyPart", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectionVar, "cHitReactionDir", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ImpactVar, "StaggerMagnitude", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        AgxPropertyBlockData fwdBlockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::ForwardTagName, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BodyPart, _BodyPartList.at(0)().tag, AgxColumnTypes::CustomDropDown, _BodyPartList),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Impact, _ImpactList.at(0)().tag, AgxColumnTypes::CustomDropDown, _ImpactList)
                                          }, nullptr);

        AgxPropertyBlockData leftBlockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::LeftTagName, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BodyPart, _BodyPartList.at(0)().tag, AgxColumnTypes::CustomDropDown, _BodyPartList),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Impact, _ImpactList.at(0)().tag, AgxColumnTypes::CustomDropDown, _ImpactList)
                                           }, nullptr);

        AgxPropertyBlockData backBlockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BackTagName, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BodyPart, _BodyPartList.at(0)().tag, AgxColumnTypes::CustomDropDown, _BodyPartList),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Impact, _ImpactList.at(0)().tag, AgxColumnTypes::CustomDropDown, _ImpactList)
                                           }, nullptr);

        AgxPropertyBlockData rightBlockData({
                                        AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::RightTagName, "", AgxColumnTypes::BasicString),
                                        AgxPropertyEntryDefinition(&AgxDictionary::BodyPart, _BodyPartList.at(0)().tag, AgxColumnTypes::CustomDropDown, _BodyPartList),
                                        AgxPropertyEntryDefinition(&AgxDictionary::Impact, _ImpactList.at(0)().tag, AgxColumnTypes::CustomDropDown, _ImpactList)
                                            }, nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Forward, fwdBlockData);
        _PropertyBlocks.insert(&AgxDictionary::Left, leftBlockData);
        _PropertyBlocks.insert(&AgxDictionary::Back, backBlockData);
        _PropertyBlocks.insert(&AgxDictionary::Right, rightBlockData);

        _BlockOrder = { &AgxDictionary::Forward, &AgxDictionary::Left, &AgxDictionary::Back, &AgxDictionary::Right, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtAnimationImpactSelector::name() const
    {

        return QStringLiteral("NT_ANIMATION_IMPACT_SELECTOR");
    }

    QString AgxNtAnimationImpactSelector::caption() const
    {
        return tr("Animation Impact Selector");
    }

    QString AgxNtAnimationImpactSelector::SubCaption() const
    {
        QString cap = _PropertyEntries.at(0).value.isEmpty() ? "-" : _PropertyEntries.at(0).value;
        return QString("%1 (%2)").arg(cap).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtAnimationImpactSelector::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtAnimationImpactSelector::GetNodeType() const
    {
        return AgxNodeType::NT_ANIMATION_IMPACT_SELECTOR;
    }

    std::shared_ptr<AgxPort> AgxNtAnimationImpactSelector::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetPropertySheetOptional(true);
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

}
