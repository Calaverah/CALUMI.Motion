//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRootTwist.h"

namespace SFBGS {
    AgxNtRootTwist::AgxNtRootTwist(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        m_nameProperty = QStringLiteral("Root Twist");

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Direction, "Direction", AgxColumnTypes::CustomFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DirectionIsRadians, "False", AgxColumnTypes::BasicBool));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::RootBlendAmount, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::COMBlendAmount, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::HipsBlendAmount, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ThighsBlendAmount, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::ActiveBlendRate, "1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::InactiveBlendRate, "0", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableBefore, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::DisableAfter, "-1", AgxColumnTypes::BasicFloat));
        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::OverrideAnimDirection, "-1", AgxColumnTypes::BasicFloat));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };
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

    std::shared_ptr<AgxPort> AgxNtRootTwist::AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->setName("passthrough");
                    break;
            }
            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

}
