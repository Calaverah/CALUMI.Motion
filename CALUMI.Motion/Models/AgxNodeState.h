#pragma once

#include <unordered_map>
#include <vector>
#pragma warning(push,0)
#include <QPointF>
#include <QPointer>
#include <QUuid>

#pragma warning(pop)

class AgxNodeGraphicsObject;
class AgxConnectionGraphicsObject;

/// Stores bool for hovering connections and resizing flag.
class AgxNodeState
{
public:
    AgxNodeState(AgxNodeGraphicsObject& ngo);

public:
    bool hovered() const { return _hovered; }

    void setHovered(bool hovered = true) { _hovered = hovered; }

    void setResizing(bool resizing);

    bool resizing() const;

    AgxConnectionGraphicsObject const* connectionForReaction() const;

    void storeConnectionForReaction(AgxConnectionGraphicsObject const* cgo);

    void resetConnectionForReaction();

private:
    AgxNodeGraphicsObject& _ngo;

    bool _hovered;

    bool _resizing;

    // QPointer tracks the QObject inside and is automatically cleared
    // when the object is destroyed.
    QPointer<AgxConnectionGraphicsObject const> _connectionForReaction;
};