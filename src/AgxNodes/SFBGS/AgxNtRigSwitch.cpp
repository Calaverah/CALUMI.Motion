//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtRigSwitch.h"

#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include "Widgets/MultiVariableDialog.h"
#include <Utilities/AgxJsonHelper.h>
namespace SFBGS {
    AgxNtRigSwitch::AgxNtRigSwitch(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = QStringLiteral("Rig Switch");

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Rig Switch", AgxColumnTypes::BasicString));

        AgxPropertyBlockData entryBlock({
            AgxPropertyEntryDefinition(&AgxDictionary::RigName,"",AgxColumnTypes::BasicString)
                                          }, nullptr);

        _PropertyBlocks.insert(&AgxDictionary::Entries, entryBlock);

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    std::shared_ptr<AgxPort> AgxNtRigSwitch::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName("");
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtRigSwitch::name() const
    {
        return QStringLiteral("NT_RIG_SWITCH");
    }

    QString AgxNtRigSwitch::caption() const
    {
        return tr("Rig Switch");
    }

    unsigned int AgxNtRigSwitch::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtRigSwitch::GetNodeType() const
    {
        return AgxNodeType::NT_RIG_SWITCH;
    }
}