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

	static AgxTerm ErrorTerm() { return { "Error", QObject::tr("Error") }; }
	static AgxTerm BlankEntry() { return { "_entry_", QObject::tr("_entry_") }; }
	static AgxTerm BlankEntryE() { return { "~entry~", QObject::tr("~entry~") }; }
	static AgxTerm UNKNOWN() { return { "UNKNOWN", QObject::tr("Unknown") }; }

	static AgxTerm UserId() { return { "user_id", QObject::tr("User Id") }; }
	static AgxTerm NodeType() { return { "node_type", QObject::tr("Node Type") }; }
	static AgxTerm UseColor() { return { "use_color_2", QObject::tr("Use Color") }; }
	static AgxTerm PosX() { return { "pos_x", QObject::tr("Pos X") }; }
	static AgxTerm PosY() { return { "pos_y", QObject::tr("Pos Y") }; }
	static AgxTerm ExpPosX() { return { "expanded_pos_x", QObject::tr("Expanded Pos X") }; }
	static AgxTerm ExpPosY() { return { "expanded_pos_y", QObject::tr("Expanded Pos Y") }; }
	static AgxTerm noninstanced() { return { "noninstanced", QObject::tr("Non Instanced") }; }
	static AgxTerm DefaultState() { return { "default_state", QObject::tr("Default State") }; }

	static AgxTerm Flags() { return { "Flags", QObject::tr("Flags") }; }

	static AgxTerm x_StandtoMoveTags_x() { return {"Stand to Move Tags", QObject::tr("Tag")}; }
	static AgxTerm x_MovetoStopTags_x() { return {"Move to Stop Tags", QObject::tr("Tag")}; }
	static AgxTerm x_WhileMovingTags_x() { return {"While Moving Tags", QObject::tr("Tag")}; }
	
	static AgxTerm x_SwapEvents_x() { return {"Swap Graph Events", QObject::tr("Event")}; }
	static AgxTerm x_EnterEvents_x() { return {"Enter Events", QObject::tr("Event")}; }
	static AgxTerm x_ExitEvents_x() { return {"Exit Events", QObject::tr("Event")}; }
	static AgxTerm EnterEvents() { return { "Enter Events", QObject::tr("Enter Events") }; }
	static AgxTerm ExitEvents() { return { "Exit Events", QObject::tr("Exit Events") }; }

	static AgxTerm Ankle() { return { "Ankle", QObject::tr("Ankle") }; }
	static AgxTerm AssignedValue() { return { "Assigned Value", QObject::tr("Assigned Value") }; }
	static AgxTerm AnimTag() { return { "Anim Tag", QObject::tr("Anim Tag") }; }
	static AgxTerm ActivateVariable() { return { "Activate Variable", QObject::tr("Activate Variable") }; }
	static AgxTerm ActivateBaseEventList() { return { "Activate Base Event List", QObject::tr("Activate Base Event List") }; }
	static AgxTerm AimVectorVariable() { return { "Aim Vector Variable", QObject::tr("Aim Vector Variable") }; }
	static AgxTerm AimHeadingOffsetOutput() { return { "Aim Heading Offset Output", QObject::tr("Aim Heading Offset Output") }; }
	static AgxTerm AimPitchOffsetOutput() { return { "Aim Pitch Offset Output", QObject::tr("Aim Pitch Offset Output") }; }
	static AgxTerm AllowNoEffect() { return { "Allow No Effect", QObject::tr("Allow No Effect") }; }
	static AgxTerm AnkleLBoneName() { return { "Ankle L Bone Name", QObject::tr("Ankle L Bone Name") }; }
	static AgxTerm AnkleRBoneName() { return { "Ankle R Bone Name", QObject::tr("Ankle R Bone Name") }; }
	static AgxTerm AngleLMultiplier() { return { "Angle L Multiplier", QObject::tr("Angle L Multiplier") }; }
	static AgxTerm AngleRMultiplier() { return { "Angle R Multiplier", QObject::tr("Angle R Multiplier") }; }
	static AgxTerm Axis() { return { "Axis", QObject::tr("Axis") }; }
	static AgxTerm AngleSpring() { return { "Angle Spring", QObject::tr("Angle Spring") }; }
	static AgxTerm AlignOrient() { return { "Align Orient", QObject::tr("Align Orient") }; }
	static AgxTerm AlignOrientPercent() { return { "Align Orient Percent", QObject::tr("Align Orient Percent") }; }
	static AgxTerm ActiveBlendRate() { return { "Active Blend Rate", QObject::tr("Active Blend Rate") }; }
	static AgxTerm AlignmentBone() { return { "Alignment Bone", QObject::tr("Alignment Bone") }; }
	static AgxTerm AngleMultiplier() { return { "Angle Multiplier", QObject::tr("Angle Multiplier") }; }
	static AgxTerm AllowSelfTransition() { return { "Allow Self Transition", QObject::tr("Allow Self Transition") }; }
	static AgxTerm Angle0to1() { return { "Angle 0-1", QObject::tr("Angle 0-1") }; }
	static AgxTerm AnkleName() { return { "Ankle Name", QObject::tr("Ankle Name") }; }
	static AgxTerm AngMult() { return { "Ang Mult", QObject::tr("Angle Multiplier") }; }
	static AgxTerm AssignValue() { return { "Assign Value", QObject::tr("Assign Value") }; }
	static AgxTerm Assign() { return { "Assign", QObject::tr("Assign") }; }

	static AgxTerm BlendRate() { return { "Blend Rate", QObject::tr("Blend Rate") }; }
	static AgxTerm BlendForces() { return { "Blend Forces", QObject::tr("Blend Forces") }; }
	static AgxTerm BoneName() { return { "Bone Name", QObject::tr("Bone Name") }; }
	static AgxTerm BendAxis() { return { "Bend Axis", QObject::tr("Bend Axis") }; }
	static AgxTerm BodyPartVar() { return { "Body Part Var", QObject::tr("Body Part Var") }; }
	static AgxTerm BlendTime() { return { "Blend Time", QObject::tr("Blend Time") }; }
	static AgxTerm BlendModeFunction() { return { "Blend Mode Function", QObject::tr("Blend Mode Function") }; }
	static AgxTerm BlendOutFrames() { return { "Blend Out Frames", QObject::tr("Blend Out Frames") }; }
	static AgxTerm BlendOutTime() { return { "Blend Out Time", QObject::tr("Blend Out Time") }; }
	static AgxTerm BlendTreeName() { return { "Blend Tree Name", QObject::tr("Blend Tree Name") }; }
	static AgxTerm BlendOnOfftime() { return { "Blend On/Off time", QObject::tr("Blend On/Off time") }; }
	static AgxTerm BlendFromRagdollTime() { return { "Blend From Ragdoll Time", QObject::tr("Blend From Ragdoll Time") }; }
	static AgxTerm Blendratewhenactive() { return { "Blend rate when active", QObject::tr("Blend rate when active") }; }
	static AgxTerm Blendratewheninactive() { return { "Blend rate when inactive", QObject::tr("Blend rate when inactive") }; }
	static AgxTerm BendBoneName() { return { "Bend Bone Name", QObject::tr("Bend Bone Name") }; }
	static AgxTerm BlendwithNext() { return { "Blend with Next", QObject::tr("Blend With Next") }; }
	static AgxTerm BlendNextMult() { return { "Blend Next Mult", QObject::tr("Blend Next Mult") }; }
	static AgxTerm BodyPart() { return { "Body Part", QObject::tr("Body Part") }; }
	static AgxTerm BackTagName() { return { "Back Tag Name", QObject::tr("Back Tag Name") }; }
	static AgxTerm BoneVariance() { return { "Bone Variance", QObject::tr("Bone Variance") }; }
	static AgxTerm BindingType() { return { "Binding Type", QObject::tr("Binding Type") }; }
	static AgxTerm BindingName() { return { "Binding Name", QObject::tr("Binding Name") }; }
	static AgxTerm BindingValue() { return { "Binding Value", QObject::tr("Binding Value") }; }
	static AgxTerm BoneNames() { return { "Bone Names", QObject::tr("Bone Names") }; }
	static AgxTerm Back() { return { "Back", QObject::tr("Back") }; }
	static AgxTerm Bones() { return { "Bones", QObject::tr("Bones") }; }
	static AgxTerm Bindings() { return { "Bindings", QObject::tr("Bindings") }; }
	static AgxTerm BindingVariables() { return { "Binding Variables", QObject::tr("Binding Variables") }; }

	static AgxTerm CoreBodyBoneName() { return { "Core Body Bone Name", QObject::tr("Core Body Bone Name") }; }
	static AgxTerm ChestAxisIndex() { return { "Chest Axis Index", QObject::tr("Chest Axis Index") }; }
	static AgxTerm ChestLocationBone() { return { "Chest Location Bone", QObject::tr("Chest Location Bone") }; }
	static AgxTerm ChestFullBlendMaxAngle() { return { "Chest Full Blend Max Angle", QObject::tr("Chest Full Blend Max Angle") }; }
	static AgxTerm ClampHeadingMin() { return { "Clamp Heading Min", QObject::tr("Clamp Heading Min") }; }
	static AgxTerm ClampHeadingMax() { return { "Clamp Heading Max", QObject::tr("Clamp Heading Max") }; }
	static AgxTerm ClampPitchMin() { return { "Clamp Pitch Min", QObject::tr("Clamp Pitch Min") }; }
	static AgxTerm ClampPitchMax() { return { "Clamp Pitch Max", QObject::tr("Clamp Pitch Max") }; }
	static AgxTerm CleanUpOnly() { return { "Clean Up Only", QObject::tr("Clean Up Only") }; }
	static AgxTerm ControlVariable() { return { "Control Variable", QObject::tr("Control Variable") }; }
	static AgxTerm CropStartFrames() { return { "Crop Start Frames", QObject::tr("Crop Start Frames") }; }
	static AgxTerm CropEndFrames() { return { "Crop End Frames", QObject::tr("Crop End Frames") }; }
	static AgxTerm CurrentCumulativeCount() { return { "Current Cumulative Count", QObject::tr("Current Cumulative Count") }; }
	static AgxTerm CastDistanceUp() { return { "Cast Distance Up", QObject::tr("Cast Distance Up") }; }
	static AgxTerm CastDistanceDown() { return { "Cast Distance Down", QObject::tr("Cast Distance Down") }; }
	static AgxTerm CastDistanceUpVariable() { return { "Cast Distance Up Variable", QObject::tr("Cast Distance Up Variable") }; }
	static AgxTerm CastDistanceDownVariable() { return { "Cast Distance Down Variable", QObject::tr("Cast Distance Down Variable") }; }
	static AgxTerm CurrentSpeed() { return { "Current Speed", QObject::tr("Current Speed") }; }
	static AgxTerm CurrentDirection() { return { "Current Direction", QObject::tr("Current Direction") }; }
	static AgxTerm ContactNormalBlendRate() { return { "Contact Normal Blend Rate", QObject::tr("Contact Normal Blend Rate") }; }
	static AgxTerm ChancetoRepick() { return { "Chance to Repick", QObject::tr("Chance to Repick") }; }
	static AgxTerm Crossblendtime() { return { "Cross blend time", QObject::tr("Cross blend time") }; }
	static AgxTerm COMBlendAmount() { return { "COM Blend Amount", QObject::tr("COM Blend Amount") }; }
	static AgxTerm ClampedBlendTime() { return { "Clamped Blend Time", QObject::tr("Clamped Blend Time") }; }
	static AgxTerm CounterAttackEvent() { return { "Counter Attack Event", QObject::tr("Counter Attack Event") }; }
	static AgxTerm CounterAttackThreshold() { return { "Counter Attack Threshold", QObject::tr("Counter Attack Threshold") }; }
	static AgxTerm CounterAttackChance() { return { "Counter Attack Chance", QObject::tr("Counter Attack Chance") }; }
	static AgxTerm CounterAttackCooldown() { return { "Counter Attack Cooldown", QObject::tr("Counter Attack Cooldown") }; }
	static AgxTerm Comment() { return { "Comment", QObject::tr("Comment") }; }
	static AgxTerm Condition() { return { "Condition", QObject::tr("Condition") }; }
	static AgxTerm CHeadingMin() { return { "C Heading Min", QObject::tr("C Heading Min") }; }
	static AgxTerm CHeadingMax() { return { "C Heading Max", QObject::tr("C Heading Max") }; }
	static AgxTerm CPitchMin() { return { "C Pitch Min", QObject::tr("C Pitch Min") }; }
	static AgxTerm CPitchMax() { return { "C Pitch Max", QObject::tr("C Pitch Max") }; }
	static AgxTerm CancelEvent() { return { "Cancel Event", QObject::tr("Cancel Event") }; }
	static AgxTerm ComHghtMin() { return { "Com Hght Min", QObject::tr("COM Height Min") }; }
	static AgxTerm CHip() { return { "CHip", QObject::tr("CHip") }; }
	static AgxTerm CKnee() { return { "CKnee", QObject::tr("CKnee") }; }
	static AgxTerm CAnkle() { return { "CAnkle", QObject::tr("CAnkle") }; }

	static AgxTerm Decrement() { return { "Decrement", QObject::tr("Decrement") }; }
	static AgxTerm DoNothing() { return { "Do Nothing", QObject::tr("Do Nothing") }; }
	static AgxTerm Dampen() { return { "Dampen", QObject::tr("Dampen") }; }
	static AgxTerm Def_() { return { "Def.", QObject::tr("Definition") }; }
	static AgxTerm Default() { return { "Default", QObject::tr("Default") }; }
	static AgxTerm DoNotFollowSyncGroup() { return { "Do Not Follow Sync Group", QObject::tr("Do Not Follow Sync Group") }; }
	static AgxTerm DesiredSpeedVar() { return { "Desired Speed Var", QObject::tr("Desired Speed Var") }; }
	static AgxTerm DisableWhenInactive() { return { "Disable When Inactive", QObject::tr("Disable When Inactive") }; }
	static AgxTerm DisableBefore() { return { "Disable Before", QObject::tr("Disable Before") }; }
	static AgxTerm DisableAfter() { return { "Disable After", QObject::tr("Disable After") }; }
	static AgxTerm DeadZone() { return { "Dead Zone", QObject::tr("Dead Zone") }; }
	static AgxTerm DesiredDirection() { return { "Desired Direction", QObject::tr("Desired Direction") }; }
	static AgxTerm DesiredSpeed() { return { "Desired Speed", QObject::tr("Desired Speed") }; }
	static AgxTerm DirectionVar() { return { "Direction Var", QObject::tr("Direction Var") }; }
	static AgxTerm Direction() { return { "Direction", QObject::tr("Direction") }; }
	static AgxTerm Directions() { return { "Directions", QObject::tr("Directions") }; }
	static AgxTerm DirectionIsRadians() { return { "Direction Is Radians", QObject::tr("Direction Is Radians") }; }
	static AgxTerm DampenOverride() { return { "Dampen Override", QObject::tr("Dampen Override") }; }
	static AgxTerm DirectAtVectorVariable() { return { "DirectAtVector Variable", QObject::tr("DirectAtVector Variable") }; }
	static AgxTerm DisableVariable() { return { "Disable Variable", QObject::tr("Disable Variable") }; }
	static AgxTerm DisableHeadingMin() { return { "Disable Heading Min", QObject::tr("Disable Heading Min") }; }
	static AgxTerm DisableHeadingMax() { return { "Disable Heading Max", QObject::tr("Disable Heading Max") }; }
	static AgxTerm DisableHeadingHysteresis() { return { "Disable Heading Hysteresis", QObject::tr("Disable Heading Hysteresis") }; }
	static AgxTerm DiscardEvents() { return { "Discard Events", QObject::tr("Discard Events") }; }
	static AgxTerm DownErrorBias() { return { "Down Error Bias", QObject::tr("Down Error Bias") }; }
	static AgxTerm DesiredValue() { return { "Desired Value", QObject::tr("Desired Value") }; }
	static AgxTerm DisableLimits() { return { "Disable Limits", QObject::tr("Disable Limits") }; }
	static AgxTerm DisableCustomBonesWhenBlendingOut() { return { "Disable Custom Bones When Blending Out", QObject::tr("Disable Custom Bones When Blending Out") }; }
	static AgxTerm DisableEvents() { return { "Disable Events", QObject::tr("Disable Events") }; }

	static AgxTerm Event() { return { "Event", QObject::tr("Event") }; }
	static AgxTerm EventandVariable() { return { "Event and Variable", QObject::tr("Event and Variable") }; }
	static AgxTerm EndOrient() { return { "End Orient", QObject::tr("End Orient") }; }
	static AgxTerm Entries() { return { "Entries", QObject::tr("Entries") }; }
	static AgxTerm EvaluatedCondition() { return { "Evaluated Condition", QObject::tr("Evaluated Condition") }; }
	static AgxTerm ExtraName() { return { "Extra Name", QObject::tr("Extra Name") }; }
	static AgxTerm EnableEvents() { return { "Enable Events", QObject::tr("Enable Events") }; }
	static AgxTerm EventList() { return { "Event List", QObject::tr("Event List") }; }
	static AgxTerm EventName() { return { "Event Name", QObject::tr("Event Name") }; }
	static AgxTerm EndValue() { return { "End Value", QObject::tr("End Value") }; }
	static AgxTerm EnforceDirectAtLimits() { return { "Enforce DirectAt Limits", QObject::tr("Enforce DirectAt Limits") }; }
	static AgxTerm EnumerateTag() { return { "Enumerate Tag", QObject::tr("Enumerate Tag") }; }
	static AgxTerm EventSentAfterMaxIterations() { return { "Event Sent After Max Iterations", QObject::tr("Event Sent After Max Iterations") }; }
	static AgxTerm EventtoResetAllCumulativeAnimations() { return { "Event to Reset All Cumulative Animations", QObject::tr("Event to Reset All Cumulative Animations") }; }
	static AgxTerm EHip() { return { "EHip", QObject::tr("EHip") }; }
	static AgxTerm EKnee() { return { "EKnee", QObject::tr("EKnee") }; }
	static AgxTerm EAnkle() { return { "EAnkle", QObject::tr("EAnkle") }; }
	static AgxTerm EndPosition() { return { "End Position", QObject::tr("End Position") }; }

	static AgxTerm FinalBoneAxisIndex() { return { "Final Bone Axis Index", QObject::tr("Final Bone Axis Index") }; }
	static AgxTerm FootUnlockBlendScalar() { return { "Foot Unlock Blend Scalar", QObject::tr("Foot Unlock Blend Scalar") }; }
	static AgxTerm ForwardTagName() { return { "Forward Tag Name", QObject::tr("Forward Tag Name") }; }
	static AgxTerm Forward() { return { "Forward", QObject::tr("Forward") }; }

	static AgxTerm Graphs() { return { "Graphs", QObject::tr("Graphs") }; }
	static AgxTerm GraphRef() { return { "Graph Ref", QObject::tr("Graph Ref") }; }
	static AgxTerm GraphName() { return { "Graph Name", QObject::tr("Graph Name") }; }
	static AgxTerm GetUpEvent() { return { "Get Up Event", QObject::tr("Get Up Event") }; }
	static AgxTerm GetUpStartEvent() { return { "Get Up Start Event", QObject::tr("Get Up Start Event") }; }
	static AgxTerm GoDynamicOnContact() { return { "Go Dynamic On Contact", QObject::tr("Go Dynamic On Contact") }; }

	static AgxTerm HeadBoneName() { return { "Head Bone Name", QObject::tr("Head Bone Name") }; }
	static AgxTerm HeadingDeltaVariable() { return { "Heading Delta Variable", QObject::tr("Heading Delta Variable") }; }
	static AgxTerm HeadingOffsetOutput() { return { "Heading Offset Output", QObject::tr("Heading Offset Output") }; }
	static AgxTerm HeadingLagDegrees() { return { "Heading Lag Degrees", QObject::tr("Heading Lag Degrees") }; }
	static AgxTerm HeadLocationBone() { return { "Head Location Bone", QObject::tr("Head Location Bone") }; }
	static AgxTerm HeadLagDegrees() { return { "Head Lag Degrees", QObject::tr("Head Lag Degrees") }; }
	static AgxTerm HeadBlendRate() { return { "Head Blend Rate", QObject::tr("Head Blend Rate") }; }
	static AgxTerm HasLoopingSegment() { return { "Has Looping Segment", QObject::tr("Has Looping Segment") }; }
	static AgxTerm HipLBoneName() { return { "Hip L Bone Name", QObject::tr("Hip L Bone Name") }; }
	static AgxTerm HipRBoneName() { return { "Hip R Bone Name", QObject::tr("Hip R Bone Name") }; }
	static AgxTerm HipsBlendAmount() { return { "Hips Blend Amount", QObject::tr("Hips Blend Amount") }; }
	static AgxTerm HipName() { return { "Hip Name", QObject::tr("Hip Name") }; }
	static AgxTerm Hip() { return { "Hip", QObject::tr("Hip") }; }
	static AgxTerm Headings() { return { "Headings", QObject::tr("Headings") }; }

	static AgxTerm IsActiveBool() { return { "Is Active Bool", QObject::tr("Is Active Bool") }; }
	static AgxTerm ImpactVar() { return { "Impact Var", QObject::tr("Impact Var") }; }
	static AgxTerm InertiaSpring() { return { "Inertia Spring", QObject::tr("Inertia Spring") }; }
	static AgxTerm IgnoreContactBones() { return { "Ignore Contact Bones", QObject::tr("Ignore Contact Bones") }; }
	static AgxTerm iStateVariable() { return{ "iState Variable", QObject::tr("iState Variable") }; }
	static AgxTerm InputLockTime() { return { "Input Lock Time", QObject::tr("Input Lock Time") }; }
	static AgxTerm IsActiveVar() { return { "Is Active Var", QObject::tr("Is Active Var") }; }
	static AgxTerm IsHeadTrackingActiveVariable() { return { "Is Head Tracking Active Variable", QObject::tr("Is Head Tracking Active Variable") }; }
	static AgxTerm IsAimActiveVariable() { return { "Is Aim Active Variable", QObject::tr("Is Aim Active Variable") }; }
	static AgxTerm IsActionAnimation() { return { "Is Action Animation", QObject::tr("Is Action Animation") }; }
	static AgxTerm InitializeSequenceOnLoad() { return { "Initialize Sequence On Load", QObject::tr("Initialize Sequence On Load") }; }
	static AgxTerm InactiveBlendRate() { return { "Inactive Blend Rate", QObject::tr("Inactive Blend Rate") }; }
	static AgxTerm IsUsingCodeDrivenRotation() { return { "Is Using Code Driven Rotation", QObject::tr("Is Using Code Driven Rotation") }; }
	static AgxTerm ImmunityThreshold() { return { "Immunity Threshold", QObject::tr("Immunity Threshold") }; }
	static AgxTerm ImmunityDelayBeforeDecay() { return { "Immunity Delay Before Decay", QObject::tr("Immunity Delay Before Decay") }; }
	static AgxTerm ImmunityDecayRate() { return { "Immunity Decay Rate", QObject::tr("Immunity Decay Rate") }; }
	static AgxTerm ImmunityConsumeRate() { return { "Immunity Consume Rate", QObject::tr("Immunity Consume Rate") }; }
	static AgxTerm Impact() { return { "Impact", QObject::tr("Impact") }; }
	static AgxTerm iStateName() { return { "iState Name", QObject::tr("iState Name") }; }
	static AgxTerm iStateValue() { return { "iState Value", QObject::tr("iState Value") }; }
	static AgxTerm iStateAssignments() { return { "iState Assignments", QObject::tr("iState Assignments") }; }
	static AgxTerm Increment() { return { "Increment", QObject::tr("Increment") }; }

	static AgxTerm Jog() { return { "Jog", QObject::tr("Jog") }; }

	static AgxTerm KneeLBoneName() { return { "Knee L Bone Name", QObject::tr("Knee L Bone Name") }; }
	static AgxTerm KneeRBoneName() { return { "Knee R Bone Name", QObject::tr("Knee R Bone Name") }; }
	static AgxTerm KneeName() { return { "Knee Name", QObject::tr("Knee Name") }; }
	static AgxTerm Knee() { return { "Knee", QObject::tr("Knee") }; }

	static AgxTerm LimitToSubgraph() { return { "Limit To Subgraph", QObject::tr("Limit To Subgraph") }; }
	static AgxTerm LagDegrees() { return { "Lag Degrees", QObject::tr("Lag Degrees") }; }
	static AgxTerm LeadSuffix() { return { "Lead Suffix", QObject::tr("Lead Suffix") }; }
	static AgxTerm LookVectorVariable() { return { "Look Vector Variable", QObject::tr("Look Vector Variable") }; }
	static AgxTerm LogicVariable() { return { "Logic Variable", QObject::tr("Logic Variable") }; }
	static AgxTerm ListenEvent() { return { "Listen Event", QObject::tr("Listen Event") }; }
	static AgxTerm LeftFootLockOverride() { return { "Left Foot Lock Override", QObject::tr("Left Foot Lock Override") }; }
	static AgxTerm LeftFootUnlockOverride() { return { "Left Foot Unlock Override", QObject::tr("Left Foot Unlock Override") }; }
	static AgxTerm RightFootLockOverride() { return { "Right Foot Lock Override", QObject::tr("Right Foot Lock Override") }; }
	static AgxTerm RightFootUnlockOverride() { return { "Right Foot Unlock Override", QObject::tr("Right Foot Unlock Override") }; }
	static AgxTerm LeftHandBoneName() { return { "Left Hand Bone Name", QObject::tr("Left Hand Bone Name") }; }
	static AgxTerm LeftLegBoneName() { return { "Left Leg Bone Name", QObject::tr("Left Leg Bone Name") }; }
	static AgxTerm LeftTagName() { return { "Left Tag Name", QObject::tr("Left Tag Name") }; }
	static AgxTerm LegsAExtra() { return { "LegsA Extra", QObject::tr("LegsA Extra") }; }
	static AgxTerm LegsBExtra() { return { "LegsB Extra", QObject::tr("LegsB Extra") }; }
	static AgxTerm LegsCExtra() { return { "LegsC Extra", QObject::tr("LegsC Extra") }; }
	static AgxTerm LegsDExtra() { return { "LegsD Extra", QObject::tr("LegsD Extra") }; }
	static AgxTerm LegsEExtra() { return { "LegsE Extra", QObject::tr("LegsE Extra") }; }
	static AgxTerm LegsFExtra() { return { "LegsF Extra", QObject::tr("LegsF Extra") }; }
	static AgxTerm Left() { return { "Left", QObject::tr("Left") }; }
	static AgxTerm Linear() { return { "Linear", QObject::tr("Linear") }; }
	static AgxTerm Layer() { return { "Layer", QObject::tr("Layer") }; }
	static AgxTerm Large() { return { "Large", QObject::tr("Large") }; }
	static AgxTerm LoopingLeft() { return { "Looping Left", QObject::tr("Looping Left") }; }
	static AgxTerm LoopingRight() { return { "Looping Right", QObject::tr("Looping Right") }; }
	static AgxTerm LongestDirection() { return { "Longest Direction", QObject::tr("Longest Direction") }; }

	static AgxTerm Max() { return { "Max", QObject::tr("Max") }; }
	static AgxTerm Medium() { return { "Medium", QObject::tr("Medium") }; }
	static AgxTerm MoveToStop() { return { "Move To Stop", QObject::tr("Move To Stop") }; }
	static AgxTerm Mirror() { return { "Mirror", QObject::tr("Mirror") }; }
	static AgxTerm MinScale() { return { "Min Scale", QObject::tr("Min Scale") }; }
	static AgxTerm MaxScale() { return { "Max Scale", QObject::tr("Max Scale") }; }
	static AgxTerm MaxCorrectionVel() { return { "Max Correction Vel", QObject::tr("Max Correction Vel") }; }
	static AgxTerm MaxPostCorrectionVel() { return { "Max Post Correction Vel", QObject::tr("Max Post Correction Vel") }; }
	static AgxTerm MaxCorrectionSpeed() { return { "Max Correction Speed", QObject::tr("Max Correction Speed") }; }
	static AgxTerm Mass() { return { "Mass", QObject::tr("Mass") }; }
	static AgxTerm MassOverride() { return { "Mass Override", QObject::tr("Mass Override") }; }
	static AgxTerm MinTimes() { return { "Min Times", QObject::tr("Min Times") }; }
	static AgxTerm Mintime() { return { "Min time", QObject::tr("Min Time") }; }
	static AgxTerm MinTime() { return { "Min Time", QObject::tr("Min Time") }; }
	static AgxTerm Maxtime() { return { "Max time", QObject::tr("Max Time") }; }
	static AgxTerm MaxTime() { return { "Max Time", QObject::tr("Max Time") }; }
	static AgxTerm MaxTimes() { return { "Max Times", QObject::tr("Max Times") }; }
	static AgxTerm MaxBlendTime() { return { "Max Blend Time", QObject::tr("Max Blend Time") }; }
	static AgxTerm MaxCumulativeAnimation() { return { "Max Cumulative Animation", QObject::tr("Max Cumulative Animation") }; }
	static AgxTerm MinBendAngle() { return { "Min Bend Angle", QObject::tr("Min Bend Angle") }; }
	static AgxTerm MaxBendAngle() { return { "Max Bend Angle", QObject::tr("Max Bend Angle") }; }
	static AgxTerm MaxIKUp() { return { "Max IK Up", QObject::tr("Max IK Up") }; }
	static AgxTerm MaxIKUpVariable() { return { "Max IK Up Variable", QObject::tr("Max IK Up Variable") }; }
	static AgxTerm MaxStrideLength() { return { "Max Stride Length", QObject::tr("Max Stride Length") }; }
	static AgxTerm MaxPostCorrectionSpeed() { return { "Max Post Correction Speed", QObject::tr("Max Post Correction Speed") }; }
	static AgxTerm MaxControllerSeperation() { return { "Max Controller Seperation", QObject::tr("Max Controller Seperation") }; }
	static AgxTerm MinimumActiveTime() { return { "Minimum Active Time", QObject::tr("Minimum Active Time") }; }
	static AgxTerm MoveStartCorrectionTime() { return { "Move Start Correction Time", QObject::tr("Move Start Correction Time") }; }
	static AgxTerm MinX() { return { "Min X", QObject::tr("Min X") }; }
	static AgxTerm MaxX() { return { "Max X", QObject::tr("Max X") }; }
	static AgxTerm MinY() { return { "Min Y", QObject::tr("Min Y") }; }
	static AgxTerm MaxY() { return { "Max Y", QObject::tr("Max Y") }; }
	static AgxTerm MinZ() { return { "Min Z", QObject::tr("Min Z") }; }
	static AgxTerm MaxZ() { return { "Max Z", QObject::tr("Max Z") }; }
	static AgxTerm MaxSpeedMult() { return { "Max Speed Mult", QObject::tr("Max Speed Mult") }; }
	static AgxTerm MaintainSyncSelectionVariableWhenExiting() { return { "Maintain Sync Selection Variable When Exiting", QObject::tr("Maintain Sync Selection Variable When Exiting") }; }

	static AgxTerm Name() { return { "Name", QObject::tr("Name") }; }
	static AgxTerm None() { return { "None", QObject::tr("None") }; }
	static AgxTerm NodeID() { return { "Node ID", QObject::tr("Node ID") }; }
	static AgxTerm NonInstanced() { return { "Non Instanced", QObject::tr("Non Instanced") }; }
	static AgxTerm NormalizeLength() { return { "Normalize Length", QObject::tr("Normalize Length") }; }
	static AgxTerm NormalizeWeights() { return { "Normalize Weights", QObject::tr("Normalize Weights") }; }
	static AgxTerm NoHeadTrackVariable() { return { "No Head Track Variable", QObject::tr("No Head Track Variable") }; }
	static AgxTerm NoAimVariable() { return { "No Aim Variable", QObject::tr("No Aim Variable") }; }
	static AgxTerm NegateVariable() { return { "Negate Variable", QObject::tr("Negate Variable") }; }
	static AgxTerm NPCHeadingOffset_Var() { return { "NPCHeadingOffset Var", QObject::tr("NPC Heading Offset Var") }; }

	static AgxTerm OnlyAssignOnce() { return { "Only Assign Once", QObject::tr("Only Assign Once") }; }
	static AgxTerm OnlyHighestWeightActive() { return { "Only Highest Weight Active", QObject::tr("Only Highest Weight Active") }; }
	static AgxTerm Operation() { return { "Operation", QObject::tr("Operation") }; }
	static AgxTerm OrientVar() { return { "Orient Var", QObject::tr("Orient Var") }; }
	static AgxTerm OutputVariable() { return { "Output Variable", QObject::tr("Output Variable") }; }
	static AgxTerm OverrideAnimDirection() { return { "Override Anim Direction", QObject::tr("Override Anim Direction") }; }
	static AgxTerm OrientedOffset() { return { "Oriented Offset", QObject::tr("Oriented Offset") }; }
	static AgxTerm Output() { return { "Output", QObject::tr("Output") }; }
	static AgxTerm Orientation() { return { "Orientation", QObject::tr("Orientation") }; }
	static AgxTerm OneMinusWeight() { return { "One Minus Weight", QObject::tr("One Minus Weight") }; }

	static AgxTerm ProcessInactiveEvents() { return { "Process Inactive Events", QObject::tr("Process Inactive Events") }; }
	static AgxTerm PitchOffsetOutput() { return { "Pitch Offset Output", QObject::tr("Pitch Offset Output") }; }
	//This shows up in two forms, might be a typo in the BGS source code...
	static AgxTerm Position_ChangeEvent() { return { "Position Change Event", QObject::tr("Position Change Event") }; }
	static AgxTerm PositionChangeEvent() { return { "PositionChange Event", QObject::tr("Position Change Event") }; }
	static AgxTerm PlaybackSpeed() { return { "Playback Speed", QObject::tr("Playback Speed") }; }
	static AgxTerm PositionVar() { return { "Position Var", QObject::tr("Position Var") }; }
	static AgxTerm ParentBoneName() { return { "Parent Bone Name", QObject::tr("Parent Bone Name") }; }
	static AgxTerm Priority() { return { "Priority", QObject::tr("Priority") }; }
	static AgxTerm Payload() { return { "Payload", QObject::tr("Payload") }; }
	static AgxTerm Prefix() { return { "Prefix", QObject::tr("Prefix") }; }
	static AgxTerm Param() { return { "Param", QObject::tr("Parameter") }; }
	static AgxTerm Parametric() { return { "Parametric", QObject::tr("Parametric") }; }
	static AgxTerm PickWeight() { return { "Pick Weight", QObject::tr("Pick Weight") }; }
	static AgxTerm PickWeights() { return { "Pick Weights", QObject::tr("Pick Weights") }; }
	static AgxTerm PropagateTagUpstream() { return { "Propagate Tag Upstream", QObject::tr("Propagate Tag Upstream") }; }
	static AgxTerm PairedAnimNameRoot() { return { "Paired Anim Name Root", QObject::tr("Paired Anim Name Root") }; }

	static AgxTerm RandomizeStartPoint() { return { "Randomize Start Point", QObject::tr("Randomize Start Point") }; }
	static AgxTerm RangeMin() { return { "Range Min", QObject::tr("Range Min") }; }
	static AgxTerm RangeMax() { return { "Range Max", QObject::tr("Range Max") }; }
	static AgxTerm ResumeEvent() { return { "Resume Event", QObject::tr("Resume Event") }; }
	static AgxTerm ResetAfterMaxIterations() { return { "Reset After Max Iterations", QObject::tr("Reset After Max Iterations") }; }
	static AgxTerm RotateHeadBeforeChest() { return { "Rotate Head Before Chest", QObject::tr("Rotate Head Before Chest") }; }
	static AgxTerm ReleaseEvent() { return { "Release Event", QObject::tr("Release Event") }; }
	static AgxTerm RootFeedbackWeight() { return { "Root Feedback Weight", QObject::tr("Root Feedback Weight") }; }
	static AgxTerm RootTwistBlendRate() { return { "Root Twist Blend Rate", QObject::tr("Root Twist Blend Rate") }; }
	static AgxTerm RangeMinMultiplier() { return { "Range Min Multiplier", QObject::tr("Range Min Multiplier") }; }
	static AgxTerm RangeMaxMultiplier() { return { "Range Max Multiplier", QObject::tr("Range Max Multiplier") }; }
	static AgxTerm RampInTime() { return { "Ramp In Time", QObject::tr("Ramp In Time") }; }
	static AgxTerm RunNormalized() { return { "Run Normalized", QObject::tr("Run Normalized") }; }
	static AgxTerm RightHandBoneName() { return { "Right Hand Bone Name", QObject::tr("Right Hand Bone Name") }; }
	static AgxTerm RightLegBoneName() { return { "Right Leg Bone Name", QObject::tr("Right Leg Bone Name") }; }
	static AgxTerm RepickAtEnd() { return { "Repick At End", QObject::tr("Repick At End") }; }
	static AgxTerm RootBlendAmount() { return { "Root Blend Amount", QObject::tr("Root Blend Amount") }; }
	static AgxTerm RotationType() { return { "Rotation Type", QObject::tr("Rotation Type") }; }
	static AgxTerm RandomizeIfInvalid() { return { "Randomize If Invalid", QObject::tr("Randomize If Invalid") }; }
	static AgxTerm ResetEventName() { return { "Reset Event Name", QObject::tr("Reset Event Name") }; }
	static AgxTerm RightTagName() { return { "Right Tag Name", QObject::tr("Right Tag Name") }; }
	static AgxTerm RigPath() { return { "Rig Path", QObject::tr("Rig Path") }; }
	static AgxTerm RigName() { return { "Rig Name", QObject::tr("Rig Name") }; }
	static AgxTerm Right() { return { "Right", QObject::tr("Right") }; }
	static AgxTerm Run() { return { "Run", QObject::tr("Run") }; }
	static AgxTerm ReceiveTagfromDownstream() { return { "Receive Tag from Downstream", QObject::tr("Receive Tag from Downstream") }; }

	static AgxTerm StaggerForceVariable() { return { "Stagger Force Variable", QObject::tr("Stagger Force Variable") }; }
	static AgxTerm StaggerCooldown() { return { "Stagger Cooldown", QObject::tr("Stagger Cooldown") }; }
	static AgxTerm SyncSystem() { return { "Sync System", QObject::tr("Sync System") }; }
	static AgxTerm SyncSlave() { return { "Sync Slave", QObject::tr("Sync Slave") }; }
	static AgxTerm SendEventOnEnd() { return { "Send Event On End", QObject::tr("Send Event On End") }; }
	static AgxTerm StateVariable() { return { "State Variable", QObject::tr("State Variable") }; }
	static AgxTerm StateVariableRefCount() { return { "State Variable Ref Count", QObject::tr("State Variable Ref Count") }; }
	static AgxTerm SpeedMultiplier() { return { "Speed Multiplier", QObject::tr("Speed Multiplier") }; }
	static AgxTerm SpeedMultiplierIsDuration() { return { "Speed Multiplier Is Duration", QObject::tr("Speed Multiplier Is Duration") }; }
	static AgxTerm SyncOnlyTransitionOut() { return { "Sync Only Transition Out", QObject::tr("Sync Only Transition Out") }; }
	static AgxTerm SyncVariable() { return { "Sync Variable", QObject::tr("Sync Variable") }; }
	static AgxTerm SourceLocationBone() { return { "Source Location Bone", QObject::tr("Source Location Bone") }; }
	static AgxTerm SourceBone() { return { "Source Bone", QObject::tr("Source Bone") }; }
	static AgxTerm SwapGraphEvent() { return { "Swap Graph Event", QObject::tr("Swap Graph Event") }; }
	static AgxTerm StartingGraph() { return { "Starting Graph", QObject::tr("Starting Graph") }; }
	static AgxTerm SequenceName() { return { "Sequence Name", QObject::tr("Sequence Name") }; }
	static AgxTerm SequenceCurrentFrameIndex() { return { "Sequence Current Frame Index", QObject::tr("Sequence Current Frame Index") }; }
	static AgxTerm StateMachineName() { return { "State Machine Name", QObject::tr("State Machine Name") }; }
	static AgxTerm StateMachine() { return { "State Machine", QObject::tr("State Machine") }; }
	static AgxTerm SendEventOnCatch() { return { "Send Event On Catch", QObject::tr("Send Event On Catch") }; }
	static AgxTerm SendEvent() { return { "Send Event", QObject::tr("Send Event") }; }
	static AgxTerm Speed() { return { "Speed", QObject::tr("Speed") }; }
	static AgxTerm SpringCoefficientOverride() { return { "Spring Coefficient Override", QObject::tr("Spring Coefficient Override") }; }
	static AgxTerm SpeedParameter() { return { "Speed Parameter", QObject::tr("Speed Parameter") }; }
	static AgxTerm SpringCoefficient() { return { "Spring Coefficient", QObject::tr("Spring Coefficient") }; }
	static AgxTerm ScaleSetVariable() { return { "Scale Set Variable", QObject::tr("Scale Set Variable") }; }
	static AgxTerm SpeedVar() { return { "Speed Var", QObject::tr("Speed Var") }; }
	static AgxTerm StartValue() { return { "Start Value", QObject::tr("Start Value") }; }
	static AgxTerm SourceBoneName() { return { "Source Bone Name", QObject::tr("Source Bone Name") }; }
	static AgxTerm StoryManagerEvent() { return { "SM Evt", QObject::tr("Story Manager Event") }; }
	static AgxTerm Set() { return { "Set", QObject::tr("Set") }; }
	static AgxTerm StateName() { return { "State Name", QObject::tr("State Name") }; }
	static AgxTerm SpeedClassification() { return { "Speed Classification", QObject::tr("Speed Classification") }; }
	static AgxTerm StartSelectionMethod() { return { "Start Selection Method", QObject::tr("Start Selection Method") }; }
	static AgxTerm StartSelectionVariable() { return { "Start Selection Variable", QObject::tr("Start Selection Variable") }; }
	static AgxTerm SwapGraphEvents() { return { "Swap Graph Events", QObject::tr("Swap Graph Events") }; }
	static AgxTerm States() { return { "States", QObject::tr("States") }; }
	static AgxTerm StandToMove() { return { "Stand To Move", QObject::tr("Stand To Move") }; }
	static AgxTerm Standard() { return { "Standard", QObject::tr("Standard") }; }
	static AgxTerm Small() { return { "Small", QObject::tr("Small") }; }
	static AgxTerm Sync() { return { "Sync", QObject::tr("Sync") }; }
	static AgxTerm Suffix() { return { "Suffix", QObject::tr("Suffix") }; }
	static AgxTerm Smooth() { return { "Smooth", QObject::tr("Smooth") }; }
	static AgxTerm ShortestDirection() { return { "Shortest Direction", QObject::tr("Shortest Direction") }; }
	static AgxTerm StartOrient() { return { "Start Orient", QObject::tr("Start Orient") }; }
	static AgxTerm StartPosition() { return { "Start Position", QObject::tr("Start Position") }; }

	static AgxTerm Tag() { return { "Tag", QObject::tr("Tag") }; }
	static AgxTerm TagName() { return { "TagName", QObject::tr("Tag Name") }; }
	static AgxTerm Tag_Name() { return { "Tag Name", QObject::tr("Tag Name") }; }
	static AgxTerm TagAlteration() { return { "Tag Alteration", QObject::tr("Tag Alteration") }; }
	static AgxTerm TagSyncSystem() { return { "Tag Sync System", QObject::tr("Tag Sync System") }; }
	static AgxTerm TypeofPropagation() { return { "Type of Propagation", QObject::tr("Type of Propagation") }; }
	static AgxTerm Type() { return { "Type", QObject::tr("Type") }; }
	static AgxTerm Time() { return { "Time", QObject::tr("Time") }; }
	static AgxTerm TurnAnimCameraVar() { return { "Turn Anim Camera Var", QObject::tr("Turn Anim Camera Var") }; }
	static AgxTerm TargetBone() { return { "Target Bone", QObject::tr("Target Bone") }; }
	static AgxTerm TimePercent() { return { "Time Percent", QObject::tr("Time Percent") }; }
	static AgxTerm TimePercentMin() { return { "Time Percent Min", QObject::tr("Time Percent Min") }; }
	static AgxTerm TimePercentMax() { return { "Time Percent Max", QObject::tr("Time Percent Max") }; }
	static AgxTerm ToeLBoneName() { return { "Toe L Bone Name", QObject::tr("Toe L Bone Name") }; }
	static AgxTerm ToeRBoneName() { return { "Toe R Bone Name", QObject::tr("Toe R Bone Name") }; }
	static AgxTerm TargetBlendTime() { return { "Target Blend Time", QObject::tr("Target Blend Time") }; }
	static AgxTerm ThighsBlendAmount() { return { "Thighs Blend Amount", QObject::tr("Thighs Blend Amount") }; }
	static AgxTerm TargetBoneName() { return { "Target Bone Name", QObject::tr("Target Bone Name") }; }
	static AgxTerm TransitionType() { return { "Transition Type", QObject::tr("Transition Type") }; }
	static AgxTerm Tags() { return { "Tags", QObject::tr("Tags") }; }
	static AgxTerm Threshold() { return { "Threshold", QObject::tr("Threshold") }; }
	static AgxTerm TurnAnimation() { return { "Turn Animation", QObject::tr("Turn Animation") }; }
	static AgxTerm TurnAnimationModifyTranslation() { return { "Turn Animation Modify Translation", QObject::tr("Turn Animation Modify Translation") }; }

	static AgxTerm UpdateRefCountWhenInactive() { return { "Update Ref Count When Inactive", QObject::tr("Update Ref Count When Inactive") }; }
	static AgxTerm UseRadians() { return { "Use Radians", QObject::tr("Use Radians") }; }
	static AgxTerm UpdateWhileInactive() { return { "Update While Inactive", QObject::tr("Update While Inactive") }; }
	static AgxTerm Unknown1() { return { "Unknown 1", QObject::tr("Unknown 1") }; }
	static AgxTerm Unknown2() { return { "Unknown 2", QObject::tr("Unknown 2") }; }
	static AgxTerm Unknown3() { return { "Unknown 3", QObject::tr("Unknown 3") }; }

	static AgxTerm VariableControl() { return { "Variable Control", QObject::tr("Variable Control") }; }
	static AgxTerm VariableName() { return { "Variable Name", QObject::tr("Variable Name") }; }
	static AgxTerm VariableType() { return { "Variable Type", QObject::tr("Variable Type") }; }
	static AgxTerm Variable() { return { "Variable", QObject::tr("Variable") }; }
	static AgxTerm VictimSuffix() { return { "Victim Suffix", QObject::tr("Victim Suffix") }; }
	static AgxTerm Value() { return { "Value", QObject::tr("Value") }; }
	static AgxTerm Variable1() { return { "Variable 1", QObject::tr("Variable 1") }; }
	static AgxTerm Variable2() { return { "Variable 2", QObject::tr("Variable 2") }; }
	static AgxTerm Variable3() { return { "Variable 3", QObject::tr("Variable 3") }; }
	static AgxTerm VariableOut() { return { "Variable Out", QObject::tr("Variable Out") }; }

	static AgxTerm WrapToRange() { return { "Wrap To Range", QObject::tr("Wrap To Range") }; }
	static AgxTerm Weight() { return { "Weight", QObject::tr("Weight") }; }
	static AgxTerm WrappedGraph() { return { "Wrapped Graph", QObject::tr("Wrapped Graph") }; }
	static AgxTerm WeightorVariable() { return { "Weight or Variable", QObject::tr("Weight or Variable") }; }
	static AgxTerm WalkRunSpeed() { return { "Walk Run Speed", QObject::tr("Walk Run Speed") }; }
	static AgxTerm WalkNormalized() { return { "Walk Normalized", QObject::tr("Walk Normalized") }; }
	static AgxTerm WhileMoving() { return { "While Moving", QObject::tr("While Moving") }; }
	static AgxTerm Walk() { return { "Walk", QObject::tr("Walk") }; }

	static AgxTerm XYTransLockTolerance() { return { "X, Y, Trans Lock Tolerance", QObject::tr("X, Y, Trans Lock Tolerance") }; }
	static AgxTerm X() { return { "X", QObject::tr("X") }; }

	static AgxTerm Y() { return { "Y", QObject::tr("Y") }; }

	static AgxTerm Z() { return { "Z", QObject::tr("Z") }; }
	static AgxTerm ZerotoOne() { return { "Zero to One", QObject::tr("Zero to One") }; }







};