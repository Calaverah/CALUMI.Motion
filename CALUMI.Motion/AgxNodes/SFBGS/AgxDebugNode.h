#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"


namespace SFBGS {

	class AgxNtDebugNode : public SFBGSNode
	{
		Q_OBJECT


	public:
		AgxNtDebugNode(AgxGraphModel* rootGraphRef);
		~AgxNtDebugNode() = default;

	public:
		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		bool resizable() const override { return false; }

		void SetUpNode(const AgxGameType& type) override;


		inline bool CanSetNameProperty() const override { return true; }

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:

		QStringList _customDropdownList1 = { "Smooth","Linear" };
		QStringList _customDropdownList2 = { "Small","Medium","Large" };
	};
}