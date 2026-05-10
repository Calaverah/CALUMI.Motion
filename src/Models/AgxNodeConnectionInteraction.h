//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#pragma warning(push,0)

////
#include <QPointF>
#include <Utilities/AgxDefinitions.h>

#pragma warning(pop)

class AgxConnectionGraphicsObject;
class AgxNodeGraphicsObject;
class AgxGraphicsScene;

//Copied over from QtNodes and modified
/**
 * @brief Class wraps conecting and disconnecting checks.
 *
 * An instance should be created on the stack and destroyed
 * automatically when the operation is completed
 */
class AgxNodeConnectionInteraction
{
public:
    AgxNodeConnectionInteraction(AgxNodeGraphicsObject& ngo,
        AgxConnectionGraphicsObject& cgo,
        AgxGraphicsScene& scene);

    /**
     * @brief We check connection possibility from the perspecpive of
     * ConnectionGraphicsObject first and just then ask the GraphModel
     *
     * Can connect when following conditions are met:
     * 1. ConnectionGrachicsObject::connectionState() 'requires' a port.
     * 2. Connection loose end is geometrically above the node port.
     * 3. GraphModel permits connection
     *    - Here we check specific data type
     *    - multi-connection policy
     *    -  New connection does not introduce a loop if
     *       `AbstractGrphModel::loopsEnabled()` forbits it.
     */
    bool canConnect(AgxPortIndex* portIndex) const;

    /// Creates a new connectino if possible.
    /**
     * 1. Check conditions from 'canConnect'.
     * 2. Creates new connection with `GraphModel::addConnection`.
     * 3. Adjust connection geometry.
     */
    bool tryConnect() const;

    /**
     * 1. Delete connection with `GraphModel::deleteConnection`.
     * 2. Create a "draft" connection with incomplete `ConnectionId`.
     * 3. Repaint both previously connected nodes.
     */
    bool disconnect(AgxPortType portToDisconnect) const;
private:
    AgxPortType connectionRequiredPort() const;

    //QPointF connectionEndScenePosition(AgxPortType) const; //no definition???

    QPointF nodePortScenePosition(AgxPortType portType, AgxPortIndex portIndex) const;

    AgxPortIndex nodePortIndexUnderScenePoint(AgxPortType portType, QPointF const& p) const;

private:
    AgxNodeGraphicsObject& _ngo;

    AgxConnectionGraphicsObject& _cgo;

    AgxGraphicsScene& _scene;
};
