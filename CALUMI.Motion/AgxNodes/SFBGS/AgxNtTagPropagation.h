#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtTagPropagation : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtTagPropagation(AgxGraphModel* rootGraphRef);
		~AgxNtTagPropagation() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		unsigned int nPorts(AgxPortType portType) const override;
		
		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		QList<TermRef> _PropList = { 
			&AgxDictionary::UNKNOWN,
			&AgxDictionary::PropagateTagUpstream,
			&AgxDictionary::ReceiveTagfromDownstream
		};
	};

}