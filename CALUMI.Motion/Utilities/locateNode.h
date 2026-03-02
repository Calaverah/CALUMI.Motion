#pragma once
#pragma warning(push,0)
#include <QtCore/QPointF>
#include <QtGui/QTransform>
#include "Models/AgxNodeGraphicsObject.h"
#pragma warning(pop)
class QGraphicsScene;
namespace QtNodes
{
    class NodeGraphicsObject;
}

AgxNodeGraphicsObject* locateNodeAt(QPointF scenePoint,
    QGraphicsScene& scene,
    QTransform const& viewTransform);

AgxNodeGraphicsObject* locateAgxNodeAt(QPointF scenePoint, AgxGraphicsScene& scene, QTransform const& viewTransform);
