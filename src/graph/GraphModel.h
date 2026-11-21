#pragma once
#include <QQmlEngine>
#include "IModelInstance.h"
#include "INode.h"
#include <QUuid>

namespace CALUMI::Motion
{
    class UniqueIdManager;

    class GraphModel : public IModelInstance
    {
        Q_OBJECT
        QML_ELEMENT
    public:
        QUuid createNode(NodeType type, const QUuid& nodeId = QUuid::createUuid());
        bool deleteNode(const QUuid& nodeId);

        [[nodiscard]] INode* node(const QUuid& nodeId);
        [[nodiscard]] const INode* node(const QUuid& nodeId) const;

        [[nodiscard]] bool hasNode(const QUuid& nodeId) const;


    private:
        UniqueIdManager* m_idManager = Q_NULLPTR;
        QHash<QUuid, QPointer<INode>> m_nodeMap;
    };
}
