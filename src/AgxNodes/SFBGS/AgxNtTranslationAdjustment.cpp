//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtTranslationAdjustment.h"

namespace SFBGS {
    AgxNtTranslationAdjustment::AgxNtTranslationAdjustment(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Translation Adjustment");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Trans Adjustment", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BoneName, "Camera Control", AgxColumnTypes::BasicString));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Value, "0;0;0.1", AgxColumnTypes::BasicVector));
        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

    }

    QString AgxNtTranslationAdjustment::name() const
    {

        return QStringLiteral("NT_TRANSLATION_ADJUSTMENT");
    }

    QString AgxNtTranslationAdjustment::caption() const
    {
        return tr("Translation Adjustment");
    }

    unsigned int AgxNtTranslationAdjustment::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtTranslationAdjustment::GetNodeType() const
    {
        return AgxNodeType::NT_TRANSLATION_ADJUSTMENT;
    }

    std::shared_ptr<AgxPort> AgxNtTranslationAdjustment::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
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
