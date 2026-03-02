#pragma once
#pragma warning(push,0)
#include <QUuid>
#include "Utilities/AgxDefinitions.h"
//#include "QtNodes/internal/Export.hpp
#pragma warning(pop)



class AgxConnectionGraphicsObject;

class AgxConnectionState
{
public:
    /// Defines whether we construct a new connection
    /// or it is already binding two nodes.
    enum LooseEnd { Pending = 0, Connected = 1 };

public:
    AgxConnectionState(AgxConnectionGraphicsObject& cgo)
        : _cgo(cgo)
        , _hovered(false)
    {
    }

    AgxConnectionState(AgxConnectionState const&) = delete;
    AgxConnectionState(AgxConnectionState&&) = delete;

    AgxConnectionState& operator=(AgxConnectionState const&) = delete;
    AgxConnectionState& operator=(AgxConnectionState&&) = delete;

    ~AgxConnectionState() = default;

public:
    AgxPortType requiredPort() const;
    bool requiresPort() const;

    bool hovered() const;
    void setHovered(bool hovered);

public:
    /// Caches NodeId for further interaction.
    void setLastHoveredNode(AgxNodeId const nodeId);

    AgxNodeId lastHoveredNode() const;

    void resetLastHoveredNode();

private:
    AgxConnectionGraphicsObject& _cgo;

    bool _hovered;

    AgxNodeId _lastHoveredNode{ InvalidNodeId };
};