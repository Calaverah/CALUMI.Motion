//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxNodeState.h"
#include "AgxConnectionGraphicsObject.h"
#include "AgxNodeGraphicsObject.h"

AgxNodeState::AgxNodeState(AgxNodeGraphicsObject& ngo) : _ngo(ngo), _hovered(false), _resizing(false), _connectionForReaction{nullptr}
{
    Q_UNUSED(_ngo);
}

void AgxNodeState::setResizing(bool resizing)
{
    _resizing = resizing;
}

bool AgxNodeState::resizing() const
{
    return _resizing;
}

AgxConnectionGraphicsObject const* AgxNodeState::connectionForReaction() const
{
    return _connectionForReaction.data();
}

void AgxNodeState::storeConnectionForReaction(AgxConnectionGraphicsObject const* cgo)
{
    _connectionForReaction = cgo;
}

void AgxNodeState::resetConnectionForReaction()
{
    _connectionForReaction.clear();
}