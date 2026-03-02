#include "stdafx.h"
#include "AgxConnectionState.h"
#include "AgxGraphicsScene.h"

AgxPortType AgxConnectionState::requiredPort() const
{
    AgxPortType t = AgxPortType::None;

    if (_cgo.connectionId().outNodeId == InvalidNodeId) {
        t = AgxPortType::Out;
    }
    else if (_cgo.connectionId().inNodeId == InvalidNodeId) {
        t = AgxPortType::In;
    }

    return t;
}

bool AgxConnectionState::requiresPort() const
{
    const AgxConnectionId& id = _cgo.connectionId();
    return id.outNodeId == InvalidNodeId || id.inNodeId == InvalidNodeId;
}

bool AgxConnectionState::hovered() const
{
    return _hovered;
}

void AgxConnectionState::setHovered(bool hovered)
{
    _hovered = hovered;
}

void AgxConnectionState::setLastHoveredNode(AgxNodeId const nodeId)
{
    _lastHoveredNode = nodeId;
}

AgxNodeId AgxConnectionState::lastHoveredNode() const
{
    return _lastHoveredNode;
}

void AgxConnectionState::resetLastHoveredNode()
{
    if (_lastHoveredNode != InvalidNodeId) {
        auto ngo = _cgo.agxNodeScene()->agxNodeGraphicsObject(_lastHoveredNode);
        ngo->update();
    }

    _lastHoveredNode = InvalidNodeId;
}
