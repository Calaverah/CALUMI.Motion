//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QtGui/QPainterPath>
#include "AgxPalette.h"
#pragma warning(pop)


class AgxConnectionGraphicsObject;

class AgxConnectionPainter
{
public:
    void paint(QPainter* painter, const AgxConnectionGraphicsObject& cgo);

    static QPainterPath GetPainterStroke(const AgxConnectionGraphicsObject& connection);
private:
    static QPainterPath CubicPath(const AgxConnectionGraphicsObject& connection);

    void drawSketchLine(QPainter* painter, const AgxConnectionGraphicsObject& cgo) const;
    void drawHoveredOrSelected(QPainter* painter, const AgxConnectionGraphicsObject& cgo) const;
    void drawNormalLine(QPainter* painter, const AgxConnectionGraphicsObject& cgo) const;

    const AgxConnectionPalette* m_connPalette = nullptr;
};

