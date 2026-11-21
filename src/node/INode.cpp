#include "INode.h"

namespace CALUMI::Motion
{
    QList<QUuid> INode::childNodeIds(const bool recursive) const
    {
        QList<QUuid> output;
        const auto children = this->children();
        output.reserve(children.size());
        for (const auto child : children)
        {
            if (const auto* childNode = qobject_cast<INode*>(child))
            {
                output.push_back(childNode->nodeId());
                if (recursive)
                {
                    const auto grandchildren = childNode->childNodeIds(true);
                    output.reserve(output.capacity() + grandchildren.size());
                    output.append(grandchildren);
                }
            }
        }
        output.shrink_to_fit();
        return output;
    }
}
