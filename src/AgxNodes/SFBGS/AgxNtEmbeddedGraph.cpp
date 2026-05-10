//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEmbeddedGraph.h"

#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/AgxLineEdit.h"
#include "Widgets/MultiVariableDialog.h"
#include <Utilities/AgxJsonHelper.h>
#include <Models/AgxGraphModel.h>
#include "CALUMIMotion.h"
#include "Models/AgxGraphicsScene.h"
namespace SFBGS {
    AgxNtEmbeddedGraph::AgxNtEmbeddedGraph(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        _nameProperty = _blendName;

        {
            _blendPropIdx = _PropertyEntries.size();
            _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTreeName, "", AgxColumnTypes::BasicString));
            
            _statePropIdx = _PropertyEntries.size();
            _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateMachineName, "", AgxColumnTypes::BasicString));
            _PropertyEntries[_statePropIdx].SetEnabledState(false);
        }

        _PropertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        _BlockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

        
    }

    std::shared_ptr<AgxPort> AgxNtEmbeddedGraph::_AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if(portType == AgxPortType::In)
                sfbgsPort->SetPropertySheetEnabled(true);

            Q_EMIT sfbgsPort->PropertySheetUpdated();
        }

        return port;
    }

    QString AgxNtEmbeddedGraph::name() const
    {
        return QStringLiteral("NT_EMBEDDED_GRAPH");
    }

    QString AgxNtEmbeddedGraph::typeName() const
    {
        switch (_EmbeddedGraphModel->getGraphType())
        {
            case AgxGraphType::SFBGS_StateMachine:
                return QStringLiteral("NT_STATE_MACHINE_EMBEDDED");
            case AgxGraphType::SFBGS_Default:
                return QStringLiteral("NT_BLEND_TREE_EMBEDDED");
            default:
                return name();
        }
    }

    QString AgxNtEmbeddedGraph::caption() const
    {
        switch (_EmbeddedGraphModel->getGraphType())
        {
            case AgxGraphType::SFBGS_StateMachine:
                return tr("State Machine Embedded");
            case AgxGraphType::SFBGS_Default:
                return tr("Blend Tree Embedded");
            default:
                return QString("---");
        }
    }

    QString AgxNtEmbeddedGraph::SubCaptionUnformatted() const
    {
        if (_EmbeddedGraphModel) {
            switch (_EmbeddedGraphModel->getGraphType())
            {
                case AgxGraphType::SFBGS_StateMachine:
                    return _PropertyEntries.at(1).value;
                    break;
                case AgxGraphType::SFBGS_Default:
                    return _PropertyEntries.at(0).value;
                    break;
            }
        }
        return "Undefined Graph";
    }

    QString AgxNtEmbeddedGraph::SubCaption() const
    {
        return QString("%1 (%2)").arg(SubCaptionUnformatted()).arg(GetPropertyValue(_SFBGS_Properties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtEmbeddedGraph::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    void AgxNtEmbeddedGraph::ToggleCollapse()
    {
        collapsed = !collapsed;

        if (!_embGraphWidget) return;

        if (!collapsed)
            _embGraphWidget->show();
        else
            _embGraphWidget->hide();

    }

    QWidget* AgxNtEmbeddedGraph::embeddedWidget()
    {

        if (!_embGraphWidget) {
            _embGraphWidget = new MiniGraphicsView(_EmbeddedGraphScene.get());
            //_MainVBoxLayout->addWidget(miniView);
            _embGraphWidget->setMinimumHeight(50);
            _embGraphWidget->setMinimumWidth(50);

            //_embGraphWidget->setStyleSheet("QWidget {background-color: transparent; border: 0px solid rgba(0,0,0,0);");

            connect(_embGraphWidget, &MiniGraphicsView::clicked, this, [this]() {

                auto widget = QApplication::activeWindow();

                if (auto calumiWindow = dynamic_cast<CALUMIMotion*>(widget))
                    calumiWindow->Create_SFBGSTab(_EmbeddedGraphScene, _EmbeddedGraphModel);

                    });
        }

        if (!collapsed)
        {
            return _embGraphWidget;
        }

        return nullptr;
    }

    AgxNodeType AgxNtEmbeddedGraph::GetNodeType() const
    {
        if (_EmbeddedGraphModel) {
            switch (_EmbeddedGraphModel->getGraphType())
            {
                case AgxGraphType::SFBGS_StateMachine:
                    return AgxNodeType::NT_STATE_MACHINE_EMBEDDED;
                    break;
                case AgxGraphType::SFBGS_Default:
                    return AgxNodeType::NT_BLEND_TREE_EMBEDDED;
                    break;
            }
        }
        return AgxNodeType::UNDEFINED;
    }

    void AgxNtEmbeddedGraph::SetUpNode(const AgxGameType& type)
    {
        SFBGSNode::SetUpNode(type);
        SetUpEmbeddedNodeGraph();

        if (_EmbeddedGraphModel) {
            connect(_EmbeddedGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [this]() {
                switch (_EmbeddedGraphModel->getGraphType())
                {
                    case AgxGraphType::SFBGS_StateMachine:
                        _nameProperty = _stateName;
                        _PropertyEntries[_statePropIdx].SetEnabledState(true);
                        _PropertyEntries[_blendPropIdx].SetEnabledState(false);
                        break;
                    case AgxGraphType::SFBGS_Default:
                        _nameProperty = _blendName;
                        _PropertyEntries[_statePropIdx].SetEnabledState(false);
                        _PropertyEntries[_blendPropIdx].SetEnabledState(true);
                        break;
                    default:
                        _nameProperty = "ERROR: UNDEFINED";
                        _PropertyEntries[_statePropIdx].SetEnabledState(false);
                        _PropertyEntries[_blendPropIdx].SetEnabledState(false);
                        AgxNodeValidationState vState = validationState();
                        vState._state = AgxNodeValidationState::State::Error;
                        if (!vState._stateMessage.isEmpty()) vState._stateMessage += "\n";
                        vState._stateMessage += "UNKNOWN EMBEDDED GRAPH TYPE";
                        setValidatonState(vState);
                        break;
                }
                Q_EMIT PropertySheetUpdated();
                    });
        }
        if (_EmbeddedGraphScene)
        {
            connect(this, &AgxNode::PropertySheetUpdated, _EmbeddedGraphScene.get(), [this]() {
                        QString tabTitle = SubCaptionUnformatted().isEmpty() ? "graph" : SubCaptionUnformatted();
                        _EmbeddedGraphScene->agxGraphModel().SetGraphTitle("embedded_" + tabTitle, false);
                    });
        }
    }
}