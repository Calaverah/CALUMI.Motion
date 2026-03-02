#include "stdafx.h"
#include "AgxNodeConnectionInteraction.h"
#pragma warning(push,0)
//#include "QtNodes/internal/ConnectionGraphicsObject.hpp
//#include "QtNodes/internal/NodeGraphicsObject.hpp
//#include "QtNodes/internal/BasicGraphicsScene.hpp
#include "Utilities/AgxConnectionIdUtils.h"
#pragma warning(pop)
#include "Painter/AgxNodeGeometry.h"
#include "Models/AgxGraphicsScene.h"
#include "Models/AgxConnectionGraphicsObject.h"
#include "Models/AgxNodeGraphicsObject.h"
#include "Utilities/UndoRedoCommands.h"

AgxNodeConnectionInteraction::AgxNodeConnectionInteraction(AgxNodeGraphicsObject& ngo, AgxConnectionGraphicsObject& cgo, AgxGraphicsScene& scene) : _ngo(ngo), _cgo(cgo) ,_scene(scene) { }

bool AgxNodeConnectionInteraction::canConnect(AgxPortIndex* portIndex) const
{
    // 1. Connection requires a port.
    AgxPortType const requiredPort = _cgo.connectionState().requiredPort();
    if (requiredPort == AgxPortType::None) {
        return false;
    }

    // 2. Connection loose end is above the node port.
    QPointF const connectionPoint = _cgo.sceneTransform().map(_cgo.endPoint(requiredPort));
    *portIndex = nodePortIndexUnderScenePoint(requiredPort, connectionPoint);
    if (*portIndex == InvalidPortIndex) {
        return false;
    }

    // 3. Model permits connection.
    AgxGraphModel& model = _ngo.agxNodeScene()->agxGraphModel();
    AgxConnectionId connectionId = makeCompleteConnectionId(_cgo.connectionId(), // incomplete
        _ngo.nodeId(),       // missing node id
        *portIndex);         // missing port index

    return model.connectionPossible(connectionId);
}

bool AgxNodeConnectionInteraction::tryConnect() const
{
    // 1. Check conditions from 'canConnect'.

    AgxPortIndex targetPortIndex = InvalidPortIndex;
    if (!canConnect(&targetPortIndex)) {
        return false;
    }

    // 2. Create new connection.

    AgxConnectionId incompleteConnectionId = _cgo.connectionId();

    AgxConnectionId newConnectionId = makeCompleteConnectionId(incompleteConnectionId,
        _ngo.nodeId(),
        targetPortIndex);

    _ngo.agxNodeScene()->resetDraftConnection();

    _ngo.agxNodeScene()->undoStack().push(new ConnectCommand(_ngo.agxNodeScene(), newConnectionId));

    return true;
}

bool AgxNodeConnectionInteraction::disconnect(AgxPortType portToDisconnect) const
{
    AgxConnectionId connectionId = _cgo.connectionId();

    _scene.undoStack().push(new DisconnectCommand(&_scene, connectionId));

    AgxNodeGeometry& geometry = _scene.agxNodeGeometry();

    QPointF scenePos = geometry.portScenePosition(_ngo.nodeId(),
        portToDisconnect,
        getPortIndex(portToDisconnect, connectionId),
        _ngo.sceneTransform());

    // Converted to "draft" connection with the new incomplete id.
    AgxConnectionId incompleteConnectionId = makeIncompleteConnectionId(connectionId, portToDisconnect);

    // Grabs the mouse
    auto const& draftConnection =
        _scene.makeDraftConnection(incompleteConnectionId);

    QPointF const looseEndPos = draftConnection->mapFromScene(scenePos);
    draftConnection->setEndPoint(portToDisconnect, looseEndPos);

    //Repaint connection points.
    AgxNodeId connectedNodeId = getNodeId(oppositePort(portToDisconnect), connectionId);
    _scene.agxNodeGraphicsObject(connectedNodeId)->update();

    AgxNodeId disconnectedNodeId = getNodeId(portToDisconnect, connectionId);
    _scene.agxNodeGraphicsObject(disconnectedNodeId)->update();

    return true;
}

AgxPortType AgxNodeConnectionInteraction::connectionRequiredPort() const
{
    auto const& state = _cgo.connectionState();

    return state.requiredPort();
}

QPointF AgxNodeConnectionInteraction::nodePortScenePosition(AgxPortType portType,
    AgxPortIndex portIndex) const
{
    AgxNodeGeometry& geometry = _scene.agxNodeGeometry();

    QPointF p = geometry.portScenePosition(_ngo.nodeId(),
        portType,
        portIndex,
        _ngo.sceneTransform());

    return p;
}

AgxPortIndex AgxNodeConnectionInteraction::nodePortIndexUnderScenePoint(AgxPortType portType,
    QPointF const& scenePoint) const
{
    AgxNodeGeometry& geometry = _scene.agxNodeGeometry();

    QTransform sceneTransform = _ngo.sceneTransform();

    QPointF nodePoint = sceneTransform.inverted().map(scenePoint);

    return geometry.checkPortHit(_ngo.nodeId(), portType, nodePoint);
}
