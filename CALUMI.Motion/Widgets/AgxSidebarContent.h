#pragma once

#pragma warning(push,0)
#include <QWidget>
#include <QLayout>
#pragma warning(pop)
#include "Widgets/AgxNodePropertiesWidget.h"

class AgxSidebarContent : public QWidget, public IAgxEmbedSceneData
{
	Q_OBJECT
public:
	AgxSidebarContent(QWidget* parent = nullptr) : QWidget(parent) {}

Q_SIGNALS:
	void StateChanged();
	void ReferenceInitialized(const IAgxEmbedSceneData& source);

public:
	inline void SetRefData(const IAgxEmbedSceneData& source) override {
		IAgxEmbedSceneData::SetRefData(source);
		Q_EMIT ReferenceInitialized(*this);
	}

	inline void SetRefData(AgxNodeId nodeId, AgxGraphicsScene* scene) override {
		IAgxEmbedSceneData::SetRefData(nodeId, scene);
		Q_EMIT ReferenceInitialized(*this);
	}

	inline void SetRefData(AgxGraphModel* model, AgxGraphicsScene* scene) override {
		IAgxEmbedSceneData::SetRefData(model, scene);
		Q_EMIT ReferenceInitialized(*this);
	}

};