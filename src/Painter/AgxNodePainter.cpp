// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScopeInitStatement
#include "stdafx.h"
#include "AgxNodePainter.h"
#pragma warning(push,0)
#include "Models/AgxNodeGraphicsObject.h"
#include "AgxNodeGeometry.h"
#pragma warning(pop)
#include "Models/AgxGraphicsScene.h"
#include <Utilities/AgxGraphRegistry.h>
#include <Utilities/AgxConnectionIdUtils.h>
#include "AgxPalette.h"

void AgxNodePainter::paint(QPainter* painter, AgxNodeGraphicsObject& ngo)
{
    m_nodePalette = &AgxPalette::GetInstance().nodePalette();

    if (!m_nodePalette)
        qFatal() << "AgxNodePainter::paint(QPainter* painter, AgxNodeGraphicsObject& ngo): node palette could not be "
                    "referenced";

    m_nodeId = ngo.nodeId();

    if(!m_model)
        m_model = &ngo.agxNodeScene()->agxGraphModel();

    if(!m_geometry)
        m_geometry = &ngo.agxNodeScene()->agxNodeGeometry();

    if (!m_geometry || !m_model) return;

    if (const auto var = m_model->nodeData(m_nodeId, AgxNodeRole::ValidationState); var.canConvert<AgxNodeValidationState>())
    {
        m_vState = var.value<AgxNodeValidationState>();
    }
    else
    {
        m_vState.m_stateMessage = "AGX NODE PAINT ERROR PASSING VALIDATION STATE";
        m_vState.m_state = AgxNodeValidationState::State::Error;
    }

    m_collapsed = m_model->nodeData(m_nodeId, AgxNodeRole::CollapseState).toBool();

    // ReSharper disable once CppTooWideScopeInitStatement
    const qreal lodPercentage = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());

    if (lodPercentage > 0.5)
        m_lod = 0;
    else if (lodPercentage > 0.2)
        m_lod = 1;
    else
        m_lod = 2;

    drawNodeRect(painter, ngo);
    
    if(m_lod < 3)
        drawConnectionPoints(painter, ngo);

    if(m_lod < 1 && !m_collapsed)
    {
        drawFilledConnectionPoints(painter, ngo);
    }

    if(m_lod <2) {
        drawNodeCaption(painter, ngo);
        drawValidationIcon(painter, ngo);
    }

    if(!m_collapsed && m_lod < 1) {
        drawEntryLabels(painter, ngo);
        drawResizeRect(painter, ngo);
    }
}

void AgxNodePainter::drawNodeRect(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{

    QSize size = m_collapsed ? m_geometry->collapsedSize(m_nodeId) : m_geometry->size(m_nodeId);

    /*QColor color = ngo.isSelected() ? nodeStyle.SelectedBoundaryColor
        : nodeStyle.NormalBoundaryColor;*/

    if(m_lod < 3){
        QColor color;

        if (ngo.isSelected()) {
            color = m_nodePalette->SelectedBoundaryColor;
        } else {
            QString gid = m_model->GetNodeGroup(m_nodeId);
            color = m_model->GroupExists(gid) ? m_model->GetGroupColor(gid) : m_nodePalette->NormalBoundaryColor;
        }

        //model.SetNodeSidebarVisibility(nodeId, ngo.isSelected());
        bool dashed = false;

        switch (m_vState.m_state) {
            case AgxNodeValidationState::State::Error:
                color = m_nodePalette->ErrorColor;
                dashed = true;
                break;
            case AgxNodeValidationState::State::Warning:
                color = m_nodePalette->WarningColor;
                dashed = true;
                break;
            default:
                break;
        }


        float penWidth;
        if (ngo.nodeState().hovered())
        {
            penWidth = m_nodePalette->HoveredPenWidth;
        } else {
            penWidth = m_nodePalette->PenWidth;
        }


        if (m_vState.m_state != AgxNodeValidationState::State::Valid) {
            float factor = m_vState.m_state == AgxNodeValidationState::State::Error ? 3.0f : 2.0f;
            penWidth *= factor;
        }
        auto pstyle = dashed ? Qt::PenStyle::DotLine : Qt::PenStyle::SolidLine;
        QPen p(color, penWidth, pstyle);
        painter->setPen(p);
    }
    
    //QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(2.0, size.height()));

    // ReSharper disable once CppTooWideScopeInitStatement
    bool canBeAlt = AgxGraphRegistry::GetInstance().GetGraphDefinition(m_model->getGraphType())._hasDefault;

    if (m_lod < 3 && m_model->nodeData(m_nodeId, AgxNodeRole::AlternateState).toBool() && canBeAlt)
    {
        painter->setBrush(m_nodePalette->AlternateStateColor);
    }
    else
    {
        painter->setBrush(m_nodePalette->NormalStateColor);
    }


    QRectF boundary(0, 0, size.width(), size.height());

    constexpr double radius = 3.0;

    painter->drawRoundedRect(boundary, radius, radius);
}

void AgxNodePainter::drawConnectionPoints(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    const auto reducedDiameter = m_nodePalette->ConnectionPointDiameter * 0.6;

    for (const AgxPortType portType : {AgxPortType::Out, AgxPortType::In})
    {
        const auto portCountRole = portType == AgxPortType::Out ? AgxNodeRole::OutPortCount
            : AgxNodeRole::InPortCount;
        size_t const n = m_model->nodeData(m_nodeId, portCountRole).toUInt();
        const size_t nC = m_collapsed ? 1 : n;

        for (AgxPortIndex portIndex = 0; portIndex < n && portIndex < nC; ++portIndex)
        {
            QPointF poi = m_geometry->portPosition(m_nodeId, portType, portIndex);

            const auto& [portId, portName] = m_model->portData(m_nodeId, portType, portIndex, AgxPortRole::DataType).value<AgxNodeDataType>();

            double r = 1.0;

            AgxNodeState const& state = ngo.nodeState();

            if (auto const* cgo = state.connectionForReaction())
            {
                const AgxPortType requiredPort = cgo->connectionState().requiredPort();

                if (requiredPort == portType)
                {
                    const AgxConnectionId possibleConnectionId = makeCompleteConnectionId(cgo->connectionId(),
                                                                                            m_nodeId,
                                                                                            portIndex);

                    bool const possible = m_model->connectionPossible(possibleConnectionId);

                    auto cp = cgo->sceneTransform().map(cgo->endPoint(requiredPort));
                    cp = ngo.sceneTransform().inverted().map(cp);

                    auto diff = cp - poi;
                    const double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                    if (possible)
                    {
                        constexpr double threshold = 40.0;
                        r = dist < threshold ? 2.0 - dist / threshold : 1.0;
                    }
                    else
                    {
                        constexpr double threshold = 80.0;
                        r = dist < threshold ? dist / threshold : 1.0;
                    }
                }
            }

            painter->setBrush(m_nodePalette->ConnectionPointColor);
            painter->drawEllipse(poi, reducedDiameter * r, reducedDiameter * r);
        }
    }

    if (ngo.nodeState().connectionForReaction())
    {
        ngo.nodeState().resetConnectionForReaction();
    }
}

void AgxNodePainter::drawFilledConnectionPoints(QPainter* painter, [[maybe_unused]] AgxNodeGraphicsObject& ngo) const
{
    const auto reducedDiameter = m_nodePalette->ConnectionPointDiameter * 0.4;

    for (const AgxPortType portType : {AgxPortType::Out, AgxPortType::In})
    {
        size_t const n = m_model->nodeData( m_nodeId, portType == AgxPortType::Out ? AgxNodeRole::OutPortCount : AgxNodeRole::InPortCount).toUInt();

        for (AgxPortIndex portIndex = 0; portIndex < n; ++portIndex)
        {
            QPointF p = m_geometry->portPosition(m_nodeId, portType, portIndex);

            auto const& connected = m_model->connections(m_nodeId, portType, portIndex);

            if (!connected.empty())
            {
                const auto& [portId, portName] = m_model->portData(m_nodeId, portType, portIndex, AgxPortRole::DataType).value<AgxNodeDataType>();

                painter->setPen(m_nodePalette->FilledConnectionPointColor);
                painter->setBrush(m_nodePalette->FilledConnectionPointColor);

                painter->drawEllipse(p, reducedDiameter, reducedDiameter);
            }
        }
    }
}

void AgxNodePainter::drawNodeCaption(QPainter* painter, [[maybe_unused]] AgxNodeGraphicsObject& ngo) const
{
    QFont f = painter->font();

    const QString title = m_collapsed ? ShortenString(m_model->nodeData(m_nodeId, AgxNodeRole::Caption).toString()) : m_model->nodeData(m_nodeId, AgxNodeRole::Caption).toString();
    const QString subtitle = ShortenString(m_model->nodeData(m_nodeId, AgxNodeRole::SubCaption).toString(),30);
    auto [capPos, subPos] = m_geometry->dualCaptionPosition(m_nodeId, title, subtitle, m_collapsed);

    if (!subtitle.isEmpty() && !m_collapsed && m_lod < 1)
    {
        painter->setFont(f);
        painter->setPen(m_nodePalette->FadedFontColor);
        painter->drawText(subPos, subtitle);
        
    }

    f.setBold(true);
    painter->setFont(f);
    painter->setPen(m_nodePalette->NormalFontColor);
    painter->drawText(capPos, title);
    f.setBold(false);
    
    painter->setFont(f);
    painter->setPen(m_nodePalette->NormalFontColor);
}

void AgxNodePainter::drawEntryLabels(QPainter* painter, [[maybe_unused]] AgxNodeGraphicsObject& ngo) const
{
    for (const AgxPortType portType : {AgxPortType::Out, AgxPortType::In})
    {
        const unsigned int n = m_model->nodeData(m_nodeId, portType == AgxPortType::Out ? AgxNodeRole::OutPortCount : AgxNodeRole::InPortCount).value<unsigned int>();

        for (AgxPortIndex portIndex = 0; portIndex < n; ++portIndex)
        {
            const auto& connected = m_model->connections(m_nodeId, portType, portIndex);

            QPointF p = m_geometry->portTextPosition(m_nodeId, portType, portIndex);

            if (connected.empty())
                painter->setPen(m_nodePalette->FadedFontColor);
            else
                painter->setPen(m_nodePalette->NormalFontColor);

            QString s;

            if (m_model->portData(m_nodeId, portType, portIndex, AgxPortRole::CaptionVisible).value<bool>())
            {
                s = m_model->portData(m_nodeId, portType, portIndex, AgxPortRole::Caption).value<QString>();
            }
            else
            {
                auto portData = m_model->portData(m_nodeId, portType, portIndex, AgxPortRole::DataType);

                s = portData.value<AgxNodeDataType>().name;
            }

            painter->drawText(p, s);
        }
    }
}

void AgxNodePainter::drawResizeRect(QPainter* painter, [[maybe_unused]] AgxNodeGraphicsObject& ngo) const
{
    if (m_model->nodeFlags(m_nodeId) & AgxNodeFlag::Resizable)
    {
        painter->setBrush(Qt::gray);
        painter->drawEllipse(m_geometry->resizeHandleRect(m_nodeId));
    }
}

void AgxNodePainter::drawValidationIcon(QPainter* painter, [[maybe_unused]] AgxNodeGraphicsObject& ngo) const
{
    if (m_vState.m_state == AgxNodeValidationState::State::Valid)
        return;

    const QSize size = m_geometry->size(m_nodeId);

    const QIcon icon(":/CALUMIMotion/Resources/info-tooltip.svg");
    constexpr QSize iconSize(8, 8);
    QPixmap pixmap = icon.pixmap(iconSize);

    const QColor color = m_vState.m_state == AgxNodeValidationState::State::Error ? m_nodePalette->ErrorColor : m_nodePalette->WarningColor;

    QPointF center(size.width(), 0.0);
    center += QPointF(iconSize.width() / 2.0, -iconSize.height() / 2.0);

    painter->save();

    // Draw a colored circle behind the icon to highlight validation issues
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawEllipse(center, iconSize.width() / 2.0 + 2.0, iconSize.height() / 2.0 + 2.0);


    QPainter imgPainter(&pixmap);
    imgPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    imgPainter.fillRect(pixmap.rect(), m_nodePalette->NormalFontColor);
    imgPainter.end();


    painter->drawPixmap(center.toPoint() - QPoint(iconSize.width() / 2, iconSize.height() / 2),
        pixmap);

    painter->restore();
}

void AgxNodePainter::drawWrappedTextInWidgetBounds(QPainter* painter, const AgxNodeGraphicsObject& ngo, [[maybe_unused]] const QString& string) const
{
    if (!ngo.m_proxyWidget)
        return;

    const QString temp = "This is a very long string that will be too long for the comment widget and should overflow easily and certainly get past the barrier or whatever it's called. The bounds? Idk man";

    auto bounds = ngo.m_proxyWidget->boundingRect();
    bounds.moveTo(m_geometry->widgetPosition(m_nodeId));
    painter->drawText(bounds, Qt::TextWordWrap | Qt::AlignLeft, temp);
}
