#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"


#include "Widgets/MiniGraphicsView.h"

namespace SFBGS {

	class AgxNtEmbeddedGraph : public SFBGSNode
	{
		Q_OBJECT


	public:
		AgxNtEmbeddedGraph(AgxGraphModel* rootGraphRef);
		~AgxNtEmbeddedGraph() = default;

	public:
		// Inherited via AgxNode
		QString name() const override;
		QString typeName() const override;
		QString caption() const override;
		QString SubCaption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		void ToggleCollapse() override;

		QWidget* embeddedWidget() override;
		AgxNodeType GetNodeType() const override;

		void SetUpNode(const AgxGameType& type) override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;
	private:
		QString SubCaptionUnformatted() const;

	private:
		MiniGraphicsView* _embGraphWidget = nullptr;
		QString _blendName = "Blend Tree Embedded";
		QString _stateName = "State Machine Embedded";

		int _blendPropIdx = -1;
		int _statePropIdx = -1;
	};
}