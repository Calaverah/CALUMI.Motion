#pragma once

#pragma warning(push,0)
////#include "QtNodes/internal/Definitions.hpp
//#include "QtNodes/internal/Export.hpp

#include <QtCore/QJsonObject>
#include <QtCore/QPointF>
#include <QUndoCommand>
#pragma warning(pop)
#include <unordered_set>
#include "Models/AgxGraphicsScene.h"

#pragma region COPIED_FROM_QTNODES


class CreateCommand : public QUndoCommand
{
public:
    CreateCommand(AgxGraphicsScene* scene, QString const name, QPointF const& mouseScenePos);
    
    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _nodeId;
    QJsonObject _sceneJson;
};

/**
 * Selected scene objects are serialized and then removed from the scene.
 * The deleted elements could be restored in `undo`.
 */
class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(AgxGraphicsScene* scene, const QString& overrideString = "Delete");

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QJsonObject _sceneJson;
    QList<AgxNodeId> _nodes;
    QList<AgxConnectionId> _conns;
};

class CopyCommand : public QUndoCommand
{
public:
    CopyCommand(AgxGraphicsScene* scene);
};

class PasteCommand : public QUndoCommand
{
public:
    PasteCommand(AgxGraphicsScene* scene, QPointF const& mouseScenePos);

    void undo() override;
    void redo() override;

private:
    QJsonObject takeSceneJsonFromClipboard();
    QJsonObject makeNewNodeIdsInScene(QJsonObject const& sceneJson);

private:
    AgxGraphicsScene* _scene;
    QPointF const& _mouseScenePos;
    QJsonObject _newSceneJson;
    std::set<QString> _groupsAdded;
};

class DisconnectCommand : public QUndoCommand
{
public:
    DisconnectCommand(AgxGraphicsScene* scene, AgxConnectionId const);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;

    AgxConnectionId _connId;
};

class ConnectCommand : public QUndoCommand
{
public:
    ConnectCommand(AgxGraphicsScene* scene, AgxConnectionId const);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;

    AgxConnectionId _connId;
};

class MoveNodeCommand : public QUndoCommand
{
public:
    MoveNodeCommand(AgxGraphicsScene* scene, QPointF const& diff);

    void undo() override;
    void redo() override;

    /**
   * A command ID is used in command compression. It must be an integer unique to
   * this command's class, or -1 if the command doesn't support compression.
   */
    int id() const override;

    /**
   * Several sequential movements could be merged into one command.
   */
    bool mergeWith(QUndoCommand const* c) override;

private:
    AgxGraphicsScene* _scene;
    std::unordered_set<AgxNodeId> _selectedNodes;
    QPointF _diff;
};

#pragma endregion

#pragma region CALUMI_MOTION_COMMANDS

class CreateGroupCommand : public QUndoCommand
{
public:
    CreateGroupCommand(AgxGraphicsScene* scene, const QString& groupId);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QString _groupId;
    QColor _color;
};

class EraseGroupCommand : public QUndoCommand
{
public:
    EraseGroupCommand(AgxGraphicsScene* scene, const QString& groupId);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QString _groupId;
    QColor _color;
    QVector<AgxNodeId> _assignedNodes;
};

class AddNodeToGroupCommand : public QUndoCommand
{
public:
    AddNodeToGroupCommand(AgxGraphicsScene* scene, const QString& groupId, AgxNodeId nodeId);

    void undo() override;
    void redo() override;
private:
    AgxGraphicsScene* _scene;
    AgxNodeId _node;
    QString _nextGroupId;
    QString _prevGroupId;
};

class AddSelectedToGroupCommand : public QUndoCommand
{
public:
    AddSelectedToGroupCommand(AgxGraphicsScene* scene, const QString& groupId, const std::unordered_map<AgxNodeId, QString>& prevAssignments);

    void undo() override;
    void redo() override;
private:
    AgxGraphicsScene* _scene;
    std::unordered_map<AgxNodeId, QString> _previousAssignments;
    QString _nextGroupId;
};

class RemoveNodeFromGroupCommand : public QUndoCommand
{
public:
    RemoveNodeFromGroupCommand(AgxGraphicsScene* scene, AgxNodeId node);

    void undo() override;
    void redo() override;
private:
    AgxGraphicsScene* _scene;
    AgxNodeId _node;
    QString _prevGroupId;
};

class AddPortCommand : public QUndoCommand
{
public:
    AddPortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type, AgxPortIndex index);
    AddPortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _node;
    AgxPortIndex _port;
    AgxPortType _type;
    QJsonObject _portData;
};

class RemovePortCommand : public QUndoCommand
{
public:
    RemovePortCommand(AgxGraphicsScene* scene, AgxNodeId node, AgxPortType type, AgxPortIndex port);

    void undo() override;
    void redo() override;
private:
    AgxGraphicsScene* _scene;
    AgxNodeId _node;
    AgxPortIndex _port;
    AgxPortType _type;
    std::unordered_set<AgxConnectionId> _connections;
    QJsonObject _data;
};

class RenameNodeCommand : public QUndoCommand
{
public:
    RenameNodeCommand(AgxGraphicsScene* scene, AgxNodeId node, const QString& newName);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _node;
    QString _prevName;
    QString _newName;
};

class ChangeGroupColorCommand : public QUndoCommand
{
public:
    ChangeGroupColorCommand(AgxGraphicsScene* scene, const QString& groupId, QColor newColor);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QString _groupId;
    QColor _oldColor;
    QColor _newColor;
};

class SetNewGraphPropertiesCommand : public QUndoCommand
{
public:
    SetNewGraphPropertiesCommand(AgxGraphicsScene* scene, const AgxGraphType& graphType);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QJsonObject _oldIData;
    QJsonObject _newIData;
    AgxGraphType _newGraphType = AgxGraphType::UNDEFINED;
    AgxGraphType _oldGraphType = AgxGraphType::UNDEFINED;
};

class InsertPropertySheetDataCommand : public QUndoCommand
{
public:
    InsertPropertySheetDataCommand(AgxGraphicsScene* scene, const AgxNodeId& _node, const QJsonObject& newIData);
    InsertPropertySheetDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QJsonObject& newIData);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene = nullptr;
    AgxGraphModel* _model = nullptr;
    AgxNodeId _nodeId = InvalidNodeId;
    QJsonObject _oldIData;
    QJsonObject _newIData;
};

class AddRowToPropertyBlockDataCommand : public QUndoCommand
{
public:
    AddRowToPropertyBlockDataCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const QString& block, int index);
    AddRowToPropertyBlockDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QString& block, int index);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene = nullptr;
    AgxGraphModel* _model = nullptr;
    AgxNodeId _nodeId = InvalidNodeId;
    QString _block;
    int _index;

};

class RemoveRowFromPropertyBlockDataCommand : public QUndoCommand
{
public:
    RemoveRowFromPropertyBlockDataCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const QString& block, int index);
    RemoveRowFromPropertyBlockDataCommand(AgxGraphicsScene* scene, AgxGraphModel* model, const QString& block, int index);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene = nullptr;
    AgxGraphModel* _model = nullptr;
    AgxNodeId _nodeId = InvalidNodeId;
    QString _block;
    int _index;
    QList<AgxPropertyBlockData::Entry> _data;

};

class ToggleNodeHiddenStateCommand : public QUndoCommand
{
public:
    ToggleNodeHiddenStateCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _nodeId;
    QJsonObject _previousValue;
    QJsonObject _newValue;
};

class AgxPortCommandPayloadCommand : public QUndoCommand
{
public:
    AgxPortCommandPayloadCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, const AgxPortType type, const AgxPortIndex& index, const QJsonObject& commandData);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _nodeId;
    AgxPortType _type;
    AgxPortIndex _index;
    QJsonObject _data;
};

class AgxNodeAltStateCommand : public QUndoCommand
{
public:
    AgxNodeAltStateCommand(AgxGraphicsScene* scene, const AgxNodeId& nodeId, bool unset = false);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    AgxNodeId _nodeId;
    AgxNodeId _prevNode;
    bool _unset;
};

class AgxSetGraphCategory : public QUndoCommand
{
public:
    AgxSetGraphCategory(AgxGraphModel* model, const QString& newCategory);

    void undo() override;
    void redo() override;

private:
    AgxGraphModel* _model;
    QString _newCat;
    QString _oldCat;
};

class AgxHideCommand : public QUndoCommand
{
public:
    AgxHideCommand(AgxGraphicsScene* scene, bool hide = true);

    void undo() override;
    void redo() override;

private:
    AgxGraphicsScene* _scene;
    QSet<AgxNodeId> _selectedNodes;
    QList<AgxConnectionId> _selectedConnections;
    bool _toHide;
};

#pragma endregion

