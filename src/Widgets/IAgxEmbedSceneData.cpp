//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "IAgxEmbedSceneData.h"
#include "Utilities/UndoRedoCommands.h"
#include "AgxGraphicsView.h"


void IAgxEmbedSceneData::SendInsertPropertySheetDataCommand(const QJsonObject& obj) {
	if(!m_scene)
		return;

	if(m_nodeId != InvalidNodeId)
		m_scene->undoStack().push(new InsertPropertySheetDataCommand(m_scene, m_nodeId, obj));

	if(m_graph)
		m_scene->undoStack().push(new InsertPropertySheetDataCommand(m_scene, m_graph, obj));
}

void IAgxEmbedSceneData::SendAddRowToPropertyBlockDataCommand(const QString& blockKey, int index)
{
	if (!m_scene)
		return;

	if (m_nodeId != InvalidNodeId)
		m_scene->undoStack().push(new AddRowToPropertyBlockDataCommand(m_scene, m_nodeId, blockKey, index));

	if(m_graph)
		m_scene->undoStack().push(new AddRowToPropertyBlockDataCommand(m_scene, m_graph, blockKey, index));
}

void IAgxEmbedSceneData::SendRemoveRowFromPropertyBlockDataCommand(const QString & blockKey, int index)
{
	if (!m_scene)
		return;

	if (m_nodeId != InvalidNodeId)
		m_scene->undoStack().push(new RemoveRowFromPropertyBlockDataCommand(m_scene, m_nodeId, blockKey, index));

	if (m_graph)
		m_scene->undoStack().push(new RemoveRowFromPropertyBlockDataCommand(m_scene, m_graph, blockKey, index));
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
	if (auto agxview = dynamic_cast<AgxGraphicsView*>(m_scene->views().at(0))) 
	{
		if(m_nodeId != InvalidNodeId)
		{
			QPoint newPos = m_scene->agxNodeGraphicsObject(m_nodeId)->scenePos().toPoint() + m_scene->agxNodeGeometry().widgetPosition(m_nodeId).toPoint() + pos; //get the pos of the node, pos of the embedded widget, then the pos within the embedded widget given by the argument

			return agxview->mapToGlobal(agxview->mapFromScene(newPos));
		}
	} 
	return pos;
}

