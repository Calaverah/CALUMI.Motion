#include "stdafx.h"
#include "AgxNodePainter.h"
#pragma warning(push,0)

#include "AgxStyleCollection.h"
#include <QtCore/QMargins>
#include <cmath>
#include "Models/AgxNodeGraphicsObject.h"
#include "AgxNodeGeometry.h"
#pragma warning(pop)
#include "Models/AgxGraphicsScene.h"
#include <Widgets/MiniGraphicsView.h>
#include <Utilities/AgxGraphRegistry.h>
#include <Utilities/AgxConnectionIdUtils.h>

void AgxNodePainter::paint(QPainter* painter, AgxNodeGraphicsObject& ngo)
{
    // TODO?
    //AbstractNodeGeometry & geometry = ngo.nodeScene()->nodeGeometry();
    //geometry.recomputeSizeIfFontChanged(painter->font());

    _nodeId = ngo.nodeId();

    if(!_model)
        _model = &ngo.agxNodeScene()->agxGraphModel();

    _nodeStyle = QJsonDocument::fromVariant(_model->nodeData(_nodeId, AgxNodeRole::Style)).object();

    if(!_geometry)
        _geometry = &ngo.agxNodeScene()->agxNodeGeometry();

    if (!_geometry || !_model) return;

    auto var = _model->nodeData(_nodeId, AgxNodeRole::ValidationState);
    if (var.canConvert<AgxNodeValidationState>()) {
        _vState = var.value<AgxNodeValidationState>();
    } else { 
        _vState._stateMessage = "AGX NODE PAINT ERROR PASSING VALIDATION STATE";
        _vState._state = AgxNodeValidationState::State::Error;
    }

    _collapsed = _model->nodeData(_nodeId, AgxNodeRole::CollapseState).toBool();

    qreal lodPercentage = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());

    if (lodPercentage > 0.5) _lod = 0;
    else if (lodPercentage > 0.2) _lod = 1;
    else _lod = 2;

    drawNodeRect(painter, ngo);
    
    if(_lod < 3)
        drawConnectionPoints(painter, ngo);

    if(_lod < 1 && !_collapsed)
    {
        drawFilledConnectionPoints(painter, ngo);
    }

    if(_lod <2) {
        drawNodeCaption(painter, ngo);
        drawValidationIcon(painter, ngo);
    }

    if(!_collapsed && _lod < 1) {
        drawEntryLabels(painter, ngo);
        drawResizeRect(painter, ngo);
    }
}

void AgxNodePainter::drawNodeRect(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{

    QSize size = _collapsed ? _geometry->collapsedSize(_nodeId) : _geometry->size(_nodeId);

    /*QColor color = ngo.isSelected() ? nodeStyle.SelectedBoundaryColor
        : nodeStyle.NormalBoundaryColor;*/

    if(_lod < 3){
        QColor color;

        if (ngo.isSelected()) {
            color = _nodeStyle.SelectedBoundaryColor;
        } else {
            QString gid = _model->GetNodeGroup(_nodeId);
            color = _model->GroupExists(gid) ? _model->GetGroupColor(gid) : _nodeStyle.NormalBoundaryColor;
        }

        //model.SetNodeSidebarVisibility(nodeId, ngo.isSelected());
        bool dashed = false;

        switch (_vState._state) {
            case AgxNodeValidationState::State::Error:
                color = _nodeStyle.ErrorColor;
                dashed = true;
                break;
            case AgxNodeValidationState::State::Warning:
                color = _nodeStyle.WarningColor;
                dashed = true;
                break;
            default:
                break;
        }


        float penWidth;
        if (ngo.nodeState().hovered())
        {
            penWidth = _nodeStyle.HoveredPenWidth;
        } else {
            penWidth = _nodeStyle.PenWidth;
        }


        if (_vState._state != AgxNodeValidationState::State::Valid) {
            float factor = (_vState._state == AgxNodeValidationState::State::Error) ? 3.0f : 2.0f;
            penWidth *= factor;
        }
        auto pstyle = dashed ? Qt::PenStyle::DotLine : Qt::PenStyle::SolidLine;
        QPen p(color, penWidth, pstyle);
        painter->setPen(p);
    }
    
    //QLinearGradient gradient(QPointF(0.0, 0.0), QPointF(2.0, size.height()));
    bool canBeAlt = AgxGraphRegistry::GetInstance().GetGraphDefinition(_model->getGraphType())._hasDefault;

    if (_lod < 3 && _model->nodeData(_nodeId, AgxNodeRole::AlternateState).toBool() && canBeAlt) {
        /*gradient.setColorAt(0.0, Qt::darkCyan);
        gradient.setColorAt(0.10, Qt::darkCyan);
        gradient.setColorAt(0.90, Qt::darkCyan);
        gradient.setColorAt(1.0, Qt::darkCyan);*/
        painter->setBrush(Qt::darkCyan);
    } else {
        /*gradient.setColorAt(0.0, _nodeStyle.GradientColor0);
        gradient.setColorAt(0.10, _nodeStyle.GradientColor1);
        gradient.setColorAt(0.90, _nodeStyle.GradientColor2);
        gradient.setColorAt(1.0, _nodeStyle.GradientColor3);*/
        painter->setBrush(_nodeStyle.GradientColor0);
    }


    QRectF boundary(0, 0, size.width(), size.height());

    double const radius = 3.0;

    painter->drawRoundedRect(boundary, radius, radius);
}

void AgxNodePainter::drawConnectionPoints(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    auto const& connectionStyle = AgxStyleCollection::connectionStyle();

    auto reducedDiameter = _nodeStyle.ConnectionPointDiameter * 0.6;

    for (AgxPortType portType : {AgxPortType::Out, AgxPortType::In}) {

        auto portCountRole = (portType == AgxPortType::Out) ? AgxNodeRole::OutPortCount
            : AgxNodeRole::InPortCount;
        size_t const n = _model->nodeData(_nodeId, portCountRole).toUInt();
        const size_t nC = _collapsed ? 1 : n;

        for (AgxPortIndex portIndex = 0; portIndex < n && portIndex < nC; ++portIndex) {
            QPointF poi = _geometry->portPosition(_nodeId, portType, portIndex);

            auto const& dataType = _model->portData(_nodeId, portType, portIndex, AgxPortRole::DataType).value<AgxNodeDataType>();

            double r = 1.0;

            AgxNodeState const& state = ngo.nodeState();

            if (auto const* cgo = state.connectionForReaction()) {
                AgxPortType requiredPort = cgo->connectionState().requiredPort();

                if (requiredPort == portType) {
                    AgxConnectionId possibleConnectionId = makeCompleteConnectionId(cgo->connectionId(),
                        _nodeId,
                        portIndex);

                    bool const possible = _model->connectionPossible(possibleConnectionId);

                    auto cp = cgo->sceneTransform().map(cgo->endPoint(requiredPort));
                    cp = ngo.sceneTransform().inverted().map(cp);

                    auto diff = cp - poi;
                    double dist = std::sqrt(QPointF::dotProduct(diff, diff));

                    if (possible) {
                        double const thres = 40.0;
                        r = (dist < thres) ? (2.0 - dist / thres) : 1.0;
                    }
                    else {
                        double const thres = 80.0;
                        r = (dist < thres) ? (dist / thres) : 1.0;
                    }
                }
            }

            if (connectionStyle.useDataDefinedColors()) {
                painter->setBrush(connectionStyle.normalColor(dataType.id));
            }
            else {
                painter->setBrush(_nodeStyle.ConnectionPointColor);
            }

            painter->drawEllipse(poi, reducedDiameter * r, reducedDiameter * r);
        }
    }

    if (ngo.nodeState().connectionForReaction()) {
        ngo.nodeState().resetConnectionForReaction();
    }
}

void AgxNodePainter::drawFilledConnectionPoints(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{   

    auto reducedDiameter = _nodeStyle.ConnectionPointDiameter * 0.4;

    for (AgxPortType portType : {AgxPortType::Out, AgxPortType::In}) {
        size_t const n = _model->nodeData( _nodeId, (portType == AgxPortType::Out) ? AgxNodeRole::OutPortCount : AgxNodeRole::InPortCount).toUInt();

        for (AgxPortIndex portIndex = 0; portIndex < n; ++portIndex) {
            QPointF p = _geometry->portPosition(_nodeId, portType, portIndex);

            auto const& connected = _model->connections(_nodeId, portType, portIndex);

            if (!connected.empty()) {
                auto const& dataType = _model->portData(_nodeId, portType, portIndex, AgxPortRole::DataType).value<AgxNodeDataType>();

                auto const& connectionStyle = AgxStyleCollection::connectionStyle();
                if (connectionStyle.useDataDefinedColors()) {
                    QColor const c = connectionStyle.normalColor(dataType.id);
                    painter->setPen(c);
                    painter->setBrush(c);
                }
                else {
                    painter->setPen(_nodeStyle.FilledConnectionPointColor);
                    painter->setBrush(_nodeStyle.FilledConnectionPointColor);
                }

                painter->drawEllipse(p, reducedDiameter, reducedDiameter);
            }
        }
    }
}

void AgxNodePainter::drawNodeCaption(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    //auto internalData = model.nodeData(nodeId, AgxNodeRole::InternalData).toJsonObject();
    //auto collapsed = internalData["internal-data"].toObject()["collapsed"].toBool();

    //auto pSheetData = internalData["internal-data"].toObject()["property-sheet"].toObject();

    QFont f = painter->font();

    const QString title = _collapsed ? ShortenString(_model->nodeData(_nodeId, AgxNodeRole::Caption).toString()) : _model->nodeData(_nodeId, AgxNodeRole::Caption).toString();
    const QString subtitle = ShortenString(_model->nodeData(_nodeId, AgxNodeRole::SubCaption).toString(),30);
    auto Positions = _geometry->dualCaptionPosition(_nodeId, title, subtitle, _collapsed);

    if (!subtitle.isEmpty() && !_collapsed && _lod < 1) {
        //if (pSheetData["Name"].toObject().contains("value") && !collapsed) 


        painter->setFont(f);
        painter->setPen(_nodeStyle.FontColorFaded);
        painter->drawText(Positions.second, subtitle);
        //painter->drawPoint(Positions.second);
        
    }

    f.setBold(true);
    painter->setFont(f);
    painter->setPen(_nodeStyle.FontColor);
    //painter->drawPoint(Positions.first);
    painter->drawText(Positions.first, title);
    f.setBold(false);
    
    painter->setFont(f);
    painter->setPen(_nodeStyle.FontColor);
}

void AgxNodePainter::drawEntryLabels(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    QJsonDocument json = QJsonDocument::fromVariant(_model->nodeData(_nodeId, AgxNodeRole::Style));
    AgxNodeStyle _nodeStyle(json.object());

    for (AgxPortType portType : {AgxPortType::Out, AgxPortType::In}) {
        unsigned int n = _model->nodeData(_nodeId, (portType == AgxPortType::Out) ? AgxNodeRole::OutPortCount : AgxNodeRole::InPortCount).value<unsigned int>();

        for (AgxPortIndex portIndex = 0; portIndex < n; ++portIndex) {
            auto const& connected = _model->connections(_nodeId, portType, portIndex);

            QPointF p = _geometry->portTextPosition(_nodeId, portType, portIndex);

            if (connected.empty())
                painter->setPen(_nodeStyle.FontColorFaded);
            else
                painter->setPen(_nodeStyle.FontColor);

            QString s;

            if (_model->portData(_nodeId, portType, portIndex, AgxPortRole::CaptionVisible).value<bool>()) {
                s = _model->portData(_nodeId, portType, portIndex, AgxPortRole::Caption).value<QString>();
            }
            else {
                auto portData = _model->portData(_nodeId, portType, portIndex, AgxPortRole::DataType);

                s = portData.value<AgxNodeDataType>().name;
            }

            painter->drawText(p, s);
        }
    }
}

void AgxNodePainter::drawResizeRect(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    if (_model->nodeFlags(_nodeId) & AgxNodeFlag::Resizable) {
        painter->setBrush(Qt::gray);

        painter->drawEllipse(_geometry->resizeHandleRect(_nodeId));
    }
}

void AgxNodePainter::drawValidationIcon(QPainter* painter, AgxNodeGraphicsObject& ngo) const
{
    if (_vState._state == AgxNodeValidationState::State::Valid)
        return;

    QSize size = _geometry->size(_nodeId);

    QIcon icon(":/CALUMIMotion/Resources/info-tooltip.svg");
    QSize iconSize(8, 8);
    QPixmap pixmap = icon.pixmap(iconSize);

    QColor color = (_vState._state == AgxNodeValidationState::State::Error) ? _nodeStyle.ErrorColor
        : _nodeStyle.WarningColor;

    QPointF center(size.width(), 0.0);
    center += QPointF(iconSize.width() / 2.0, -iconSize.height() / 2.0);

    painter->save();

    // Draw a colored circle behind the icon to highlight validation issues
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawEllipse(center, iconSize.width() / 2.0 + 2.0, iconSize.height() / 2.0 + 2.0);


    QPainter imgPainter(&pixmap);
    imgPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    imgPainter.fillRect(pixmap.rect(), _nodeStyle.FontColor);
    imgPainter.end();


    painter->drawPixmap(center.toPoint() - QPoint(iconSize.width() / 2, iconSize.height() / 2),
        pixmap);

    painter->restore();
}

void AgxNodePainter::drawWrappedTextInWidgetBounds(QPainter* painter, AgxNodeGraphicsObject& ngo, const QString& string) const
{
    if (!ngo._proxyWidget) return;
    QString temp = "This is a very long string that will be too long for the comment widget and should overflow easily and certainly get past the barrier or whatever it's called. The bounds? Idk man";
    auto bounds = ngo._proxyWidget->boundingRect();
    bounds.moveTo(_geometry->widgetPosition(_nodeId));
    painter->drawText(bounds, Qt::TextWordWrap | Qt::AlignLeft, temp);
}
