#pragma once

#pragma warning(push,0)
#include <QIcon>
#include <QtGui/QPainter>
#pragma warning(pop)

#include <AgxNodes/AgxNode.h>
#include <Utilities/AgxDefinitions.h>
#include <cstdint>


class AgxNodeGraphicsObject;
class AgxNodeGeometry;
class AgxGraphModel;

//borrowing DefaultNodePainter decl/def instead of inheriting
class AgxNodePainter{
public:
    void paint(QPainter* painter, AgxNodeGraphicsObject& ngo);

    void drawNodeRect(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawConnectionPoints(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawFilledConnectionPoints(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawNodeCaption(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawEntryLabels(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawResizeRect(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawValidationIcon(QPainter* painter, AgxNodeGraphicsObject& ngo) const;

    void drawWrappedTextInWidgetBounds(QPainter* painter, AgxNodeGraphicsObject& ngo, const QString& string = QString()) const;

private:
    QIcon _toolTipIcon{ ":/CALUMIMotion/Resources/info-tooltip.svg" };

    AgxNodeStyle _nodeStyle;
    AgxNodeGeometry* _geometry = nullptr;
    AgxNodeValidationState _vState;
    AgxNodeId _nodeId = InvalidNodeId;
    AgxGraphModel* _model = nullptr;
    bool _collapsed = false;
    uint8_t _lod = 0;
};
