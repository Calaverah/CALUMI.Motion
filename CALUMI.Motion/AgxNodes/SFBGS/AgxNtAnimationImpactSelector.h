#pragma once
#pragma warning(push,0)
#include <QObject>
#pragma warning(pop)
#include "..\AgxNode.h"
#include "Widgets/AgxNodePropertiesWidget.h"


namespace SFBGS {

	class AgxNtAnimationImpactSelector : public SFBGSNode
	{
		Q_OBJECT


	public:
		AgxNtAnimationImpactSelector(AgxGraphModel* rootGraphRef);
		~AgxNtAnimationImpactSelector() = default;

	public:
		// Inherited via AgxNode
		QString name() const override;
		QString caption() const override;
		QString SubCaption() const override;
		unsigned int nPorts(AgxPortType portType) const override;

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		QStringList _BodyPartList = {
			"TORSO",				//0
			"HEAD_1",				//1
			"EYE_1",				//2 //Unconfirmed
			"LOOKAT_1",				//3 //Unconfirmed
			"FLY_GRAB",				//4 //Unconfirmed
			"HEAD_2",				//5 //Unconfirmed
			"LEFT_ARM_1",			//6
			"LEFT_ARM_2",			//7 //Unconfirmed
			"RIGHT_ARM_1",			//8
			"RIGHT_ARM_2",			//9 //Unconfirmed
			"LEFT_LEG_1",			//10
			"LEFT_LEG_2",			//11
			"LEFT_LEG_3",			//12
			"RIGHT_LEG_1",			//13
			"RIGHT_LEG_2",			//14
			"RIGHT_LEG_3",			//15
			"BRAIN",				//16 //Unconfirmed
			"WEAPON",				//17 //Unconfirmed
			"ROOT",					//18 //Unconfirmed
			"COM",					//19 //Unconfirmed
			"PELVIS",				//20
			"CAMERA",				//21 //Unconfirmed
			"ROOT",					//22 //Unconfirmed
			"LEFT_FOOT",			//23
			"RIGHT_FOOT",			//24
			"FACE_TARGET_SOURCE"	//25 //Unconfirmed
		};
		QStringList _ImpactList = { "Small","Medium","Large" };
	};
}