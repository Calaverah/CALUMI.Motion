//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <qstring.h>
#include <qobject.h>

struct AgxTerm {
	QString tag;
	QString translation;
};

using TermRef = AgxTerm(*)();

class AgxDictionary
{
private:
	AgxDictionary() = default;

	~AgxDictionary() = default;

public:
	AgxDictionary(const AgxDictionary&) = delete;
	AgxDictionary& operator=(const AgxDictionary&) = delete;

	AgxDictionary(AgxDictionary&&) = delete;
	AgxDictionary& operator=(AgxDictionary&&) = delete;

	static AgxDictionary& GetInstance() {
		static AgxDictionary instance;
		return instance;
	}

public:

	inline static AgxTerm ErrorTerm() { return { "Error", QObject::tr("Error") }; }
	inline static AgxTerm BlankEntry() { return { "_entry_", QObject::tr("_entry_") }; }
	inline static AgxTerm BlankEntryE() { return { "~entry~", QObject::tr("~entry~") }; }
	inline static AgxTerm UNKNOWN() { return { "UNKNOWN", QObject::tr("Unknown") }; }

	inline static AgxTerm UserId() { return { "user_id", QObject::tr("User Id") }; }
	inline static AgxTerm NodeType() { return { "node_type", QObject::tr("Node Type") }; }
	inline static AgxTerm UseColor() { return { "use_color_2", QObject::tr("Use Color") }; }
	inline static AgxTerm PosX() { return { "pos_x", QObject::tr("Pos X") }; }
	inline static AgxTerm PosY() { return { "pos_y", QObject::tr("Pos Y") }; }
	inline static AgxTerm ExpPosX() { return { "expanded_pos_x", QObject::tr("Expanded Pos X") }; }
	inline static AgxTerm ExpPosY() { return { "expanded_pos_y", QObject::tr("Expanded Pos Y") }; }
	inline static AgxTerm noninstanced() { return { "noninstanced", QObject::tr("Non Instanced") }; }
	inline static AgxTerm DefaultState() { return { "default_state", QObject::tr("Default State") }; }

	inline static AgxTerm Flags() { return { "Flags", QObject::tr("Flags") }; }

	inline static AgxTerm x_StandtoMoveTags_x() { return {"Stand to Move Tags", QObject::tr("Tag")}; }
	inline static AgxTerm x_MovetoStopTags_x() { return {"Move to Stop Tags", QObject::tr("Tag")}; }
	inline static AgxTerm x_WhileMovingTags_x() { return {"While Moving Tags", QObject::tr("Tag")}; }
	
	inline static AgxTerm x_SwapEvents_x() { return {"Swap Graph Events", QObject::tr("Event")}; }
	inline static AgxTerm x_EnterEvents_x() { return {"Enter Events", QObject::tr("Event")}; }
	inline static AgxTerm x_ExitEvents_x() { return {"Exit Events", QObject::tr("Event")}; }
	inline static AgxTerm EnterEvents() { return { "Enter Events", QObject::tr("Enter Events") }; }
	inline static AgxTerm ExitEvents() { return { "Exit Events", QObject::tr("Exit Events") }; }

	inline static AgxTerm Ankle() { return { "Ankle", QObject::tr("Ankle") }; }
	inline static AgxTerm AssignedValue() { return { "Assigned Value", QObject::tr("Assigned Value") }; }
	inline static AgxTerm AnimTag() { return { "Anim Tag", QObject::tr("Anim Tag") }; }
	inline static AgxTerm ActivateVariable() { return { "Activate Variable", QObject::tr("Activate Variable") }; }
	inline static AgxTerm ActivateBaseEventList() { return { "Activate Base Event List", QObject::tr("Activate Base Event List") }; }
	inline static AgxTerm AimVectorVariable() { return { "Aim Vector Variable", QObject::tr("Aim Vector Variable") }; }
	inline static AgxTerm AimHeadingOffsetOutput() { return { "Aim Heading Offset Output", QObject::tr("Aim Heading Offset Output") }; }
	inline static AgxTerm AimPitchOffsetOutput() { return { "Aim Pitch Offset Output", QObject::tr("Aim Pitch Offset Output") }; }
	inline static AgxTerm AllowNoEffect() { return { "Allow No Effect", QObject::tr("Allow No Effect") }; }
	inline static AgxTerm AnkleLBoneName() { return { "Ankle L Bone Name", QObject::tr("Ankle L Bone Name") }; }
	inline static AgxTerm AnkleRBoneName() { return { "Ankle R Bone Name", QObject::tr("Ankle R Bone Name") }; }
	inline static AgxTerm AngleLMultiplier() { return { "Angle L Multiplier", QObject::tr("Angle L Multiplier") }; }
	inline static AgxTerm AngleRMultiplier() { return { "Angle R Multiplier", QObject::tr("Angle R Multiplier") }; }
	inline static AgxTerm Axis() { return { "Axis", QObject::tr("Axis") }; }
	inline static AgxTerm AngleSpring() { return { "Angle Spring", QObject::tr("Angle Spring") }; }
	inline static AgxTerm AlignOrient() { return { "Align Orient", QObject::tr("Align Orient") }; }
	inline static AgxTerm AlignOrientPercent() { return { "Align Orient Percent", QObject::tr("Align Orient Percent") }; }
	inline static AgxTerm ActiveBlendRate() { return { "Active Blend Rate", QObject::tr("Active Blend Rate") }; }
	inline static AgxTerm AlignmentBone() { return { "Alignment Bone", QObject::tr("Alignment Bone") }; }
	inline static AgxTerm AngleMultiplier() { return { "Angle Multiplier", QObject::tr("Angle Multiplier") }; }
	inline static AgxTerm AllowSelfTransition() { return { "Allow Self Transition", QObject::tr("Allow Self Transition") }; }
	inline static AgxTerm Angle0to1() { return { "Angle 0-1", QObject::tr("Angle 0-1") }; }
	inline static AgxTerm AnkleName() { return { "Ankle Name", QObject::tr("Ankle Name") }; }
	inline static AgxTerm AngMult() { return { "Ang Mult", QObject::tr("Angle Multiplier") }; }
	inline static AgxTerm AssignValue() { return { "Assign Value", QObject::tr("Assign Value") }; }
	inline static AgxTerm Assign() { return { "Assign", QObject::tr("Assign") }; }

	inline static AgxTerm BlendRate() { return { "Blend Rate", QObject::tr("Blend Rate") }; }
	inline static AgxTerm BlendForces() { return { "Blend Forces", QObject::tr("Blend Forces") }; }
	inline static AgxTerm BoneName() { return { "Bone Name", QObject::tr("Bone Name") }; }
	inline static AgxTerm BendAxis() { return { "Bend Axis", QObject::tr("Bend Axis") }; }
	inline static AgxTerm BodyPartVar() { return { "Body Part Var", QObject::tr("Body Part Var") }; }
	inline static AgxTerm BlendTime() { return { "Blend Time", QObject::tr("Blend Time") }; }
	inline static AgxTerm BlendModeFunction() { return { "Blend Mode Function", QObject::tr("Blend Mode Function") }; }
	inline static AgxTerm BlendOutFrames() { return { "Blend Out Frames", QObject::tr("Blend Out Frames") }; }
	inline static AgxTerm BlendOutTime() { return { "Blend Out Time", QObject::tr("Blend Out Time") }; }
	inline static AgxTerm BlendTreeName() { return { "Blend Tree Name", QObject::tr("Blend Tree Name") }; }
	inline static AgxTerm BlendOnOfftime() { return { "Blend On/Off time", QObject::tr("Blend On/Off time") }; }
	inline static AgxTerm BlendFromRagdollTime() { return { "Blend From Ragdoll Time", QObject::tr("Blend From Ragdoll Time") }; }
	inline static AgxTerm Blendratewhenactive() { return { "Blend rate when active", QObject::tr("Blend rate when active") }; }
	inline static AgxTerm Blendratewheninactive() { return { "Blend rate when inactive", QObject::tr("Blend rate when inactive") }; }
	inline static AgxTerm BendBoneName() { return { "Bend Bone Name", QObject::tr("Bend Bone Name") }; }
	inline static AgxTerm BlendwithNext() { return { "Blend with Next", QObject::tr("Blend With Next") }; }
	inline static AgxTerm BlendNextMult() { return { "Blend Next Mult", QObject::tr("Blend Next Mult") }; }
	inline static AgxTerm BodyPart() { return { "Body Part", QObject::tr("Body Part") }; }
	inline static AgxTerm BackTagName() { return { "Back Tag Name", QObject::tr("Back Tag Name") }; }
	inline static AgxTerm BoneVariance() { return { "Bone Variance", QObject::tr("Bone Variance") }; }
	inline static AgxTerm BindingType() { return { "Binding Type", QObject::tr("Binding Type") }; }
	inline static AgxTerm BindingName() { return { "Binding Name", QObject::tr("Binding Name") }; }
	inline static AgxTerm BindingValue() { return { "Binding Value", QObject::tr("Binding Value") }; }
	inline static AgxTerm BoneNames() { return { "Bone Names", QObject::tr("Bone Names") }; }
	inline static AgxTerm Back() { return { "Back", QObject::tr("Back") }; }
	inline static AgxTerm Bones() { return { "Bones", QObject::tr("Bones") }; }
	inline static AgxTerm Bindings() { return { "Bindings", QObject::tr("Bindings") }; }
	inline static AgxTerm BindingVariables() { return { "Binding Variables", QObject::tr("Binding Variables") }; }

	inline static AgxTerm CoreBodyBoneName() { return { "Core Body Bone Name", QObject::tr("Core Body Bone Name") }; }
	inline static AgxTerm ChestAxisIndex() { return { "Chest Axis Index", QObject::tr("Chest Axis Index") }; }
	inline static AgxTerm ChestLocationBone() { return { "Chest Location Bone", QObject::tr("Chest Location Bone") }; }
	inline static AgxTerm ChestFullBlendMaxAngle() { return { "Chest Full Blend Max Angle", QObject::tr("Chest Full Blend Max Angle") }; }
	inline static AgxTerm ClampHeadingMin() { return { "Clamp Heading Min", QObject::tr("Clamp Heading Min") }; }
	inline static AgxTerm ClampHeadingMax() { return { "Clamp Heading Max", QObject::tr("Clamp Heading Max") }; }
	inline static AgxTerm ClampPitchMin() { return { "Clamp Pitch Min", QObject::tr("Clamp Pitch Min") }; }
	inline static AgxTerm ClampPitchMax() { return { "Clamp Pitch Max", QObject::tr("Clamp Pitch Max") }; }
	inline static AgxTerm CleanUpOnly() { return { "Clean Up Only", QObject::tr("Clean Up Only") }; }
	inline static AgxTerm ControlVariable() { return { "Control Variable", QObject::tr("Control Variable") }; }
	inline static AgxTerm CropStartFrames() { return { "Crop Start Frames", QObject::tr("Crop Start Frames") }; }
	inline static AgxTerm CropEndFrames() { return { "Crop End Frames", QObject::tr("Crop End Frames") }; }
	inline static AgxTerm CurrentCumulativeCount() { return { "Current Cumulative Count", QObject::tr("Current Cumulative Count") }; }
	inline static AgxTerm CastDistanceUp() { return { "Cast Distance Up", QObject::tr("Cast Distance Up") }; }
	inline static AgxTerm CastDistanceDown() { return { "Cast Distance Down", QObject::tr("Cast Distance Down") }; }
	inline static AgxTerm CastDistanceUpVariable() { return { "Cast Distance Up Variable", QObject::tr("Cast Distance Up Variable") }; }
	inline static AgxTerm CastDistanceDownVariable() { return { "Cast Distance Down Variable", QObject::tr("Cast Distance Down Variable") }; }
	inline static AgxTerm CurrentSpeed() { return { "Current Speed", QObject::tr("Current Speed") }; }
	inline static AgxTerm CurrentDirection() { return { "Current Direction", QObject::tr("Current Direction") }; }
	inline static AgxTerm ContactNormalBlendRate() { return { "Contact Normal Blend Rate", QObject::tr("Contact Normal Blend Rate") }; }
	inline static AgxTerm ChancetoRepick() { return { "Chance to Repick", QObject::tr("Chance to Repick") }; }
	inline static AgxTerm Crossblendtime() { return { "Cross blend time", QObject::tr("Cross blend time") }; }
	inline static AgxTerm COMBlendAmount() { return { "COM Blend Amount", QObject::tr("COM Blend Amount") }; }
	inline static AgxTerm ClampedBlendTime() { return { "Clamped Blend Time", QObject::tr("Clamped Blend Time") }; }
	inline static AgxTerm CounterAttackEvent() { return { "Counter Attack Event", QObject::tr("Counter Attack Event") }; }
	inline static AgxTerm CounterAttackThreshold() { return { "Counter Attack Threshold", QObject::tr("Counter Attack Threshold") }; }
	inline static AgxTerm CounterAttackChance() { return { "Counter Attack Chance", QObject::tr("Counter Attack Chance") }; }
	inline static AgxTerm CounterAttackCooldown() { return { "Counter Attack Cooldown", QObject::tr("Counter Attack Cooldown") }; }
	inline static AgxTerm Comment() { return { "Comment", QObject::tr("Comment") }; }
	inline static AgxTerm Condition() { return { "Condition", QObject::tr("Condition") }; }
	inline static AgxTerm CHeadingMin() { return { "C Heading Min", QObject::tr("C Heading Min") }; }
	inline static AgxTerm CHeadingMax() { return { "C Heading Max", QObject::tr("C Heading Max") }; }
	inline static AgxTerm CPitchMin() { return { "C Pitch Min", QObject::tr("C Pitch Min") }; }
	inline static AgxTerm CPitchMax() { return { "C Pitch Max", QObject::tr("C Pitch Max") }; }
	inline static AgxTerm CancelEvent() { return { "Cancel Event", QObject::tr("Cancel Event") }; }
	inline static AgxTerm ComHghtMin() { return { "Com Hght Min", QObject::tr("COM Height Min") }; }
	inline static AgxTerm CHip() { return { "CHip", QObject::tr("CHip") }; }
	inline static AgxTerm CKnee() { return { "CKnee", QObject::tr("CKnee") }; }
	inline static AgxTerm CAnkle() { return { "CAnkle", QObject::tr("CAnkle") }; }

	inline static AgxTerm Decrement() { return { "Decrement", QObject::tr("Decrement") }; }
	inline static AgxTerm DoNothing() { return { "Do Nothing", QObject::tr("Do Nothing") }; }
	inline static AgxTerm Dampen() { return { "Dampen", QObject::tr("Dampen") }; }
	inline static AgxTerm Def_() { return { "Def.", QObject::tr("Definition") }; }
	inline static AgxTerm Default() { return { "Default", QObject::tr("Default") }; }
	inline static AgxTerm DoNotFollowSyncGroup() { return { "Do Not Follow Sync Group", QObject::tr("Do Not Follow Sync Group") }; }
	inline static AgxTerm DesiredSpeedVar() { return { "Desired Speed Var", QObject::tr("Desired Speed Var") }; }
	inline static AgxTerm DisableWhenInactive() { return { "Disable When Inactive", QObject::tr("Disable When Inactive") }; }
	inline static AgxTerm DisableBefore() { return { "Disable Before", QObject::tr("Disable Before") }; }
	inline static AgxTerm DisableAfter() { return { "Disable After", QObject::tr("Disable After") }; }
	inline static AgxTerm DeadZone() { return { "Dead Zone", QObject::tr("Dead Zone") }; }
	inline static AgxTerm DesiredDirection() { return { "Desired Direction", QObject::tr("Desired Direction") }; }
	inline static AgxTerm DesiredSpeed() { return { "Desired Speed", QObject::tr("Desired Speed") }; }
	inline static AgxTerm DirectionVar() { return { "Direction Var", QObject::tr("Direction Var") }; }
	inline static AgxTerm Direction() { return { "Direction", QObject::tr("Direction") }; }
	inline static AgxTerm Directions() { return { "Directions", QObject::tr("Directions") }; }
	inline static AgxTerm DirectionIsRadians() { return { "Direction Is Radians", QObject::tr("Direction Is Radians") }; }
	inline static AgxTerm DampenOverride() { return { "Dampen Override", QObject::tr("Dampen Override") }; }
	inline static AgxTerm DirectAtVectorVariable() { return { "DirectAtVector Variable", QObject::tr("DirectAtVector Variable") }; }
	inline static AgxTerm DisableVariable() { return { "Disable Variable", QObject::tr("Disable Variable") }; }
	inline static AgxTerm DisableHeadingMin() { return { "Disable Heading Min", QObject::tr("Disable Heading Min") }; }
	inline static AgxTerm DisableHeadingMax() { return { "Disable Heading Max", QObject::tr("Disable Heading Max") }; }
	inline static AgxTerm DisableHeadingHysteresis() { return { "Disable Heading Hysteresis", QObject::tr("Disable Heading Hysteresis") }; }
	inline static AgxTerm DiscardEvents() { return { "Discard Events", QObject::tr("Discard Events") }; }
	inline static AgxTerm DownErrorBias() { return { "Down Error Bias", QObject::tr("Down Error Bias") }; }
	inline static AgxTerm DesiredValue() { return { "Desired Value", QObject::tr("Desired Value") }; }
	inline static AgxTerm DisableLimits() { return { "Disable Limits", QObject::tr("Disable Limits") }; }
	inline static AgxTerm DisableCustomBonesWhenBlendingOut() { return { "Disable Custom Bones When Blending Out", QObject::tr("Disable Custom Bones When Blending Out") }; }
	inline static AgxTerm DisableEvents() { return { "Disable Events", QObject::tr("Disable Events") }; }

	inline static AgxTerm Event() { return { "Event", QObject::tr("Event") }; }
	inline static AgxTerm EventandVariable() { return { "Event and Variable", QObject::tr("Event and Variable") }; }
	inline static AgxTerm EndOrient() { return { "End Orient", QObject::tr("End Orient") }; }
	inline static AgxTerm Entries() { return { "Entries", QObject::tr("Entries") }; }
	inline static AgxTerm EvaluatedCondition() { return { "Evaluated Condition", QObject::tr("Evaluated Condition") }; }
	inline static AgxTerm ExtraName() { return { "Extra Name", QObject::tr("Extra Name") }; }
	inline static AgxTerm EnableEvents() { return { "Enable Events", QObject::tr("Enable Events") }; }
	inline static AgxTerm EventList() { return { "Event List", QObject::tr("Event List") }; }
	inline static AgxTerm EventName() { return { "Event Name", QObject::tr("Event Name") }; }
	inline static AgxTerm EndValue() { return { "End Value", QObject::tr("End Value") }; }
	inline static AgxTerm EnforceDirectAtLimits() { return { "Enforce DirectAt Limits", QObject::tr("Enforce DirectAt Limits") }; }
	inline static AgxTerm EnumerateTag() { return { "Enumerate Tag", QObject::tr("Enumerate Tag") }; }
	inline static AgxTerm EventSentAfterMaxIterations() { return { "Event Sent After Max Iterations", QObject::tr("Event Sent After Max Iterations") }; }
	inline static AgxTerm EventtoResetAllCumulativeAnimations() { return { "Event to Reset All Cumulative Animations", QObject::tr("Event to Reset All Cumulative Animations") }; }
	inline static AgxTerm EHip() { return { "EHip", QObject::tr("EHip") }; }
	inline static AgxTerm EKnee() { return { "EKnee", QObject::tr("EKnee") }; }
	inline static AgxTerm EAnkle() { return { "EAnkle", QObject::tr("EAnkle") }; }
	inline static AgxTerm EndPosition() { return { "End Position", QObject::tr("End Position") }; }

	inline static AgxTerm FinalBoneAxisIndex() { return { "Final Bone Axis Index", QObject::tr("Final Bone Axis Index") }; }
	inline static AgxTerm FootUnlockBlendScalar() { return { "Foot Unlock Blend Scalar", QObject::tr("Foot Unlock Blend Scalar") }; }
	inline static AgxTerm ForwardTagName() { return { "Forward Tag Name", QObject::tr("Forward Tag Name") }; }
	inline static AgxTerm Forward() { return { "Forward", QObject::tr("Forward") }; }

	inline static AgxTerm Graphs() { return { "Graphs", QObject::tr("Graphs") }; }
	inline static AgxTerm GraphRef() { return { "Graph Ref", QObject::tr("Graph Ref") }; }
	inline static AgxTerm GraphName() { return { "Graph Name", QObject::tr("Graph Name") }; }
	inline static AgxTerm GetUpEvent() { return { "Get Up Event", QObject::tr("Get Up Event") }; }
	inline static AgxTerm GetUpStartEvent() { return { "Get Up Start Event", QObject::tr("Get Up Start Event") }; }
	inline static AgxTerm GoDynamicOnContact() { return { "Go Dynamic On Contact", QObject::tr("Go Dynamic On Contact") }; }

	inline static AgxTerm HeadBoneName() { return { "Head Bone Name", QObject::tr("Head Bone Name") }; }
	inline static AgxTerm HeadingDeltaVariable() { return { "Heading Delta Variable", QObject::tr("Heading Delta Variable") }; }
	inline static AgxTerm HeadingOffsetOutput() { return { "Heading Offset Output", QObject::tr("Heading Offset Output") }; }
	inline static AgxTerm HeadingLagDegrees() { return { "Heading Lag Degrees", QObject::tr("Heading Lag Degrees") }; }
	inline static AgxTerm HeadLocationBone() { return { "Head Location Bone", QObject::tr("Head Location Bone") }; }
	inline static AgxTerm HeadLagDegrees() { return { "Head Lag Degrees", QObject::tr("Head Lag Degrees") }; }
	inline static AgxTerm HeadBlendRate() { return { "Head Blend Rate", QObject::tr("Head Blend Rate") }; }
	inline static AgxTerm HasLoopingSegment() { return { "Has Looping Segment", QObject::tr("Has Looping Segment") }; }
	inline static AgxTerm HipLBoneName() { return { "Hip L Bone Name", QObject::tr("Hip L Bone Name") }; }
	inline static AgxTerm HipRBoneName() { return { "Hip R Bone Name", QObject::tr("Hip R Bone Name") }; }
	inline static AgxTerm HipsBlendAmount() { return { "Hips Blend Amount", QObject::tr("Hips Blend Amount") }; }
	inline static AgxTerm HipName() { return { "Hip Name", QObject::tr("Hip Name") }; }
	inline static AgxTerm Hip() { return { "Hip", QObject::tr("Hip") }; }
	inline static AgxTerm Headings() { return { "Headings", QObject::tr("Headings") }; }

	inline static AgxTerm IsActiveBool() { return { "Is Active Bool", QObject::tr("Is Active Bool") }; }
	inline static AgxTerm ImpactVar() { return { "Impact Var", QObject::tr("Impact Var") }; }
	inline static AgxTerm InertiaSpring() { return { "Inertia Spring", QObject::tr("Inertia Spring") }; }
	inline static AgxTerm IgnoreContactBones() { return { "Ignore Contact Bones", QObject::tr("Ignore Contact Bones") }; }
	inline static AgxTerm iStateVariable() { return{ "iState Variable", QObject::tr("iState Variable") }; }
	inline static AgxTerm InputLockTime() { return { "Input Lock Time", QObject::tr("Input Lock Time") }; }
	inline static AgxTerm IsActiveVar() { return { "Is Active Var", QObject::tr("Is Active Var") }; }
	inline static AgxTerm IsHeadTrackingActiveVariable() { return { "Is Head Tracking Active Variable", QObject::tr("Is Head Tracking Active Variable") }; }
	inline static AgxTerm IsAimActiveVariable() { return { "Is Aim Active Variable", QObject::tr("Is Aim Active Variable") }; }
	inline static AgxTerm IsActionAnimation() { return { "Is Action Animation", QObject::tr("Is Action Animation") }; }
	inline static AgxTerm InitializeSequenceOnLoad() { return { "Initialize Sequence On Load", QObject::tr("Initialize Sequence On Load") }; }
	inline static AgxTerm InactiveBlendRate() { return { "Inactive Blend Rate", QObject::tr("Inactive Blend Rate") }; }
	inline static AgxTerm IsUsingCodeDrivenRotation() { return { "Is Using Code Driven Rotation", QObject::tr("Is Using Code Driven Rotation") }; }
	inline static AgxTerm ImmunityThreshold() { return { "Immunity Threshold", QObject::tr("Immunity Threshold") }; }
	inline static AgxTerm ImmunityDelayBeforeDecay() { return { "Immunity Delay Before Decay", QObject::tr("Immunity Delay Before Decay") }; }
	inline static AgxTerm ImmunityDecayRate() { return { "Immunity Decay Rate", QObject::tr("Immunity Decay Rate") }; }
	inline static AgxTerm ImmunityConsumeRate() { return { "Immunity Consume Rate", QObject::tr("Immunity Consume Rate") }; }
	inline static AgxTerm Impact() { return { "Impact", QObject::tr("Impact") }; }
	inline static AgxTerm iStateName() { return { "iState Name", QObject::tr("iState Name") }; }
	inline static AgxTerm iStateValue() { return { "iState Value", QObject::tr("iState Value") }; }
	inline static AgxTerm iStateAssignments() { return { "iState Assignments", QObject::tr("iState Assignments") }; }
	inline static AgxTerm Increment() { return { "Increment", QObject::tr("Increment") }; }

	inline static AgxTerm Jog() { return { "Jog", QObject::tr("Jog") }; }

	inline static AgxTerm KneeLBoneName() { return { "Knee L Bone Name", QObject::tr("Knee L Bone Name") }; }
	inline static AgxTerm KneeRBoneName() { return { "Knee R Bone Name", QObject::tr("Knee R Bone Name") }; }
	inline static AgxTerm KneeName() { return { "Knee Name", QObject::tr("Knee Name") }; }
	inline static AgxTerm Knee() { return { "Knee", QObject::tr("Knee") }; }
	
	inline static AgxTerm LimitToSubgraph() { return { "Limit To Subgraph", QObject::tr("Limit To Subgraph") }; }
	inline static AgxTerm LagDegrees() { return { "Lag Degrees", QObject::tr("Lag Degrees") }; }
	inline static AgxTerm LeadSuffix() { return { "Lead Suffix", QObject::tr("Lead Suffix") }; }
	inline static AgxTerm LookVectorVariable() { return { "Look Vector Variable", QObject::tr("Look Vector Variable") }; }
	inline static AgxTerm LogicVariable() { return { "Logic Variable", QObject::tr("Logic Variable") }; }
	inline static AgxTerm ListenEvent() { return { "Listen Event", QObject::tr("Listen Event") }; }
	inline static AgxTerm LeftFootLockOverride() { return { "Left Foot Lock Override", QObject::tr("Left Foot Lock Override") }; }
	inline static AgxTerm LeftFootUnlockOverride() { return { "Left Foot Unlock Override", QObject::tr("Left Foot Unlock Override") }; }
	inline static AgxTerm RightFootLockOverride() { return { "Right Foot Lock Override", QObject::tr("Right Foot Lock Override") }; }
	inline static AgxTerm RightFootUnlockOverride() { return { "Right Foot Unlock Override", QObject::tr("Right Foot Unlock Override") }; }
	inline static AgxTerm LeftHandBoneName() { return { "Left Hand Bone Name", QObject::tr("Left Hand Bone Name") }; }
	inline static AgxTerm LeftLegBoneName() { return { "Left Leg Bone Name", QObject::tr("Left Leg Bone Name") }; }
	inline static AgxTerm LeftTagName() { return { "Left Tag Name", QObject::tr("Left Tag Name") }; }
	inline static AgxTerm LegsAExtra() { return { "LegsA Extra", QObject::tr("LegsA Extra") }; }
	inline static AgxTerm LegsBExtra() { return { "LegsB Extra", QObject::tr("LegsB Extra") }; }
	inline static AgxTerm LegsCExtra() { return { "LegsC Extra", QObject::tr("LegsC Extra") }; }
	inline static AgxTerm LegsDExtra() { return { "LegsD Extra", QObject::tr("LegsD Extra") }; }
	inline static AgxTerm LegsEExtra() { return { "LegsE Extra", QObject::tr("LegsE Extra") }; }
	inline static AgxTerm LegsFExtra() { return { "LegsF Extra", QObject::tr("LegsF Extra") }; }
	inline static AgxTerm Left() { return { "Left", QObject::tr("Left") }; }
	inline static AgxTerm Linear() { return { "Linear", QObject::tr("Linear") }; }
	inline static AgxTerm Layer() { return { "Layer", QObject::tr("Layer") }; }
	inline static AgxTerm Large() { return { "Large", QObject::tr("Large") }; }
	inline static AgxTerm LoopingLeft() { return { "Looping Left", QObject::tr("Looping Left") }; }
	inline static AgxTerm LoopingRight() { return { "Looping Right", QObject::tr("Looping Right") }; }
	inline static AgxTerm LongestDirection() { return { "Longest Direction", QObject::tr("Longest Direction") }; }
	
	inline static AgxTerm Max() { return { "Max", QObject::tr("Max") }; }
	inline static AgxTerm Medium() { return { "Medium", QObject::tr("Medium") }; }
	inline static AgxTerm MoveToStop() { return { "Move To Stop", QObject::tr("Move To Stop") }; }
	inline static AgxTerm Mirror() { return { "Mirror", QObject::tr("Mirror") }; }
	inline static AgxTerm MinScale() { return { "Min Scale", QObject::tr("Min Scale") }; }
	inline static AgxTerm MaxScale() { return { "Max Scale", QObject::tr("Max Scale") }; }
	inline static AgxTerm MaxCorrectionVel() { return { "Max Correction Vel", QObject::tr("Max Correction Vel") }; }
	inline static AgxTerm MaxPostCorrectionVel() { return { "Max Post Correction Vel", QObject::tr("Max Post Correction Vel") }; }
	inline static AgxTerm MaxCorrectionSpeed() { return { "Max Correction Speed", QObject::tr("Max Correction Speed") }; }
	inline static AgxTerm Mass() { return { "Mass", QObject::tr("Mass") }; }
	inline static AgxTerm MassOverride() { return { "Mass Override", QObject::tr("Mass Override") }; }
	inline static AgxTerm MinTimes() { return { "Min Times", QObject::tr("Min Times") }; }
	inline static AgxTerm Mintime() { return { "Min time", QObject::tr("Min Time") }; }
	inline static AgxTerm MinTime() { return { "Min Time", QObject::tr("Min Time") }; }
	inline static AgxTerm Maxtime() { return { "Max time", QObject::tr("Max Time") }; }
	inline static AgxTerm MaxTime() { return { "Max Time", QObject::tr("Max Time") }; }
	inline static AgxTerm MaxTimes() { return { "Max Times", QObject::tr("Max Times") }; }
	inline static AgxTerm MaxBlendTime() { return { "Max Blend Time", QObject::tr("Max Blend Time") }; }
	inline static AgxTerm MaxCumulativeAnimation() { return { "Max Cumulative Animation", QObject::tr("Max Cumulative Animation") }; }
	inline static AgxTerm MinBendAngle() { return { "Min Bend Angle", QObject::tr("Min Bend Angle") }; }
	inline static AgxTerm MaxBendAngle() { return { "Max Bend Angle", QObject::tr("Max Bend Angle") }; }
	inline static AgxTerm MaxIKUp() { return { "Max IK Up", QObject::tr("Max IK Up") }; }
	inline static AgxTerm MaxIKUpVariable() { return { "Max IK Up Variable", QObject::tr("Max IK Up Variable") }; }
	inline static AgxTerm MaxStrideLength() { return { "Max Stride Length", QObject::tr("Max Stride Length") }; }
	inline static AgxTerm MaxPostCorrectionSpeed() { return { "Max Post Correction Speed", QObject::tr("Max Post Correction Speed") }; }
	inline static AgxTerm MaxControllerSeperation() { return { "Max Controller Seperation", QObject::tr("Max Controller Seperation") }; }
	inline static AgxTerm MinimumActiveTime() { return { "Minimum Active Time", QObject::tr("Minimum Active Time") }; }
	inline static AgxTerm MoveStartCorrectionTime() { return { "Move Start Correction Time", QObject::tr("Move Start Correction Time") }; }
	inline static AgxTerm MinX() { return { "Min X", QObject::tr("Min X") }; }
	inline static AgxTerm MaxX() { return { "Max X", QObject::tr("Max X") }; }
	inline static AgxTerm MinY() { return { "Min Y", QObject::tr("Min Y") }; }
	inline static AgxTerm MaxY() { return { "Max Y", QObject::tr("Max Y") }; }
	inline static AgxTerm MinZ() { return { "Min Z", QObject::tr("Min Z") }; }
	inline static AgxTerm MaxZ() { return { "Max Z", QObject::tr("Max Z") }; }
	inline static AgxTerm MaxSpeedMult() { return { "Max Speed Mult", QObject::tr("Max Speed Mult") }; }
	inline static AgxTerm MaintainSyncSelectionVariableWhenExiting() { return { "Maintain Sync Selection Variable When Exiting", QObject::tr("Maintain Sync Selection Variable When Exiting") }; }
	
	inline static AgxTerm Name() { return { "Name", QObject::tr("Name") }; }
	inline static AgxTerm None() { return { "None", QObject::tr("None") }; }
	inline static AgxTerm NodeID() { return { "Node ID", QObject::tr("Node ID") }; }
	inline static AgxTerm NonInstanced() { return { "Non Instanced", QObject::tr("Non Instanced") }; }
	inline static AgxTerm NormalizeLength() { return { "Normalize Length", QObject::tr("Normalize Length") }; }
	inline static AgxTerm NormalizeWeights() { return { "Normalize Weights", QObject::tr("Normalize Weights") }; }
	inline static AgxTerm NoHeadTrackVariable() { return { "No Head Track Variable", QObject::tr("No Head Track Variable") }; }
	inline static AgxTerm NoAimVariable() { return { "No Aim Variable", QObject::tr("No Aim Variable") }; }
	inline static AgxTerm NegateVariable() { return { "Negate Variable", QObject::tr("Negate Variable") }; }
	inline static AgxTerm NPCHeadingOffset_Var() { return { "NPCHeadingOffset Var", QObject::tr("NPC Heading Offset Var") }; }
	
	inline static AgxTerm OnlyAssignOnce() { return { "Only Assign Once", QObject::tr("Only Assign Once") }; }
	inline static AgxTerm OnlyHighestWeightActive() { return { "Only Highest Weight Active", QObject::tr("Only Highest Weight Active") }; }
	inline static AgxTerm Operation() { return { "Operation", QObject::tr("Operation") }; }
	inline static AgxTerm OrientVar() { return { "Orient Var", QObject::tr("Orient Var") }; }
	inline static AgxTerm OutputVariable() { return { "Output Variable", QObject::tr("Output Variable") }; }
	inline static AgxTerm OverrideAnimDirection() { return { "Override Anim Direction", QObject::tr("Override Anim Direction") }; }
	inline static AgxTerm OrientedOffset() { return { "Oriented Offset", QObject::tr("Oriented Offset") }; }
	inline static AgxTerm Output() { return { "Output", QObject::tr("Output") }; }
	inline static AgxTerm Orientation() { return { "Orientation", QObject::tr("Orientation") }; }
	inline static AgxTerm OneMinusWeight() { return { "One Minus Weight", QObject::tr("One Minus Weight") }; }
	
	inline static AgxTerm ProcessInactiveEvents() { return { "Process Inactive Events", QObject::tr("Process Inactive Events") }; }
	inline static AgxTerm PitchOffsetOutput() { return { "Pitch Offset Output", QObject::tr("Pitch Offset Output") }; }
	//This shows up in two forms, might be a typo in the BGS source code...
	inline static AgxTerm Position_ChangeEvent() { return { "Position Change Event", QObject::tr("Position Change Event") }; }
	inline static AgxTerm PositionChangeEvent() { return { "PositionChange Event", QObject::tr("Position Change Event") }; }
	inline static AgxTerm PlaybackSpeed() { return { "Playback Speed", QObject::tr("Playback Speed") }; }
	inline static AgxTerm PositionVar() { return { "Position Var", QObject::tr("Position Var") }; }
	inline static AgxTerm ParentBoneName() { return { "Parent Bone Name", QObject::tr("Parent Bone Name") }; }
	inline static AgxTerm Priority() { return { "Priority", QObject::tr("Priority") }; }
	inline static AgxTerm Payload() { return { "Payload", QObject::tr("Payload") }; }
	inline static AgxTerm Prefix() { return { "Prefix", QObject::tr("Prefix") }; }
	inline static AgxTerm Param() { return { "Param", QObject::tr("Parameter") }; }
	inline static AgxTerm Parametric() { return { "Parametric", QObject::tr("Parametric") }; }
	inline static AgxTerm PickWeight() { return { "Pick Weight", QObject::tr("Pick Weight") }; }
	inline static AgxTerm PickWeights() { return { "Pick Weights", QObject::tr("Pick Weights") }; }
	inline static AgxTerm PropagateTagUpstream() { return { "Propagate Tag Upstream", QObject::tr("Propagate Tag Upstream") }; }
	inline static AgxTerm PairedAnimNameRoot() { return { "Paired Anim Name Root", QObject::tr("Paired Anim Name Root") }; }
	
	inline static AgxTerm RandomizeStartPoint() { return { "Randomize Start Point", QObject::tr("Randomize Start Point") }; }
	inline static AgxTerm RangeMin() { return { "Range Min", QObject::tr("Range Min") }; }
	inline static AgxTerm RangeMax() { return { "Range Max", QObject::tr("Range Max") }; }
	inline static AgxTerm ResumeEvent() { return { "Resume Event", QObject::tr("Resume Event") }; }
	inline static AgxTerm ResetAfterMaxIterations() { return { "Reset After Max Iterations", QObject::tr("Reset After Max Iterations") }; }
	inline static AgxTerm RotateHeadBeforeChest() { return { "Rotate Head Before Chest", QObject::tr("Rotate Head Before Chest") }; }
	inline static AgxTerm ReleaseEvent() { return { "Release Event", QObject::tr("Release Event") }; }
	inline static AgxTerm RootFeedbackWeight() { return { "Root Feedback Weight", QObject::tr("Root Feedback Weight") }; }
	inline static AgxTerm RootTwistBlendRate() { return { "Root Twist Blend Rate", QObject::tr("Root Twist Blend Rate") }; }
	inline static AgxTerm RangeMinMultiplier() { return { "Range Min Multiplier", QObject::tr("Range Min Multiplier") }; }
	inline static AgxTerm RangeMaxMultiplier() { return { "Range Max Multiplier", QObject::tr("Range Max Multiplier") }; }
	inline static AgxTerm RampInTime() { return { "Ramp In Time", QObject::tr("Ramp In Time") }; }
	inline static AgxTerm RunNormalized() { return { "Run Normalized", QObject::tr("Run Normalized") }; }
	inline static AgxTerm RightHandBoneName() { return { "Right Hand Bone Name", QObject::tr("Right Hand Bone Name") }; }
	inline static AgxTerm RightLegBoneName() { return { "Right Leg Bone Name", QObject::tr("Right Leg Bone Name") }; }
	inline static AgxTerm RepickAtEnd() { return { "Repick At End", QObject::tr("Repick At End") }; }
	inline static AgxTerm RootBlendAmount() { return { "Root Blend Amount", QObject::tr("Root Blend Amount") }; }
	inline static AgxTerm RotationType() { return { "Rotation Type", QObject::tr("Rotation Type") }; }
	inline static AgxTerm RandomizeIfInvalid() { return { "Randomize If Invalid", QObject::tr("Randomize If Invalid") }; }
	inline static AgxTerm ResetEventName() { return { "Reset Event Name", QObject::tr("Reset Event Name") }; }
	inline static AgxTerm RightTagName() { return { "Right Tag Name", QObject::tr("Right Tag Name") }; }
	inline static AgxTerm RigPath() { return { "Rig Path", QObject::tr("Rig Path") }; }
	inline static AgxTerm RigName() { return { "Rig Name", QObject::tr("Rig Name") }; }
	inline static AgxTerm Right() { return { "Right", QObject::tr("Right") }; }
	inline static AgxTerm Run() { return { "Run", QObject::tr("Run") }; }
	inline static AgxTerm ReceiveTagfromDownstream() { return { "Receive Tag from Downstream", QObject::tr("Receive Tag from Downstream") }; }
	
	inline static AgxTerm StaggerForceVariable() { return { "Stagger Force Variable", QObject::tr("Stagger Force Variable") }; }
	inline static AgxTerm StaggerCooldown() { return { "Stagger Cooldown", QObject::tr("Stagger Cooldown") }; }
	inline static AgxTerm SyncSystem() { return { "Sync System", QObject::tr("Sync System") }; }
	inline static AgxTerm SyncSlave() { return { "Sync Slave", QObject::tr("Sync Slave") }; }
	inline static AgxTerm SendEventOnEnd() { return { "Send Event On End", QObject::tr("Send Event On End") }; }
	inline static AgxTerm StateVariable() { return { "State Variable", QObject::tr("State Variable") }; }
	inline static AgxTerm StateVariableRefCount() { return { "State Variable Ref Count", QObject::tr("State Variable Ref Count") }; }
	inline static AgxTerm SpeedMultiplier() { return { "Speed Multiplier", QObject::tr("Speed Multiplier") }; }
	inline static AgxTerm SpeedMultiplierIsDuration() { return { "Speed Multiplier Is Duration", QObject::tr("Speed Multiplier Is Duration") }; }
	inline static AgxTerm SyncOnlyTransitionOut() { return { "Sync Only Transition Out", QObject::tr("Sync Only Transition Out") }; }
	inline static AgxTerm SyncVariable() { return { "Sync Variable", QObject::tr("Sync Variable") }; }
	inline static AgxTerm SourceLocationBone() { return { "Source Location Bone", QObject::tr("Source Location Bone") }; }
	inline static AgxTerm SourceBone() { return { "Source Bone", QObject::tr("Source Bone") }; }
	inline static AgxTerm SwapGraphEvent() { return { "Swap Graph Event", QObject::tr("Swap Graph Event") }; }
	inline static AgxTerm StartingGraph() { return { "Starting Graph", QObject::tr("Starting Graph") }; }
	inline static AgxTerm SequenceName() { return { "Sequence Name", QObject::tr("Sequence Name") }; }
	inline static AgxTerm SequenceCurrentFrameIndex() { return { "Sequence Current Frame Index", QObject::tr("Sequence Current Frame Index") }; }
	inline static AgxTerm StateMachineName() { return { "State Machine Name", QObject::tr("State Machine Name") }; }
	inline static AgxTerm StateMachine() { return { "State Machine", QObject::tr("State Machine") }; }
	inline static AgxTerm SendEventOnCatch() { return { "Send Event On Catch", QObject::tr("Send Event On Catch") }; }
	inline static AgxTerm SendEvent() { return { "Send Event", QObject::tr("Send Event") }; }
	inline static AgxTerm Speed() { return { "Speed", QObject::tr("Speed") }; }
	inline static AgxTerm SpringCoefficientOverride() { return { "Spring Coefficient Override", QObject::tr("Spring Coefficient Override") }; }
	inline static AgxTerm SpeedParameter() { return { "Speed Parameter", QObject::tr("Speed Parameter") }; }
	inline static AgxTerm SpringCoefficient() { return { "Spring Coefficient", QObject::tr("Spring Coefficient") }; }
	inline static AgxTerm ScaleSetVariable() { return { "Scale Set Variable", QObject::tr("Scale Set Variable") }; }
	inline static AgxTerm SpeedVar() { return { "Speed Var", QObject::tr("Speed Var") }; }
	inline static AgxTerm StartValue() { return { "Start Value", QObject::tr("Start Value") }; }
	inline static AgxTerm SourceBoneName() { return { "Source Bone Name", QObject::tr("Source Bone Name") }; }
	inline static AgxTerm StoryManagerEvent() { return { "SM Evt", QObject::tr("Story Manager Event") }; }
	inline static AgxTerm Set() { return { "Set", QObject::tr("Set") }; }
	inline static AgxTerm StateName() { return { "State Name", QObject::tr("State Name") }; }
	inline static AgxTerm SpeedClassification() { return { "Speed Classification", QObject::tr("Speed Classification") }; }
	inline static AgxTerm StartSelectionMethod() { return { "Start Selection Method", QObject::tr("Start Selection Method") }; }
	inline static AgxTerm StartSelectionVariable() { return { "Start Selection Variable", QObject::tr("Start Selection Variable") }; }
	inline static AgxTerm SwapGraphEvents() { return { "Swap Graph Events", QObject::tr("Swap Graph Events") }; }
	inline static AgxTerm States() { return { "States", QObject::tr("States") }; }
	inline static AgxTerm StandToMove() { return { "Stand To Move", QObject::tr("Stand To Move") }; }
	inline static AgxTerm Standard() { return { "Standard", QObject::tr("Standard") }; }
	inline static AgxTerm Small() { return { "Small", QObject::tr("Small") }; }
	inline static AgxTerm Sync() { return { "Sync", QObject::tr("Sync") }; }
	inline static AgxTerm Suffix() { return { "Suffix", QObject::tr("Suffix") }; }
	inline static AgxTerm Smooth() { return { "Smooth", QObject::tr("Smooth") }; }
	inline static AgxTerm ShortestDirection() { return { "Shortest Direction", QObject::tr("Shortest Direction") }; }
	inline static AgxTerm StartOrient() { return { "Start Orient", QObject::tr("Start Orient") }; }
	inline static AgxTerm StartPosition() { return { "Start Position", QObject::tr("Start Position") }; }
	
	inline static AgxTerm Tag() { return { "Tag", QObject::tr("Tag") }; }
	inline static AgxTerm TagName() { return { "TagName", QObject::tr("Tag Name") }; }
	inline static AgxTerm Tag_Name() { return { "Tag Name", QObject::tr("Tag Name") }; }
	inline static AgxTerm TagAlteration() { return { "Tag Alteration", QObject::tr("Tag Alteration") }; }
	inline static AgxTerm TagSyncSystem() { return { "Tag Sync System", QObject::tr("Tag Sync System") }; }
	inline static AgxTerm TypeofPropagation() { return { "Type of Propagation", QObject::tr("Type of Propagation") }; }
	inline static AgxTerm Type() { return { "Type", QObject::tr("Type") }; }
	inline static AgxTerm Time() { return { "Time", QObject::tr("Time") }; }
	inline static AgxTerm TurnAnimCameraVar() { return { "Turn Anim Camera Var", QObject::tr("Turn Anim Camera Var") }; }
	inline static AgxTerm TargetBone() { return { "Target Bone", QObject::tr("Target Bone") }; }
	inline static AgxTerm TimePercent() { return { "Time Percent", QObject::tr("Time Percent") }; }
	inline static AgxTerm TimePercentMin() { return { "Time Percent Min", QObject::tr("Time Percent Min") }; }
	inline static AgxTerm TimePercentMax() { return { "Time Percent Max", QObject::tr("Time Percent Max") }; }
	inline static AgxTerm ToeLBoneName() { return { "Toe L Bone Name", QObject::tr("Toe L Bone Name") }; }
	inline static AgxTerm ToeRBoneName() { return { "Toe R Bone Name", QObject::tr("Toe R Bone Name") }; }
	inline static AgxTerm TargetBlendTime() { return { "Target Blend Time", QObject::tr("Target Blend Time") }; }
	inline static AgxTerm ThighsBlendAmount() { return { "Thighs Blend Amount", QObject::tr("Thighs Blend Amount") }; }
	inline static AgxTerm TargetBoneName() { return { "Target Bone Name", QObject::tr("Target Bone Name") }; }
	inline static AgxTerm TransitionType() { return { "Transition Type", QObject::tr("Transition Type") }; }
	inline static AgxTerm Tags() { return { "Tags", QObject::tr("Tags") }; }
	inline static AgxTerm Threshold() { return { "Threshold", QObject::tr("Threshold") }; }
	inline static AgxTerm TurnAnimation() { return { "Turn Animation", QObject::tr("Turn Animation") }; }
	inline static AgxTerm TurnAnimationModifyTranslation() { return { "Turn Animation Modify Translation", QObject::tr("Turn Animation Modify Translation") }; }
	
	inline static AgxTerm UpdateRefCountWhenInactive() { return { "Update Ref Count When Inactive", QObject::tr("Update Ref Count When Inactive") }; }
	inline static AgxTerm UseRadians() { return { "Use Radians", QObject::tr("Use Radians") }; }
	inline static AgxTerm UpdateWhileInactive() { return { "Update While Inactive", QObject::tr("Update While Inactive") }; }
	inline static AgxTerm Unknown1() { return { "Unknown 1", QObject::tr("Unknown 1") }; }
	inline static AgxTerm Unknown2() { return { "Unknown 2", QObject::tr("Unknown 2") }; }
	inline static AgxTerm Unknown3() { return { "Unknown 3", QObject::tr("Unknown 3") }; }
	
	inline static AgxTerm VariableControl() { return { "Variable Control", QObject::tr("Variable Control") }; }
	inline static AgxTerm VariableName() { return { "Variable Name", QObject::tr("Variable Name") }; }
	inline static AgxTerm VariableType() { return { "Variable Type", QObject::tr("Variable Type") }; }
	inline static AgxTerm Variable() { return { "Variable", QObject::tr("Variable") }; }
	inline static AgxTerm VictimSuffix() { return { "Victim Suffix", QObject::tr("Victim Suffix") }; }
	inline static AgxTerm Value() { return { "Value", QObject::tr("Value") }; }
	inline static AgxTerm Variable1() { return { "Variable 1", QObject::tr("Variable 1") }; }
	inline static AgxTerm Variable2() { return { "Variable 2", QObject::tr("Variable 2") }; }
	inline static AgxTerm Variable3() { return { "Variable 3", QObject::tr("Variable 3") }; }
	inline static AgxTerm VariableOut() { return { "Variable Out", QObject::tr("Variable Out") }; }

	inline static AgxTerm WrapToRange() { return { "Wrap To Range", QObject::tr("Wrap To Range") }; }
	inline static AgxTerm Weight() { return { "Weight", QObject::tr("Weight") }; }
	inline static AgxTerm WrappedGraph() { return { "Wrapped Graph", QObject::tr("Wrapped Graph") }; }
	inline static AgxTerm WeightorVariable() { return { "Weight or Variable", QObject::tr("Weight or Variable") }; }
	inline static AgxTerm WalkRunSpeed() { return { "Walk Run Speed", QObject::tr("Walk Run Speed") }; }
	inline static AgxTerm WalkNormalized() { return { "Walk Normalized", QObject::tr("Walk Normalized") }; }
	inline static AgxTerm WhileMoving() { return { "While Moving", QObject::tr("While Moving") }; }
	inline static AgxTerm Walk() { return { "Walk", QObject::tr("Walk") }; }

	inline static AgxTerm XYTransLockTolerance() { return { "X, Y, Trans Lock Tolerance", QObject::tr("X, Y, Trans Lock Tolerance") }; }
	inline static AgxTerm X() { return { "X", QObject::tr("X") }; }
	
	inline static AgxTerm Y() { return { "Y", QObject::tr("Y") }; }

	inline static AgxTerm Z() { return { "Z", QObject::tr("Z") }; }
	inline static AgxTerm ZerotoOne() { return { "Zero to One", QObject::tr("Zero to One") }; }







};