#include "stdafx.h"
#include "locateNode.h"
#pragma warning(push,0)
#include "Models/AgxNodeGraphicsObject.h"

#include <QtCore/QList>
#include <QtWidgets/QGraphicsScene>
#pragma warning(pop)
#include "Models/AgxGraphicsScene.h"

AgxNodeGraphicsObject* locateNodeAt(QPointF scenePoint, QGraphicsScene& scene, QTransform const& viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items = scene.items(scenePoint,
        Qt::IntersectsItemShape,
        Qt::DescendingOrder,
        viewTransform);

    // items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem*> filteredItems;

    std::copy_if(items.begin(),
        items.end(),
        std::back_inserter(filteredItems),
        [](QGraphicsItem* item) {
            return (qgraphicsitem_cast<AgxNodeGraphicsObject*>(item) != nullptr);
        });

    AgxNodeGraphicsObject* node = nullptr;

    if (!filteredItems.empty()) {
        QGraphicsItem* graphicsItem = filteredItems.front();
        node = dynamic_cast<AgxNodeGraphicsObject*>(graphicsItem);
    }

    return node;
}

AgxNodeGraphicsObject* locateAgxNodeAt(QPointF scenePoint, AgxGraphicsScene& scene, QTransform const& viewTransform)
{
    // items under cursor
    QList<QGraphicsItem*> items = scene.items(scenePoint,
        Qt::IntersectsItemShape,
        Qt::DescendingOrder,
        viewTransform);

    // items convertable to NodeGraphicsObject
    std::vector<QGraphicsItem*> filteredItems;

    std::copy_if(items.begin(),
        items.end(),
        std::back_inserter(filteredItems),
        [](QGraphicsItem* item) {
            return (qgraphicsitem_cast<AgxNodeGraphicsObject*>(item) != nullptr);
        });

    AgxNodeGraphicsObject* node = nullptr;

    if (!filteredItems.empty()) {
        QGraphicsItem* graphicsItem = filteredItems.front();
        node = dynamic_cast<AgxNodeGraphicsObject*>(graphicsItem);
    }

    return node;
}
