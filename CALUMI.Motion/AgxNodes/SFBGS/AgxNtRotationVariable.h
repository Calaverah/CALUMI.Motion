#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtRotationVariable : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtRotationVariable(AgxGraphModel* rootGraphRef);
		~AgxNtRotationVariable() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		QStringList _RotationList = {
			"Shortest Direction",
			"Longest Direction",
			"Zero to One",
			"Looping Left",
			"Looping Right"
		};
	};

}