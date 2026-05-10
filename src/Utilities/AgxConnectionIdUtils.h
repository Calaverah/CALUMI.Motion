#pragma once

#include "AgxDefinitions.h"

#include <QJsonObject>



inline AgxPortIndex getNodeId(AgxPortType portType, AgxConnectionId connectionId)
{
    AgxNodeId id = InvalidNodeId;

    switch (portType)
    {
        case AgxPortType::In:
            id = connectionId.inNodeId;
            break;
        case AgxPortType::Out:
            id = connectionId.outNodeId;
            break;
    }

    return id;
}

inline AgxPortIndex getPortIndex(AgxPortType portType, AgxConnectionId connectionId)
{
    AgxPortIndex index = InvalidPortIndex;

    switch (portType)
    {
        case AgxPortType::In:
            index = connectionId.inPortIndex;
            break;
        case AgxPortType::Out:
            index = connectionId.outPortIndex;
            break;
    }

    return index;
}

inline AgxPortType oppositePort(AgxPortType port)
{
    AgxPortType result = AgxPortType::None;

    switch (port) {
        case AgxPortType::In:
            result = AgxPortType::Out;
            break;

        case AgxPortType::Out:
            result = AgxPortType::In;
            break;
    }
    return result;
}

inline bool isPortIndexValid(AgxPortIndex index)
{
    return index != InvalidPortIndex;
}

inline bool isPortTypeValid(AgxPortType portType)
{
    return portType != AgxPortType::None;
}

/**
    * Creates a connection Id instance filled just on one side.
    */
inline AgxConnectionId makeIncompleteConnectionId(AgxNodeId const connectedNodeId,
                                                AgxPortType const connectedPort,
                                                AgxPortIndex const connectedPortIndex)
{
    return (connectedPort == AgxPortType::In)
        ? AgxConnectionId{ InvalidNodeId, InvalidPortIndex, connectedNodeId, connectedPortIndex }
    : AgxConnectionId{ connectedNodeId, connectedPortIndex, InvalidNodeId, InvalidPortIndex };
}

/**
    * Turns a full connection Id into an incomplete one by removing the
    * data on the given side
    */
inline AgxConnectionId makeIncompleteConnectionId(AgxConnectionId connectionId,
                                                AgxPortType const portToDisconnect)
{
    if (portToDisconnect == AgxPortType::Out) {
        connectionId.outNodeId = InvalidNodeId;
        connectionId.outPortIndex = InvalidPortIndex;
    } else {
        connectionId.inNodeId = InvalidNodeId;
        connectionId.inPortIndex = InvalidPortIndex;
    }

    return connectionId;
}

inline AgxConnectionId makeCompleteConnectionId(AgxConnectionId incompleteConnectionId,
                                                AgxNodeId const nodeId,
                                                AgxPortIndex const portIndex)
{
    if (incompleteConnectionId.outNodeId == InvalidNodeId) {
        incompleteConnectionId.outNodeId = nodeId;
        incompleteConnectionId.outPortIndex = portIndex;
    } else {
        incompleteConnectionId.inNodeId = nodeId;
        incompleteConnectionId.inPortIndex = portIndex;
    }

    return incompleteConnectionId;
}

inline QDebug& operator<<(QDebug& ostr, AgxConnectionId const connectionId)
{
    ostr << "(" << connectionId.outNodeId << ", "
        << (isPortIndexValid(connectionId.outPortIndex) ? QString("%1").arg(connectionId.outPortIndex)
            : "INVALID")
        << ", " << connectionId.inNodeId << ", "
        << (isPortIndexValid(connectionId.inPortIndex) ? QString("%1").arg(connectionId.inPortIndex)
            : "INVALID")
        << ")";// << std::endl;

    return ostr;
}

inline QJsonObject toJson(AgxConnectionId const& connId)
{
    QJsonObject connJson;

    connJson["outNodeId"] = static_cast<qint64>(connId.outNodeId);
    connJson["outPortIndex"] = static_cast<qint64>(connId.outPortIndex);
    connJson["intNodeId"] = static_cast<qint64>(connId.inNodeId);
    connJson["inPortIndex"] = static_cast<qint64>(connId.inPortIndex);

    return connJson;
}

inline AgxConnectionId fromJson(QJsonObject const& connJson)
{
    AgxConnectionId connId{ static_cast<AgxNodeId>(connJson["outNodeId"].toInt(InvalidNodeId)),
                        static_cast<AgxPortIndex>(connJson["outPortIndex"].toInt(InvalidPortIndex)),
                        static_cast<AgxNodeId>(connJson["intNodeId"].toInt(InvalidNodeId)),
                        static_cast<AgxPortIndex>(connJson["inPortIndex"].toInt(InvalidPortIndex)) };

    return connId;
}

