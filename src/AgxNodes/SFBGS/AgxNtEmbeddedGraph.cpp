//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNtEmbeddedGraph.h"

#include "Widgets/AgxPropertyBlockWidget.h"
#include "Widgets/MultiVariableDialog.h"
#include <Utilities/AgxJsonHelper.h>
#include <Models/AgxGraphModel.h>
#include "CALUMIMotion.h"
#include "Models/AgxGraphicsScene.h"
namespace SFBGS {
    AgxNtEmbeddedGraph::AgxNtEmbeddedGraph(AgxGraphModel* rootGraphRef) :SFBGSNode(rootGraphRef)
    {
        m_nameProperty = _blendName;

        {
            _blendPropIdx = m_propertyEntries.size();
            m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTreeName, "", AgxColumnTypes::BasicString));
            
            _statePropIdx = m_propertyEntries.size();
            m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StateMachineName, "", AgxColumnTypes::BasicString));
            m_propertyEntries[_statePropIdx].SetEnabledState(false);
        }

        m_propertyEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::SyncOnlyTransitionOut, "False", AgxColumnTypes::BasicBool));

        m_blockOrder = { &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents };

        
    }

    std::shared_ptr<AgxPort> AgxNtEmbeddedGraph::_AddPort(const AgxPortType portType, const AgxPortIndex index, const QJsonObject data)
    {
        auto port = SFBGSNode::_AddPort(portType, index, data);

        if (const auto sfbgsPort = dynamic_cast<AgxPort_SFBGS*>(port.get()))
        {
            if(portType == AgxPortType::In)
                sfbgsPort->setPropertySheetEnabled(true);

            Q_EMIT sfbgsPort->propertySheetUpdated();
        }

        return port;
    }

    QString AgxNtEmbeddedGraph::name() const
    {
        return QStringLiteral("NT_EMBEDDED_GRAPH");
    }

    QString AgxNtEmbeddedGraph::typeName() const
    {
        switch (m_embeddedGraphModel->getGraphType())
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
        switch (m_embeddedGraphModel->getGraphType())
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
        if (m_embeddedGraphModel)
        {
            switch (m_embeddedGraphModel->getGraphType())
            {
                case AgxGraphType::SFBGS_StateMachine:
                    return m_propertyEntries.at(1).value;
                case AgxGraphType::SFBGS_Default:
                    return m_propertyEntries.at(0).value;
                default:
                    break;
            }
        }
        return "Undefined Graph";
    }

    QString AgxNtEmbeddedGraph::SubCaption() const
    {
        return QString("%1 (%2)").arg(SubCaptionUnformatted()).arg(GetPropertyValue(m_sfbgsProperties, AgxDictionary::UserId().tag, "?"));
    }

    unsigned int AgxNtEmbeddedGraph::nPorts(AgxPortType portType) const
    {
        return 1;
    }

    void AgxNtEmbeddedGraph::ToggleCollapse()
    {
        m_collapsed = !m_collapsed;

        if (!_embGraphWidget) return;

        if (!m_collapsed)
            _embGraphWidget->show();
        else
            _embGraphWidget->hide();

    }

    QWidget* AgxNtEmbeddedGraph::embeddedWidget()
    {

        if (!_embGraphWidget) {
            _embGraphWidget = new MiniGraphicsView(m_embeddedGraphScene.get());
            //_MainVBoxLayout->addWidget(miniView);
            _embGraphWidget->setMinimumHeight(50);
            _embGraphWidget->setMinimumWidth(50);

            //_embGraphWidget->setStyleSheet("QWidget {background-color: transparent; border: 0px solid rgba(0,0,0,0);");

            connect(_embGraphWidget, &MiniGraphicsView::clicked, this, [this]
                {
                    const auto widget = QApplication::activeWindow();

                    if (const auto calumiWindow = dynamic_cast<CALUMIMotion*>(widget))
                        calumiWindow->Create_SFBGSTab(m_embeddedGraphScene, m_embeddedGraphModel);
                });
        }

        if (!m_collapsed)
        {
            return _embGraphWidget;
        }

        return nullptr;
    }

    AgxNodeType AgxNtEmbeddedGraph::GetNodeType() const
    {
        if (m_embeddedGraphModel) {
            switch (m_embeddedGraphModel->getGraphType())
            {
                case AgxGraphType::SFBGS_StateMachine:
                    return AgxNodeType::NT_STATE_MACHINE_EMBEDDED;
                case AgxGraphType::SFBGS_Default:
                    return AgxNodeType::NT_BLEND_TREE_EMBEDDED;
                default:
                    break;
            }
        }
        return AgxNodeType::UNDEFINED;
    }

    void AgxNtEmbeddedGraph::SetUpNode(const AgxGameType& type)
    {
        SFBGSNode::SetUpNode(type);
        SetUpEmbeddedNodeGraph();

        if (m_embeddedGraphModel) {
            connect(m_embeddedGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [this] {
                switch (m_embeddedGraphModel->getGraphType())
                {
                    case AgxGraphType::SFBGS_StateMachine:
                        m_nameProperty = _stateName;
                        m_propertyEntries[_statePropIdx].SetEnabledState(true);
                        m_propertyEntries[_blendPropIdx].SetEnabledState(false);
                        break;
                    case AgxGraphType::SFBGS_Default:
                        m_nameProperty = _blendName;
                        m_propertyEntries[_statePropIdx].SetEnabledState(false);
                        m_propertyEntries[_blendPropIdx].SetEnabledState(true);
                        break;
                    default:
                        m_nameProperty = "ERROR: UNDEFINED";
                        m_propertyEntries[_statePropIdx].SetEnabledState(false);
                        m_propertyEntries[_blendPropIdx].SetEnabledState(false);
                        AgxNodeValidationState vState = validationState();
                        vState.m_state = AgxNodeValidationState::State::Error;
                        if (!vState.m_stateMessage.isEmpty()) vState.m_stateMessage += "\n";
                        vState.m_stateMessage += "UNKNOWN EMBEDDED GRAPH TYPE";
                        setValidationState(vState);
                        break;
                }
                Q_EMIT PropertySheetUpdated();
                    });
        }
        if (m_embeddedGraphScene)
        {
            connect(this, &AgxNode::PropertySheetUpdated, m_embeddedGraphScene.get(), [this]
                {
                    const QString tabTitle = SubCaptionUnformatted().isEmpty() ? "graph" : SubCaptionUnformatted();
                    m_embeddedGraphScene->agxGraphModel().SetGraphTitle("embedded_" + tabTitle, false);
                });
        }
    }
}