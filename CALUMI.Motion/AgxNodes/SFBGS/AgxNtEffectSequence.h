#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"

namespace SFBGS {
	class AgxNtEffectSequence : public SFBGSNode
	{
		Q_OBJECT

	public:
		AgxNtEffectSequence(AgxGraphModel* rootGraphRef);
		~AgxNtEffectSequence() = default;

		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		QString SubCaption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		AgxNodeValidationState validationState() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		QList<TermRef> _ModeList = {
		&AgxDictionary::None,
		&AgxDictionary::Weight,
		&AgxDictionary::OneMinusWeight
		};
	};

}