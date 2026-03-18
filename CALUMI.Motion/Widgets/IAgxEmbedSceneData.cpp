//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "IAgxEmbedSceneData.h"
#include "Utilities/UndoRedoCommands.h"
#include "AgxGraphicsView.h"


void IAgxEmbedSceneData::SendInsertPropertySheetDataCommand(const QJsonObject& obj) {
	if(!_scene)
		return;

	if(_nodeId != InvalidNodeId)
		_scene->undoStack().push(new InsertPropertySheetDataCommand(_scene, _nodeId, obj));

	if(_graph)
		_scene->undoStack().push(new InsertPropertySheetDataCommand(_scene, _graph, obj));
}

void IAgxEmbedSceneData::SendAddRowToPropertyBlockDataCommand(const QString& blockKey, int index)
{
	if (!_scene)
		return;

	if (_nodeId != InvalidNodeId)
		_scene->undoStack().push(new AddRowToPropertyBlockDataCommand(_scene, _nodeId, blockKey, index));

	if(_graph)
		_scene->undoStack().push(new AddRowToPropertyBlockDataCommand(_scene, _graph, blockKey, index));
}

void IAgxEmbedSceneData::SendRemoveRowFromPropertyBlockDataCommand(const QString & blockKey, int index)
{
	if (!_scene)
		return;

	if (_nodeId != InvalidNodeId)
		_scene->undoStack().push(new RemoveRowFromPropertyBlockDataCommand(_scene, _nodeId, blockKey, index));

	if (_graph)
		_scene->undoStack().push(new RemoveRowFromPropertyBlockDataCommand(_scene, _graph, blockKey, index));
}

void IAgxEmbedSceneData::RefreshNode() const
{
	//if (!_scene || _nodeId == InvalidNodeId)
		//return;

	//_scene->agxNodeGeometry().recomputeSize(_nodeId);
	//Q_EMIT _scene->agxGraphModel().nodeUpdated(_nodeId);
}

QPoint IAgxEmbedSceneData::mapToAgxView(QPoint pos)
{
	if (auto agxview = dynamic_cast<AgxGraphicsView*>(_scene->views().at(0))) 
	{
		if(_nodeId != InvalidNodeId)
		{
			QPoint newPos = _scene->agxNodeGraphicsObject(_nodeId)->scenePos().toPoint() + _scene->agxNodeGeometry().widgetPosition(_nodeId).toPoint() + pos; //get the pos of the node, pos of the embedded widget, then the pos within the embedded widget given by the argument

			return agxview->mapToGlobal(agxview->mapFromScene(newPos));
		}
	} 
	return pos;
}

