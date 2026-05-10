//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEvaluateConditionVariable.h"

#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include "Widgets/MultiVariableDialog.h"
#include <Utilities/AgxJsonHelper.h>
namespace SFBGS {
    AgxNtEvaluateConditionVariable::AgxNtEvaluateConditionVariable(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = "Evaluate Condition Variable";

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::Name, "Evaluate Condition Variable", AgxColumnTypes::BasicString));

        AgxPropertyBlockData multiBlock({
            AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::EvaluatedCondition,"",AgxColumnTypes::BasicString),
            AgxPropertyEntryDefinition(&AgxDictionary::VariableName,"",AgxColumnTypes::CustomFloat)
                                        }, nullptr);


        _PropertyBlocks.insert(&AgxDictionary::Entries, multiBlock);

        _BlockOrder = { &AgxDictionary::Entries, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents};
    }

    std::shared_ptr<AgxPort> AgxNtEvaluateConditionVariable::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetName(QString("input%1").arg(_In_Ports.size()));
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtEvaluateConditionVariable::name() const
    {
        return QStringLiteral("NT_EVALUATE_CONDITION_VARIABLE");
    }

    QString AgxNtEvaluateConditionVariable::caption() const
    {
        return tr("Evaluate Condition Variable");
    }

    unsigned int AgxNtEvaluateConditionVariable::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    AgxNodeType AgxNtEvaluateConditionVariable::GetNodeType() const
    {
        return AgxNodeType::NT_EVALUATE_CONDITION_VARIABLE;
    }
}