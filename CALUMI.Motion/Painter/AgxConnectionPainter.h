#pragma once
#pragma warning(push,0)
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>

#include "AgxConnectionStyle.h"
#pragma warning(pop)


class AgxConnectionGraphicsObject;

class AgxConnectionPainter
{
public:
    void paint(QPainter* painter, AgxConnectionGraphicsObject const& cgo);
    QPainterPath getPainterStroke(AgxConnectionGraphicsObject const& cgo) const;
private:
    QPainterPath cubicPath(AgxConnectionGraphicsObject const& connection) const;
    void drawSketchLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const;
    void drawHoveredOrSelected(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const;
    void drawNormalLine(QPainter* painter, AgxConnectionGraphicsObject const& cgo) const;

    AgxConnectionStyle _connectionStyle;
};

