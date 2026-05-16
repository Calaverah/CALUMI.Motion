//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)
#include <QIcon>
#pragma warning(pop)

#include <AgxNodes/AgxNode.h>
#include <Utilities/AgxDefinitions.h>
#include <cstdint>

#include "AgxPalette.h"

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

    void drawWrappedTextInWidgetBounds(QPainter* painter, const AgxNodeGraphicsObject& ngo, const QString& string = QString()) const;

private:
    QIcon m_toolTipIcon{ ":/CALUMIMotion/Resources/info-tooltip.svg" };

    const AgxNodePalette* m_nodePalette = nullptr;

    AgxNodeGeometry* m_geometry = nullptr;
    AgxNodeValidationState m_vState;
    AgxNodeId m_nodeId = InvalidNodeId;
    AgxGraphModel* m_model = nullptr;
    bool m_collapsed = false;
    uint8_t m_lod = 0;
};
