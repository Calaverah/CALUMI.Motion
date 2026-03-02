#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"


namespace SFBGS {

	class AgxNtGamebryoSequence : public SFBGSNode
	{
		Q_OBJECT


	public:
		AgxNtGamebryoSequence(AgxGraphModel* rootGraphRef);
		~AgxNtGamebryoSequence() = default;

	public:
		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		QString SubCaption() const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		QStringList _BlendList = {
			"None",
			"Weight",
			"One Minus Weight"
		};
		
	};
}