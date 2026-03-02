#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtAnimationNode : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtAnimationNode(AgxGraphModel* rootGraphRef);
		~AgxNtAnimationNode() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		QString SubCaption() const override;
		unsigned int nPorts(AgxPortType portType) const override;
		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

		//non instanced is true 15 times total but only on graph type 1 (state machine?)
	};

}