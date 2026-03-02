#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtPathingAnimations : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtPathingAnimations(AgxGraphModel* rootGraphRef);
		~AgxNtPathingAnimations() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;


	private:
		QStringList _dropdown = { 
			"Walk",		//0
			//unk		//1 - jog?
			"Run" };	//2

		//non instanced only once on graph type 0. Never seen on graph type 1
	};

}