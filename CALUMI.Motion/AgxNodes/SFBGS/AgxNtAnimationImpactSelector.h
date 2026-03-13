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

		inline bool CanSetNameProperty() const override { return false; }

		AgxNodeType GetNodeType() const override;

		std::shared_ptr<AgxPort> _AddPort(AgxPortType portType, AgxPortIndex index, QJsonObject data) override;

	private:
		inline static AgxTerm TORSO() {return					{"TORSO",				QObject::tr("Torso") };}				//0		
		inline static AgxTerm HEAD_1() { return					{ "HEAD_1",				QObject::tr("Head 1") }; }				//1		
		inline static AgxTerm EYE_1() { return					{ "EYE_1",				QObject::tr("Eye 1") }; }				//2 //Unconfirmed		
		inline static AgxTerm LOOKAT_1() { return				{ "LOOKAT_1",			QObject::tr("LookAt 1") }; }			//3 //Unconfirmed			
		inline static AgxTerm FLY_GRAB() { return				{ "FLY_GRAB",			QObject::tr("Fly Grab") }; }			//4 //Unconfirmed			
		inline static AgxTerm HEAD_2() { return					{ "HEAD_2",				QObject::tr("Head 2") }; }				//5 //Unconfirmed		
		inline static AgxTerm LEFT_ARM_1() { return				{ "LEFT_ARM_1",			QObject::tr("Left Arm 1") }; }			//6			
		inline static AgxTerm LEFT_ARM_2() { return				{ "LEFT_ARM_2",			QObject::tr("Left Arm 2") }; }			//7 //Unconfirmed			
		inline static AgxTerm RIGHT_ARM_1() { return			{ "RIGHT_ARM_1",		QObject::tr("Right Arm 1") }; }			//8			
		inline static AgxTerm RIGHT_ARM_2() { return			{ "RIGHT_ARM_2",		QObject::tr("Right Arm 2") }; }			//9 //Unconfirmed			
		inline static AgxTerm LEFT_LEG_1() { return				{ "LEFT_LEG_1",			QObject::tr("Left Leg 1") }; }			//10			
		inline static AgxTerm LEFT_LEG_2() { return				{ "LEFT_LEG_2",			QObject::tr("Left Leg 2") }; }			//11			
		inline static AgxTerm LEFT_LEG_3() { return				{ "LEFT_LEG_3",			QObject::tr("Left Leg 3") }; }			//12			
		inline static AgxTerm RIGHT_LEG_1() { return			{ "RIGHT_LEG_1",		QObject::tr("Right Leg 1") }; }			//13			
		inline static AgxTerm RIGHT_LEG_2() { return			{ "RIGHT_LEG_2",		QObject::tr("Right Leg 2") }; }			//14			
		inline static AgxTerm RIGHT_LEG_3() { return			{ "RIGHT_LEG_3",		QObject::tr("Right Leg 3") }; }			//15			
		inline static AgxTerm BRAIN() { return					{ "BRAIN",				QObject::tr("Brain") }; }				//16 //Unconfirmed		
		inline static AgxTerm WEAPON() { return					{ "WEAPON",				QObject::tr("Weapon") }; }				//17 //Unconfirmed		
		inline static AgxTerm ROOT() { return					{ "ROOT",				QObject::tr("Root") }; }				//18 //Unconfirmed		
		inline static AgxTerm COM() { return					{ "COM",				QObject::tr("Center Of Mass") }; }		//19 //Unconfirmed				
		inline static AgxTerm PELVIS() { return					{ "PELVIS",				QObject::tr("Pelvis") }; }				//20		
		inline static AgxTerm CAMERA() { return					{ "CAMERA",				QObject::tr("Camera") }; }				//21 //Unconfirmed		
		inline static AgxTerm OFFSET_ROOT() { return			{ "OFFSET_ROOT",		QObject::tr("Offset Root") }; }			//22 //Unconfirmed			
		inline static AgxTerm LEFT_FOOT() { return				{ "LEFT_FOOT",			QObject::tr("Left Foot") }; }			//23			
		inline static AgxTerm RIGHT_FOOT() { return				{ "RIGHT_FOOT",			QObject::tr("Right Foot") }; }			//24			
		inline static AgxTerm FACE_TARGET_SOURCE() { return		{ "FACE_TARGET_SOURCE", QObject::tr("Face Target Source") }; }	//25 //Unconfirmed					

		QList<TermRef> _BodyPartList = {
			&TORSO,			
			&HEAD_1,		
			&EYE_1,			
			&LOOKAT_1,		
			&FLY_GRAB,		
			&HEAD_2,		
			&LEFT_ARM_1,	
			&LEFT_ARM_2,	
			&RIGHT_ARM_1,	
			&RIGHT_ARM_2,	
			&LEFT_LEG_1,	
			&LEFT_LEG_2,	
			&LEFT_LEG_3,	
			&RIGHT_LEG_1,	
			&RIGHT_LEG_2,	
			&RIGHT_LEG_3,	
			&BRAIN,			
			&WEAPON,		
			&ROOT,			
			&COM,			
			&PELVIS,		
			&CAMERA,		
			&OFFSET_ROOT,	
			&LEFT_FOOT,			
			&RIGHT_FOOT,			
			&FACE_TARGET_SOURCE	
		};
		QList<TermRef> _ImpactList = { &AgxDictionary::Small,&AgxDictionary::Medium,&AgxDictionary::Large };
	};
}