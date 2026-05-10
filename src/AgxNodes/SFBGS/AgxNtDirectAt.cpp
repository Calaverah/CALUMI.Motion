//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtDirectAt.h"


namespace SFBGS {
    AgxNtDirectAt::AgxNtDirectAt(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Direct At");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Direct At", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceBone, "DirectAt", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SourceLocationBone, "", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectAtVectorVariable, "LookAtLocation", AgxColumnTypes::CustomVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingDeltaVariable, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableVariable, "bNoHeadTrack", AgxColumnTypes::CustomInteger));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingOffsetOutput, "LookAtHeadingVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::PitchOffsetOutput, "LookAtPitchVar", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::IsActiveVar, "bHeadTrackingActive", AgxColumnTypes::CustomBool));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendRate, "0.3", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMin, "-100", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableHeadingMax, "100", AgxColumnTypes::BasicFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HeadingLagDegrees, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ClampHeadingMin, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ClampHeadingMax, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ClampPitchMin, "", AgxColumnTypes::CustomFloat));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ClampPitchMax, "", AgxColumnTypes::CustomFloat));

        {
            AgxPropertyBlockData entryBlock({
                AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::BoneName, "", AgxColumnTypes::BasicString),
                AgxPropertyEntryDefinition(&AgxDictionary::MinX,"-90",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::MaxX,"90",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::MinY,"-90",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::MaxY,"90",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::MinZ,"-90",AgxColumnTypes::BasicFloat),
                AgxPropertyEntryDefinition(&AgxDictionary::MaxZ,"90",AgxColumnTypes::BasicFloat)
                                            }, nullptr);


            _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);


        }

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    QString AgxNtDirectAt::name() const
    {
        return QStringLiteral("NT_DIRECT_AT");
    }

    QString AgxNtDirectAt::caption() const
    {
        return tr("Direct At");
    }

    unsigned int AgxNtDirectAt::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtDirectAt::GetNodeType() const
    {
        return AgxNodeType::NT_DIRECT_AT;
    }

    std::shared_ptr<AgxPort> AgxNtDirectAt::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if (portType == AgxPortType::In) {
                sfbgsPort->SetName("passthrough");
            }
        }

        return port;
    }
}