//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"



namespace SFBGS {
	class AgxNtSwapGraph : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtSwapGraph(AgxGraphModel* rootGraphRef);
		~AgxNtSwapGraph() = default;

	public:
		//Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;
	};
}