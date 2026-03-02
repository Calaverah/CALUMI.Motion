#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtDynamicGraphReference : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtDynamicGraphReference(AgxGraphModel* rootGraphRef);
		~AgxNtDynamicGraphReference() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	};

}