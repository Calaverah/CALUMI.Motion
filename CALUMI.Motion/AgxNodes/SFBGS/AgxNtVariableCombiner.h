#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"


namespace SFBGS {

	class AgxNtVariableCombiner : public SFBGSNode
	{
		Q_OBJECT


	public:
		AgxNtVariableCombiner(AgxGraphModel* rootGraphRef);
		~AgxNtVariableCombiner() = default;

	public:
		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;
		
		AgxNodeType GetNodeType() const override;
	};
}