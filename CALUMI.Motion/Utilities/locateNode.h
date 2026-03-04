#pragma once
#pragma warning(push,0)
#include <QtCore/QPointF>
#include <QtGui/QTransform>
#include "Models/AgxNodeGraphicsObject.h"
#pragma warning(pop)

class QGraphicsScene;

AgxNodeGraphicsObject* locateNodeAt(QPointF scenePoint, QGraphicsScene& scene, const QTransform& viewTransform);

AgxNodeGraphicsObject* locateAgxNodeAt(QPointF scenePoint, AgxGraphicsScene& scene, const QTransform& viewTransform);
