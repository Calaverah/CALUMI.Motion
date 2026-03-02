#include "stdafx.h"
#include "AgxDebugNode.h"

#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include "Widgets/MultiVariableDialog.h"
#include <Utilities/AgxJsonHelper.h>
namespace SFBGS {
    SFBGS::AgxNtDebugNode::AgxNtDebugNode(AgxGraphModel* rootGraphRef) : SFBGSNode(rootGraphRef)
    {
        _nameProperty = "Debug Node";

        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Basic Int", "0", AgxColumnTypes::BasicInteger));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Basic Float", "0", AgxColumnTypes::BasicFloat));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Basic String", "", AgxColumnTypes::BasicString));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Basic Bool", "False", AgxColumnTypes::BasicBool));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Basic Vector", "0;0;0", AgxColumnTypes::BasicVector));

        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Custom Int", "", AgxColumnTypes::CustomInteger));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Custom Float", "", AgxColumnTypes::CustomFloat));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Custom Bool", "", AgxColumnTypes::CustomBool));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Custom Vector", "", AgxColumnTypes::CustomVector));

        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Event Variable", "", AgxColumnTypes::Event));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::TransitionType, "Smooth", AgxColumnTypes::CustomDropDown, { "Smooth","Linear" }));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateVariable, "", AgxColumnTypes::State));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Prefix Variable", "", AgxColumnTypes::Prefix));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Suffix Variable", "", AgxColumnTypes::Suffix));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Action Variable", "", AgxColumnTypes::Action));
        //_PropertyEntries.push_back(AgxPropertyEntryDefinition("Sync Sytem", "", AgxColumnTypes::SyncSystem));
        /*{
            AgxPropertyEntryDefinition testVar("Test Variable", "", AgxColumnTypes::CustomFloat);
            testVar.propertyEnabled = false;
            auto idxTestVar = _PropertyEntries.size();
            _PropertyEntries.push_back(testVar);
            connect(this, &AgxNode::ParentGraphTypeUpdated, this, [this, idxTestVar](const AgxGraphType& type) {
                if (type == AgxGraphType::SFBGS_StateMachine) {
                    _PropertyEntries[idxTestVar].SetEnabledState(true);
                } else {
                    _PropertyEntries[idxTestVar].SetEnabledState(false);
                }

                    });
        }*/

        /*{
            AgxPropertyBlockData d({ AgxPropertyEntryDefinition("Basic Bool","False",AgxColumnTypes::BasicBool), AgxPropertyEntryDefinition("Basic Float","0",AgxColumnTypes::BasicFloat), AgxPropertyEntryDefinition("Basic Integer","0",AgxColumnTypes::BasicInteger), AgxPropertyEntryDefinition("Basic Vector","0;0;0",AgxColumnTypes::BasicVector),AgxPropertyEntryDefinition("Basic String","",AgxColumnTypes::BasicString) }, nullptr);
            d.SetEnabledState(false);
            _PropertyBlocks.insert("Basic Block", d);
            connect(this, &AgxNode::ParentGraphTypeUpdated, this, [this](const AgxGraphType& type) {
                if (type == AgxGraphType::SFBGS_StateMachine) {
                    _PropertyBlocks["Basic Block"].SetEnabledState(true);
                } else {
                    _PropertyBlocks["Basic Block"].SetEnabledState(false);
                }

                    });
        }*/
        /*AgxPropertyBlockData f({ AgxPropertyEntryDefinition("Custom Integer","",AgxColumnTypes::CustomInteger), AgxPropertyEntryDefinition("Custom Float","",AgxColumnTypes::CustomFloat), AgxPropertyEntryDefinition("Custom Bool","",AgxColumnTypes::CustomBool), AgxPropertyEntryDefinition("Custom Vector","",AgxColumnTypes::CustomVector), AgxPropertyEntryDefinition("Custom DropDown","Small",AgxColumnTypes::CustomDropDown,{"Small","Medium","Large"}) }, nullptr);
        _PropertyBlocks.insert("Custom Values", f);
        AgxPropertyBlockData g({ AgxPropertyEntryDefinition(&AgxDictionary::Event,"",AgxColumnTypes::Event), AgxPropertyEntryDefinition("State","",AgxColumnTypes::State), AgxPropertyEntryDefinition(&AgxDictionary::SyncSystem,"",AgxColumnTypes::SyncSystem), AgxPropertyEntryDefinition("Prefix","",AgxColumnTypes::Prefix), AgxPropertyEntryDefinition("Suffix","",AgxColumnTypes::Suffix), AgxPropertyEntryDefinition("Action","",AgxColumnTypes::Action) }, nullptr);
        _PropertyBlocks.insert("Misc Values", g);

        _BlockOrder = { "Basic Block", "Custom Values", "Misc Values", "Enter Events", "Exit Events" };*/
    }

    std::shared_ptr<AgxPort> AgxNtDebugNode::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            switch (portType)
            {
                case AgxPortType::In:
                    sfbgsPort->SetPropertySheetEnabled(true);
                    break;
                case AgxPortType::Out:
                    break;
            }
            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtDebugNode::name() const
    {
        return QStringLiteral("DEBUG_SFBGS");
    }

    QString AgxNtDebugNode::caption() const
    {
        return _nameProperty;
    }

    unsigned int AgxNtDebugNode::nPorts(AgxPortType portType) const
    {
        return 3;
    }

    AgxNodeType AgxNtDebugNode::GetNodeType() const
    {
        return AgxNodeType::DEBUG;
    }

    void AgxNtDebugNode::SetUpNode(const AgxGameType& type)
    {
        SFBGSNode::SetUpNode(type);
        SetUpEmbeddedNodeGraph();
    }
}