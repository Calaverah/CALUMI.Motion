#pragma once
#pragma warning(push,0)
#include "Utilities/AgxDefinitions.h"
#include <QJsonObject>
#pragma warning(pop)


class AgxGraphicsScene;
class AgxGraphModel;

class IAgxEmbedSceneData 
{
public:
	IAgxEmbedSceneData() : _graph(nullptr), _scene(nullptr), _nodeId(InvalidNodeId) {}
	virtual ~IAgxEmbedSceneData() = default;


public:
	//For Node Properties
	virtual inline void SetRefData(AgxNodeId nodeId, AgxGraphicsScene* scene) { 
		_graph = nullptr;
		_scene = scene; 
		_nodeId = nodeId;
		}

	//For Extending To Child Objects
	virtual inline void SetRefData(const IAgxEmbedSceneData& source) {
		_graph = source._graph;
		_scene = source._scene;
		_nodeId = source._nodeId;
	}

	//For Graph Properties
	virtual inline void SetRefData(AgxGraphModel* model, AgxGraphicsScene* scene) {
		_nodeId = InvalidNodeId;
		_scene = scene;
		_graph = model;
	}
protected:
	AgxNodeId _nodeId;
	AgxGraphicsScene* _scene;
	AgxGraphModel* _graph;

public:
	virtual void SendInsertPropertySheetDataCommand(const QJsonObject& obj);
	virtual void SendAddRowToPropertyBlockDataCommand(const QString& blockKey, int index);
	virtual void SendRemoveRowFromPropertyBlockDataCommand(const QString& blockKey, int index);

	virtual void RefreshNode() const;

	virtual QPoint mapToAgxView(QPoint pos);



};