#include "GraphModel.h"

namespace CALUMI::Motion
{
    bool GraphModel::deleteNode(const QUuid& nodeId)
    {
        auto node = m_nodeMap.take(nodeId);

        if (!node)
            return false;

        
    }

    INode* GraphModel::node(const QUuid& nodeId)
    {
        return hasNode(nodeId) ? m_nodeMap.value(nodeId) : Q_NULLPTR;
    }

    const INode* GraphModel::node(const QUuid& nodeId) const
    {
        return hasNode(nodeId) ? m_nodeMap.value(nodeId) : Q_NULLPTR;
    }

    bool GraphModel::hasNode(const QUuid& nodeId) const
    {
        return m_nodeMap.contains(nodeId);
    }
}
