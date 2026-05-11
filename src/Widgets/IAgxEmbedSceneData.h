//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include "Utilities/AgxDefinitions.h"
#pragma warning(pop)


class AgxGraphicsScene;
class AgxGraphModel;

class IAgxEmbedSceneData 
{
public:
	IAgxEmbedSceneData() : m_nodeId(InvalidNodeId), m_scene(nullptr), m_graph(nullptr) {}
	virtual ~IAgxEmbedSceneData() = default;


//For Node Properties
	virtual void SetRefData(const AgxNodeId& nodeId, AgxGraphicsScene* scene) {
		m_graph = nullptr;
		m_scene = scene;
		m_nodeId = nodeId;
		}

	//For Extending To Child Objects
	virtual void SetRefData(const IAgxEmbedSceneData& source) {
		m_graph = source.m_graph;
		m_scene = source.m_scene;
		m_nodeId = source.m_nodeId;
	}

	//For Graph Properties
	virtual void SetRefData(AgxGraphModel* model, AgxGraphicsScene* scene) {
		m_nodeId = InvalidNodeId;
		m_scene = scene;
		m_graph = model;
	}
protected:
	AgxNodeId m_nodeId;
	AgxGraphicsScene* m_scene;
	AgxGraphModel* m_graph;

public:
	virtual void SendInsertPropertySheetDataCommand(const QJsonObject& obj);
	virtual void SendAddRowToPropertyBlockDataCommand(const QString& blockKey, int index);
	virtual void SendRemoveRowFromPropertyBlockDataCommand(const QString& blockKey, int index);

	virtual void RefreshNode() const;

	virtual QPoint mapToAgxView(QPoint pos);
};