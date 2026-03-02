#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtGraphRef: public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtGraphRef(AgxGraphModel* rootGraphRef);
		~AgxNtGraphRef() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		QString SubCaption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

		//non instanced is true 3 times total but only on graph type 1 (state machine?)
		//uses color on graphtype1 14 times and on gtype2 2 times

	};

}