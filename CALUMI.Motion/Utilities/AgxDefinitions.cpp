#include "stdafx.h"
#include "AgxDefinitions.h"
#include "AgxDictionary.h"
#include "AgxFormat.h"
//#include <AgxNodes/AgxNodes>

const QMap<QString, AgxEventType> AgxEventTypeMap = {
	{"Graph Event", AgxEventType::Graph_Event},
	{"Game Event", AgxEventType::Game_Event},
	{"Both", AgxEventType::Both},
	{"Special", AgxEventType::Special},
	{"UNDEFINED", AgxEventType::UNDEFINED},
	{"All", AgxEventType::UNDEFINED}
};

const QStringList AgxIntegerVars = {
	"ActiveTurnInPlace",
	"AimAndAttack",
	"AnimObjActive",
	"ArmillaryAssemblePhase1",
	"ArmillaryAssemblePhase2",
	"ArmillaryAssemblePhase3",
	"ArmillaryAssemblePhase4",
	"ArmillaryAssemblePhase5",
	"ArmillaryAssemblePhase6",
	"ArmillaryPhase1",
	"ArmillaryPhase2",
	"ArmillaryPhase3",
	"ArmillaryPhase4",
	"ArmillaryPhase5",
	"ArmillaryPhase6",
	"bAlignWithAimAssistTarget",
	"bAllowFinisher",
	"bAllowReversal",
	"bBlockMeleeBash",
	"bCameraHits",
	"bCanComboAttack",
	"bCanQueueAttackWhileReloading",
	"bCombatWalk",
	"bDelayReloading",
	"bDelayStandStart",
	"bDisableAlert",
	"bDisableFurnitureHeadtrack",
	"bDisableWeaponConstraint",
	"bDontAllowAttack",
	"bDontAllowBlock",
	"bDontAllowFlinch",
	"bDontAllowPowerAttack",
	"bDontAllowReloading",
	"bDoReloadLoop",
	"bEnableHandIK",
	"bEnableLeftHandIK",
	"bEnableRightHandIK",
	"bFootLeftLock",
	"bFootLeftUnlock",
	"bFootRightLock",
	"bFootRightUnlock",
	"bFreeMovement",
	"bHasAnimObjectVisible",
	"bHeadLightIsVisible",
	"bHelmetIsVisible",
	"bInitializeInstant",
	"bInRelaxedPose",
	"bInReloadState",
	"bInReloadStateWithoutSightMode",
	"bIsAttacking",
	"bIsBleedingOut",
	"bIsBlocking",
	"bIsEnteringCover",
	"bIsEquipping",
	"bIsExitingCover",
	"bIsFirstPerson",
	"bIsFurnitureExit",
	"bIsGrazing",
	"bIsHiding",
	"bIsInAir",
	"bIsInCover",
	"bIsInLargeFlavor",
	"bIsInScene",
	"bIsLanding",
	//"bIsMantling", //Duplicated somehow in BGS source
	"bIsMantling",
	"bIsMelee",
	"bIsMirrored",
	"bIsNotTalking",
	"bIsReloading",
	"bIsShuffling",
	"bIsSliding",
	"bIsStaggered",
	"bIsTouchingGround",
	"bIsTunneling",
	"bIsUnequipping",
	"bLadderLeftHandUp",
	"bLandInPlace",
	"bLeftHandCasting",
	"bManualAnimObjUnequip",
	"bNoAdditivePose",
	"bNoAim",
	"bNoAlertAdditive",
	"bNoArmorOffset",
	"bNoFeatherSpine",
	"bNoFlavor",
	"bNoFootIK",
	"bNoHeadTrack",
	"bNoIdleBack",
	"bNoIdleFace",
	"bNoLeftHandIK",
	"bNoRightHandIK",
	"BoneWeightsAction",
	"bPartialBodyInstant",
	"bPlayerMoveStartActive",
	"bPlayReloadEnd",
	"bReloadAborted",
	"bRetainEquipment",
	"bShipPitchDown",
	"bShipPitchUp",
	"bShipRoll",
	"bShipStrafeDown",
	"bShipStrafeLeft",
	"bShipStrafeLR",
	"bShipStrafeRight",
	"bShipStrafeUD",
	"bShipStrafeUp",
	"bShipYawLeft",
	"bShipYawRight",
	"bTailAttack",
	"bUseSequentialReloading",
	"bUseSoaring",
	"bUseStandToExit",
	"bWeaponLeftToLAnimObject",
	"bWeaponLeftToRAnimObject",
	"bWeaponLeftToRAnimObject3",
	"bWeaponToRAnimObject",
	"bZeroGSpine",
	"CameraNearFar",
	"CanEnableGunFailsafe",
	"CanStaySightedWhileReloading",
	"cHitReactionBodyPart",
	"cHitReactionDir",
	"CumulativeAnimationsCount",
	"DisableAllowRotation",
	"DisableAnimationDriven",
	"HasPower",
	"HitReactionTransferVariable",
	"iAmmoCount",
	"iAttackState",
	"iAttackState0",
	"iAttackState1",
	"iAttackState2",
	"iAttackState3",
	"iAttackState4",
	"iAttackState5",
	"iAttackState6",
	"iAttackState7",
	"iCriticalHitType",
	"iIsBlindFiring",
	"iIsGunDown",
	"iIsInSneak",
	"iIsPlayer",
	"iIsPoseB",
	"iIsSighted",
	"iLadderClimbState",
	"iMagazineCapacity",
	"iPartialCover",
	"iPlayerLadderClimbAnimation",
	"iPlayingSprintAnimation",
	"iRigIndex",
	"iRootSwitch",
	"IsCompletelyStopped",
	"iSightedRequested",
	"IsNotAligned",
	"IsOn",
	"iStaggerType",
	"iState",
	"iSwitchingPerspectives",
	"iSyncActions",
	"iSyncActorStance",
	"iSyncBleedout",
	"iSyncCoverStates",
	"iSyncFullbodyActions",
	"iSyncGravDash",
	"iSyncGravity",
	"iSyncGunActions",
	"iSyncIdleLocomotion",
	"iSyncIdleTurnState",
	"iSyncIdleWalkRun",
	"iSyncInDialogueScene",
	"iSyncInjured",
	"iSyncInventoryCamera",
	"iSyncJumpState",
	"iSyncMeleeActions",
	"iSyncMoveDirection",
	"iSyncOnGround",
	"iSyncOpenClose",
	"iSyncPartialBody",
	"iSyncPartialBodyBlend",
	"iSyncPartialBodyState",
	"iSyncPose",
	"iSyncPower",
	"iSyncRelaxedReadyAlert",
	"iSyncRelaxReadySneak",
	"iSyncRightLeft",
	"iSyncSighted",
	"iSyncSprintState",
	"iSyncStandingCrouching",
	"iSyncStateAB",
	"iSyncStunState",
	"iSyncSwimState",
	"iSyncTransitions",
	"iSyncTurnLeft",
	"iSyncTurnRight",
	"iSyncTurnState",
	"iTurnType",
	"iWantBlock",
	"NeedPower",
	"PairedAnimHash",
	"SetColorA",
	"SetColorB",
	"SetStateA",
	"SetStateB",
	"StaggerMagnitude",
	"State01Index",
	"State02Index",
	"StateA01Index",
	"StateA02Index",
	"StateB01Index",
	"StateB02Index",
	"SyncJumpBlend",
	"UnequipInterruptable",
	"VehicleIsBoosting",
	"VehicleIsVertBoosting",
	"VehicleWheelOnGround"
};

const QStringList AgxFloatVars = {
"Acceleration",
"AimWobble",
"AimWobbleSpeedMult",
"Amount",
"AnimSpeed",
"Body Length",
"CameraFOV",
"CameraYawSpeed",
"Capacity",
"CapacityPercent",
"ChestHeadingMax",
"ChestHeadingMin",
"CurrentAmount",
"CurrentAperture",
"CurrentGraphState",
"CurrentPosition",
"CurrentRotation",
"CurrentSpeed",
"DirectAt Heading Delta",
"DirectAtHeadingMax",
"DirectAtHeadingMin",
"DirectAtHeadingVar",
"DirectAtPitchMax",
"DirectAtPitchMin",
"DirectAtPitchVar",
"Direction",
"DirectionSmoothed",
"DisableHeadingMax",
"DisableHeadingMin",
"DistanceToStop",
"DrawSpeedMult",
"fActorScale",
"fActorScaleInverse",
"fAmmoCountAsTimePercent",
"fAmmoCountPercentage",
"fAmmoRemaining",
"fBlendTime_Combat",
"fBlendTime_Default",
"fBlendTime_IdleLocomotionSwitch",
"fCameraPitch",
"fCameraYaw",
"fChargePercentage",
"fCockpitBoost",
"fCockpitBrakeThrusters",
"fCockpitEngineState",
"fCockpitGForce",
"fCockpitGravDriveState",
"fCockpitHorizontalInput",
"fCockpitHullIntegrityState",
"fCockpitPitch",
"fCockpitReactorState",
"fCockpitRoll",
"fCockpitShieldState",
"fCockpitSpeed",
"fCockpitTargetLock",
"fCockpitThrottle",
"fCockpitThrottleInput",
"fCockpitThrottleReverse",
"fCockpitThrusters",
"fCockpitVerticalInput",
"fCockpitYaw",
"fControllerMoveXDampen",
"fControllerMoveXRaw",
"fControllerMoveYRaw",
"fControllerMoveZRaw",
"fControllerXRaw",
"fControllerXRawDampen",
"fControllerYRaw",
"fControllerYRawDampen",
"fDesiredHeading",
"fFastWalkSpeed",
"fFlightHorizontalInput",
"fFlightMaxSpeed",
"fFlightPitchInput",
"fFlightRollInput",
"fFlightThrottleInput",
"fFlightVerticalInput",
"fFlightYawInput",
"fFXgeneric01",
"fFXgeneric02",
"fFXgeneric03",
"fGravityScale",
"fGunDownToGunUpDuration",
"fGunUpToGunDownDuration",
"fHeadingSpeed",
"fHintVisibility",
"fHipRelativeMotion",
"fImpactDist",
"fInventoryHeading",
"fMeterCount",
"fMeterCountPercentage",
"fPitchSpeed",
"fPowerIntensity",
"fProjectilePower",
"fRandomClipStartTimePercentage",
"fRingControl",
"fRingSpeed",
"fRingSpeedFast",
"fRingSpeedMedium",
"fRingSpeedSlow",
"fRingSpin",
"fRingSuccessSpeed",
"fRingTarget",
"fShipQuadrant",
"fThrottleDamp",
"fTurretHeadingMax",
"fTurretHeadingMin",
"fTurretPitchMax",
"fTurretPitchMin",
"fVariableRangeAperture",
"fVariableRangeDistance",
"GripOffset",
"HeadHeadingMax",
"HeadHeadingMin",
"LockAlignementToCameraDuringTurnAnimation",
"LookAtHeadingMax",
"LookAtHeadingMin",
"LookAtHeadingVar",
"LookAtPitchMax",
"LookAtPitchMin",
"LookAtPitchVar",
"LookHeading",
"LookHeadingOffset",
"MantleHeadingOffset",
"MeleeAttack_Bite02",
"NPCHeading",
"NPCHeadingOffset",
"OpenCloseFormState",
"PairedHeadingOffset",
"Pitch",
"PitchDelta",
"PitchOffset",
"PlaybackSpeedMult",
"Position",
"ReloadSpeedMult",
"Rotation",
"RotationSpeed",
"ShipPitchDown",
"ShipPitchUp",
"ShipRoll",
"ShipStrafeDown",
"ShipStrafeLeft",
"ShipStrafeRight",
"ShipStrafeUp",
"ShipYawLeft",
"ShipYawRight",
"SightedSpeedMult",
"SightedTransitionSec",
"Speed",
"StaggerDirection",
"StartAltIdle",
"StartsOff",
"TurnDelta",
"TurnDeltaSmoothed",
"VehicleAirTime",
"VehicleForwardVelocity",
"VehicleLateralVelocity",
"VehicleSpeed",
"VehicleWheelForwardSlipVelocity",
"VehicleZVelocity",
"VelocityZ",
"VerticalDirection",
"WeaponAttackAnimationDuration",
"WeaponSpeedMult",
"WeaponSpeedMult1",
"WeaponSpeedMult2",

};

const QStringList AgxBooleanVars = {
	"bAimActive",
	"bChargeRecharging",
	"bDoBoltCharge",
	"bHeadTrackingActive",
	"bHelmetLightIsOn",
	"bIsInCamera",
	"bIsPathing",
	"bIsPlayerShip",
	"bPairedLead",
	"IsInstantInit",
	"IsUsingCodeDrivenRotation",
	"MantleTogether",
	"PlayPartialBodyStart",
	"ShouldUseTurnOnSpotGraph",
	"TurnOnSpotEnforceDirectAtLimits"
};

const QStringList AgxVectorVars = {
	"AimAtLocation",
	"DesiredLocation",
	"LadderStepDesiredLocation",
	"LookAtDir",
	"LookAtLocation",
	"MantleEndPos",
	"PairedTranslationOffset"
};

const QStringList AgxActionVars = {
	"Activate",
	"Assemble",
	"Attack1",
	"Attack2",
	"Attack3",
	"Attack4",
	"BoltCharge",
	"Camera",
	"Camera_JumpLand",
	"CameraHit",
	"Capacity",
	"CastSelf",
	"CastTarget",
	"ChargeDown",
	"ChargeUp",
	"ChargeUp_LeftArm",
	"ChargeUp_RightArm",
	"Close",
	"Collect",
	"Death",
	"Death_Backward",
	"Death_Forward",
	"Death_Head",
	"Death_Left",
	"Death_LeftArm",
	"Death_LeftLeg",
	"Death_Moving_Forward",
	"Death_Right",
	"Death_RightArm",
	"Death_RightLeg",
	"Death_Torso",
	"DeleteMe",
	"Deploy",
	"DirectionMarker",
	"Dock",
	"Dock_Bottom",
	"Dock_Nose",
	"Dock_Port",
	"Dock_Starboard",
	"Dock_Starborn",
	"Dock_Top",
	"DodgeBack",
	"DodgeLeft",
	"DodgeRight",
	"Down",
	"DynamicAnimation",
	"EnterFromStand",
	"EnterInstant",
	"Equip",
	"Equip_LeftArm",
	"Equip_RightArm",
	"EquipFast",
	"EvadeLeft",
	"EvadeRight",
	"EvadeRunning",
	"Exit",
	"ExitMirrored",
	"ExitToDead",
	"ExitToStand",
	"FireAutoPush",
	"FireSingle",
	"FireSingle_LeftArm",
	"FireSingle_Mouth1",
	"FireSingle_Mouth2",
	"FireSingle_RightArm",
	"FireSingle_Tail",
	"FireSingleBinary",
	"Flavor01",
	"Flavor02",
	"Flavor03",
	"Flavor04",
	"Flavor05",
	"Flavor06",
	"Flavor07",
	"Flavor08",
	"Flavor09",
	"Flavor10",
	"FlavorLrg01",
	"FlavorLrg02",
	"FlavorLrg03",
	"FlavorLrg04",
	"Flinch",
	"Flinch_Head",
	"Flinch_LeftArm",
	"Flinch_LeftLeg",
	"Flinch_RightArm",
	"Flinch_RightLeg",
	"Flinch_Torso",
	"FrontEnterFromStand",
	"FrontEnterFromWalk",
	"FrontExitToRun",
	"FrontExitToStand",
	"FrontExitToWalk",
	"FurnitureDeath",
	"GetUp_Back",
	"GetUp_Front",
	"GetUp_Left",
	"GetUp_Right",
	"GravDash_Long",
	"GravDash_Medium",
	"GravDash_Short",
	"GravDriveCamera",
	"GravDriveExtend",
	"GravDriveRetract",
	"GravJump",
	"GravJumpInitiate",
	"Idle_Dialogue",
	"Idle_Flavor",
	"Idle_Flavor01",
	"Idle_Flavor02",
	"Idle_Flavor03",
	"Idle_Flavor04",
	"Idle_Flavor05",
	"Idle_Flavor06",
	"Idle_Flavor07",
	"Idle_Flavor08",
	"Idle_Flavor09",
	"Idle_Flavor10",
	"Idle_Flavor11",
	"Idle_Flavor12",
	"Idle_Flavor13",
	"Idle_Flavor14",
	"Idle_Flavor15",
	"Idle_FlavorLarge",
	"Idle_FlavorLarge01",
	"Idle_FlavorLarge02",
	"Idle_FlavorLarge03",
	"Idle_FlavorLarge04",
	"Idle_FlavorLarge05",
	"Idle_FlavorPlayer",
	"Idle_Taunt",
	"IdleFace_Flavor",
	"IdleFace_Flavor01",
	"IdleFace_Flavor02",
	"IdlePartialBody_Flavor",
	"IdlePartialBody_Start",
	"IdlePartialBody_Stop",
	"Into",
	"JigglePitchDown",
	"JigglePitchUp",
	"JiggleRollLeft",
	"JiggleRollRight",
	"JiggleYawLeft",
	"JiggleYawRight",
	"JumpInPlace",
	"JumpInPlaceBoost",
	"JumpInPlaceFall",
	"JumpInPlaceLand",
	"JumpRunBackward",
	"JumpRunBackwardBoost",
	"JumpRunBackwardFall",
	"JumpRunForward",
	"JumpRunForwardBoost",
	"JumpRunForwardFall",
	"JumpRunLand",
	"JumpRunLeft",
	"JumpRunLeftBoost",
	"JumpRunLeftFall",
	"JumpRunRight",
	"JumpRunRightBoost",
	"JumpRunRightFall",
	"JumpTraversal",
	"Knockdown",
	"Ladder_ClimbDown_LeftHand",
	"Ladder_ClimbDown_RightHand",
	"Ladder_ClimbUp_LeftHand",
	"Ladder_ClimbUp_RightHand",
	"Landing",
	"Landing_NPC",
	"Landing_Starborn",
	"Landing_Starborn_NPC",
	"Landing_Vertical",
	"LandingKreet",
	"LandingKreetPirates",
	"LandingLarge",
	"LeanRight",
	"Left",
	"LeftEnterFromStand",
	"LeftEnterFromWalk",
	"LeftExitToRun",
	"LeftExitToStand",
	"LeftExitToWalk",
	"LeftQuickExitToStand",
	"LooseAnim_Shot11",
	"LooseAnim_Shot12",
	"LooseAnim_Shot13",
	"LooseAnim_Shot14",
	"LooseAnim_Shot15_Patch",
	"LooseAnim_Shot21_LookUp",
	"LooseAnim_Shot21b_CloseUp",
	"Mantle",
	"MantleLow",
	"MantleTogether",
	"MeleeAttack_A",
	"MeleeAttack_B",
	"MeleeAttack_Bite01",
	"MeleeAttack_Bite02",
	"MeleeAttack_Bite03",
	"MeleeAttack_C",
	"MeleeAttack_Claw01",
	"MeleeAttack_Claw02",
	"MeleeAttack_D",
	"MeleeAttack_E",
	"MeleeAttack_Head01",
	"MeleeAttack_Head02",
	"MeleeAttack_Head03",
	"MeleeAttack_Left180",
	"MeleeAttack_Left90",
	"MeleeAttack_LeftArm",
	"MeleeAttack_Right180",
	"MeleeAttack_Right90",
	"MeleeAttack_RightArm",
	"MeleeAttack_RightClaw01",
	"MeleeAttack_Tail01",
	"MeleeAttack_Tail02",
	"MeleeAttack01",
	"MeleeAttack02",
	"MeleeAttack03",
	"MeleeAttack04",
	"MeleeAttack05",
	"MeleeAttackBash",
	"MeleeAttackBashMiss",
	"MeleeAttackForward_A",
	"MeleeAttackForward_B",
	"MeleeAttackForward_C",
	"MeleeAttackForward_D",
	"MeleeAttackForward_E",
	"MeleeAttackInAir_A",
	"MeleeAttackPower",
	"MeleeAttackPower_A",
	"MeleeAttackPower_B",
	"MeleeAttackPower_Bite01",
	"MeleeAttackPower_Bite02",
	"MeleeAttackPower_Claw01",
	"MeleeAttackPower_Head01",
	"MeleeAttackPower_Tail01",
	"MeleeAttackPower01",
	"MeleeAttackPower02",
	"MeleeAttackPowerForward_A",
	"MeleeAttackPowerForward_B",
	"MeleeAttackSprint",
	"MeleeBlock_HitReaction",
	"MeleeBlock_Into",
	"MeleeBlock_Out",
	"MeleePowerAttack_Forward01",
	"Open",
	"Out",
	"PitchDown",
	"PitchUp",
	"PoseBToPoseA",
	"PoweringDown",
	"PoweringUp",
	"QuickExitToStand",
	"Reaction01",
	"Reaction02",
	"Reaction03",
	"Reaction04",
	"Reaction05",
	"ReadyToRelaxed",
	"ReadyToSneak",
	"RearEnterFromStand",
	"RearEnterFromWalk",
	"RearExitToRun",
	"RearExitToStand",
	"RearExitToWalk",
	"RearQuickExitToStand",
	"Recoil_LeftArm",
	"Recoil_RightArm",
	"RecoverLeft",
	"RecoverRight",
	"RelaxedToReady",
	"RelaxedToSneak",
	"Reload",
	"ReloadEnd",
	"ReloadLoop",
	"Right",
	"RightEnterFromStand",
	"RightEnterFromWalk",
	"RightExitToRun",
	"RightExitToStand",
	"RightExitToWalk",
	"RightQuickExitToStand",
	"RunBackward_Start",
	"RunBackward_Stop",
	"RunBackwardToForward",
	"RunForward_Start",
	"RunForward_Stop",
	"RunForwardToBackward",
	"RunLeft_Start",
	"RunLeft_Stop",
	"RunLeftToRight",
	"RunRight_Start",
	"RunRight_Stop",
	"RunRightToLeft",
	"SeatAnim_Shot11",
	"SeatAnim_Shot12",
	"SeatAnim_Shot13",
	"SeatAnim_Shot14",
	"SeatAnim_Shot15",
	"SeatAnim_Shot21",
	"SeatAnim_Shot21b",
	"Sequence",
	"SequenceA",
	"SequenceA01",
	"SequenceA02",
	"SequenceB",
	"SequenceC",
	"SequenceC01",
	"ShipScreenFX_Boost",
	"ShipScreenFX_BrakeThrusters",
	"ShipScreenFX_DriveState",
	"ShipScreenFX_EngineState",
	"ShipScreenFX_GForce",
	"ShipScreenFX_HorizontalInput",
	"ShipScreenFX_HullIntegrityState",
	"ShipScreenFX_Pitch",
	"ShipScreenFX_ReactorState",
	"ShipScreenFX_Reverse",
	"ShipScreenFX_Roll",
	"ShipScreenFX_ShieldState",
	"ShipScreenFX_ShieldState_Dmg",
	"ShipScreenFX_ShieldState_Offline",
	"ShipScreenFX_Speed",
	"ShipScreenFX_TargetLock",
	"ShipScreenFX_Throttle",
	"ShipScreenFX_VerticalInput",
	"ShipScreenFX_Yaw",
	"ShipScreensFX_Thrusters_Aft_D",
	"ShipScreensFX_Thrusters_Aft_P",
	"ShipScreensFX_Thrusters_Aft_S",
	"ShipScreensFX_Thrusters_Aft_U",
	"ShipScreensFX_Thrusters_Fore_D",
	"ShipScreensFX_Thrusters_Fore_P",
	"ShipScreensFX_Thrusters_Fore_S",
	"ShipScreensFX_Thrusters_Fore_U",
	"ShuffleBackward",
	"ShuffleForward",
	"SightedEnd",
	"SightedStart",
	"Sit_Start",
	"Slaved_IdleFlavor",
	"Slaved_IdleFlavor01",
	"Slaved_IdleFlavor02",
	"Slaved_IdleFlavor03",
	"Slaved_IdleFlavor04",
	"Slaved_IdleFlavor05",
	"Slaved_IdleFlavor06",
	"Slaved_IdleFlavor08",
	"Slaved_IdleFlavor09",
	"SlideFromSprint",
	"SneakStart",
	"SneakStop",
	"SneakToReady",
	"SneakToRelaxed",
	"SprintStart",
	"SprintStop",
	"Stage1Idle",
	"Stage1Transition",
	"Stage2Idle",
	"Stage2Transition",
	"StaggerBackwardLarge",
	"StaggerBackwardLarge_Left",
	"StaggerBackwardLarge_Right",
	"StaggerBackwardMedium",
	"StaggerBackwardMedium_Left",
	"StaggerBackwardMedium_Right",
	"StaggerBackwardSmall",
	"StaggerBackwardSmall_Left",
	"StaggerBackwardSmall_Right",
	"StaggerForward",
	"StaggerForwardLarge",
	"StaggerForwardMedium",
	"StaggerForwardSmall",
	"StaggerLeft",
	"StaggerRight",
	"Stand_Start",
	"Start",
	"StateA_Play",
	"StateB_Play",
	"StateC_Play",
	"StateD_Play",
	"Stop",
	"TakeOff",
	"TakeOff_Neon",
	"TakeOff_NPC",
	"TakeOff_Starborn",
	"TakeOff_Starborn_NPC",
	"TakeOff_Vertical",
	"TakeOffE3",
	"Talk_L",
	"Talk_M",
	"Talk_S",
	"ThrowGrenade",
	"ThrowMine",
	"ToCoverCrouching",
	"ToCoverCrouchingMirror",
	"ToCoverStanding",
	"ToCoverStandingMirror",
	"ToMirror",
	"Traversal_80cm_RunForward",
	"Traversal_Door",
	"Traversal_PressButton",
	"TunnelStart",
	"TunnelStop",
	"TurningOff",
	"TurningOn",
	"TurnInPlace_Left180",
	"TurnInPlace_Left30",
	"TurnInPlace_Left90",
	"TurnInPlace_Right180",
	"TurnInPlace_Right30",
	"TurnInPlace_Right90",
	"TurnLeft",
	"TurnRight",
	"UnDock",
	"UnDock_Bottom",
	"UnDock_Nose",
	"UnDock_Port",
	"UnDock_Starboard",
	"UnDock_Starborn",
	"UnDock_Top",
	"Unequip",
	"Unequip_LeftArm",
	"Unequip_RightArm",
	"Up",
	"WalkBackward_Start",
	"WalkBackward_Stop",
	"WalkBackwardToForward",
	"WalkForward_Start",
	"WalkForward_Stop",
	"WalkForwardToBackward",
	"WalkLeft_Start",
	"WalkLeft_Stop",
	"WalkLeftToRight",
	"WalkRight_Start",
	"WalkRight_Stop",
	"WalkRightToLeft",
	"ZeroG_GetUp_Back",
	"ZeroG_GetUp_Front"
};

const QStringList AgxStateVars = {
	"AdditiveEyeIdle",
	"AdditivePose",
	"Anim01",
	"Anim02",
	"ArmorOffset",
	"BoostDown",
	"BoostUp",
	"ChargeHold",
	"Closed",
	"CriticalHit",
	"Empty",
	"FireAuto",
	"FireAuto_LeftArm",
	"FireAuto_RightArm",
	"FireAuto_Tail",
	"FurnitureDynamicAnimationLoop",
	"GravDriveActive",
	"GripOffset_LeftArm",
	"GripOffset_RightArm",
	"GripOffset_RightArm_MirrorCover",
	"GunDown",
	"HyperSection01",
	"HyperSection01Idle",
	"HyperSection01Off",
	"HyperSection02Idle",
	"Idle",
	"Idle_FlavorLoop",
	"Idle_LeftArm",
	"Idle_RightArm",
	"IdleBack",
	"IdleDead",
	"IdleEmpty",
	"IdleFace",
	"IdlePartialBody",
	"IdlePoseOffset",
	"IdleSighted",
	"IdleSitting",
	"IdleToSneak",
	"IdleWobble",
	"JogBackward",
	"JogBackwardLeft",
	"JogBackwardRight",
	"JogForward",
	"JogForwardLeft",
	"JogLeft",
	"JogRight",
	"JumpInPlaceFallLoop",
	"JumpRunBackwardFallLoop",
	"JumpRunForwardFallLoop",
	"JumpRunLeftFallLoop",
	"JumpRunRightFallLoop",
	"Landed",
	"LeftArm",
	"LookDirections",
	"LookDirections_Gun",
	"LookDirections_Head",
	"LookDirections_LeftArm",
	"LookDirections_RightArm",
	"LookDirections_Tail",
	"MeleeBlock_Idle",
	"Off",
	"OffsetState",
	"On",
	"OnInactive",
	"Opened",
	"PitchDown_COM",
	"PitchUp_COM",
	"Pose",
	"RightArm",
	"RunBackward",
	"RunBackwardLeft",
	"RunBackwardRight",
	"RunForward",
	"RunForward_TurnLeft",
	"RunForward_TurnRight",
	"RunForwardLeft",
	"RunLeft",
	"RunRight",
	"SneakToIdle",
	"SoarForward",
	"Spin",
	"Spine",
	"Spine_BoneWeights",
	"Sprint",
	"Sprint_TurnLeft",
	"Sprint_TurnRight",
	"Stage1",
	"Stage2",
	"StateA_Idle",
	"StateB_Idle",
	"StateC_Idle",
	"StateD_Idle",
	"Tail_BoneWeights",
	"TPose",
	"TrotForward",
	"UpperBody_BoneWeights",
	"WalkBackward",
	"WalkBackwardLeft",
	"WalkBackwardRight",
	"WalkForward",
	"WalkForward_TurnLeft",
	"WalkForward_TurnLeftTight",
	"WalkForward_TurnRight",
	"WalkForward_TurnRightTight",
	"WalkForwardLeft",
	"WalkLeft",
	"WalkRight",
	"ZeroG_BoneWeights"
};

const QHash<QString, AgxEventInfo>* AgxEventVars = new QHash<QString, AgxEventInfo>({
	{"<none>",{AgxEventType::UNDEFINED, AgxTrueFalse::UNDEFINED}},
	{"ActionEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ActionInterrupt",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Activate",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ActivateInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ActivationDone",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"AddRagdollToWorld",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"AlertStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AlphaEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"AlphaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"AmbushEnter",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AmbushEnterInstant",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AmbushExit",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AnimationComplete",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AnimationOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"AnimationOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"AnimFinish",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AnimObjDraw",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"AnimObjHide",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"AnimObjLoad",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"AnimObjUnequip",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"AnimObjUnequipAll",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Arrive",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"AssignVariable",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"AttackEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackRelease",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackState",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackState2",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackStateNextAttack",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"AttackStateReset",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"AttackStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BeginCastLeft",	{AgxEventType::Game_Event,	AgxTrueFalse::TRUE}},
	{"BeginWeaponDraw",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"BeginWeaponSheathe",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"BetaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"BetaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"BleedOutBegin",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"BleedOutEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"BleedOutStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"BleedOutStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlindFireEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlindFireOverStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlindFireStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlockHitStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlockStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BlockStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Blue",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"BoltChargeStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"BoostAnimFinish",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BoostBegin",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"BoostpackExplode",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"BoostStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"CameraBack",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CameraFront",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CameraLeft",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CameraOverrideStart",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"CameraOverrideStop",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"CameraRight",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CameraShake",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CancelAbort",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CardSwiped",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CastEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CastSelfStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CastTargetStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CharacterGenStart",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"ChargeDownStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ChargeHoldStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CharGenBackground",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CharGenBody",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CharGenFace",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CharGenPresets",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"CharGenTraits",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"ChargeRelease",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ChargeRelease_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ChargeRelease_RightArm",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ChargeUpStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ChargeUpStart_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ChargeUpStart_RightArm",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ChiStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ChiStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Close",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Closed",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Collect",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CollectEmpty",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CollectFull",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CoverActionsEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"CoverEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"CoverExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CriticalHitEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"CriticalHitStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"CullBone",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CullFurnitureBone",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"CullWeapons",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"DataSlateClose",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DataSlateDown",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DataSlateEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DataSlateUp",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Deactivation",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DeathBegin",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DeathBleedout",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DeathBoostpackBleedout",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DeathBoostpackStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DeathFurniture",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DeathMeleeStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DeathMovingStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DeathStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DeltaStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DeltaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Depart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Deploy",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Destruction",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DigiPicClose",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DisableBumper",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DisableCharacterController",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"disableCollision",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock_Bottom",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock_Nose",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock_Port",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock_Starboard",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Dock_Top",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Docked",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DockingBridgeClose",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DockingBridgeOpen",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DockingFinished",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DockStarborn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DodgeBackStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DodgeLeftStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DodgeRightStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Done",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DoneQ01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"dyn_Activation",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicCameraShot",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicFlavor",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicIdle",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicIdleAnimObject",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicPartialBody",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicPartialBodyIdle",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicPathing",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicPhotomode",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicPoseSwitch",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicRootEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicRootStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicScene",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicSceneHold",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"DynamicShuffle",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicTalk",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"DynamicTalkLarge",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DynamicTalkMedium",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DynamicTalkSmall",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"DynamicThreat",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicTransition",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicTraversal",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"DynamicTurn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EarlyExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"EffectSequenceFinish",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"EjectShellCasing",	{AgxEventType::Game_Event,	AgxTrueFalse::TRUE}},
	{"EnableBumper",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EnableCharacterController",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"enableCollision",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"End",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"EndAnim",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EndAnimatedCamera",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"EnterCoverCrouchingLeft",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"EnterCoverCrouchingRight",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"EnterCoverStandingLeft",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"EnterCoverStandingRight",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"EnterFromStand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"EpsilonStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"EpsilonStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Equip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EtaStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"EvadeLeftStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EvadeRightStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EvadeStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ExitCover",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ExitLoopEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ExitToDeadStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ExitToStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ExitToStandAmbush",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"EyeTrackingOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"EyeTrackingOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"FaceLightOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FaceLightOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FaceLightPowerOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FaceLightPowerOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FireAutoStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"FireAutoStart_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireAutoStart_RightArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireAutoStart_Tail",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireRelease",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"FireRelease_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireRelease_RightArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireSingleBinaryStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FireSingleStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"FireSingleStart_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireSingleStart_Mouth1",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FireSingleStart_Mouth2",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FireSingleStart_RightArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FireSingleStart_Tail",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FlavorEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FlinchEnd",	{AgxEventType::Both,	AgxTrueFalse::FALSE}},
	{"FlinchStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Flying",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FootLeft",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"FootRight",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"FootSneakLeft",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FootSneakRight",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FootSprintLeft",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"FootSprintRight",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"ForceEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ForceFaceLightOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ForceFaceLightOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ForceFurnitureExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ForceWeaponFire",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FrontExitToRun",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FrontExitToStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FrontExitToStandAmbush",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FrontExitToWalk",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FurnitureCameraReset",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FurnitureCameraStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FXAnimFinish",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FXCoolantActive",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"FXIdleOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FXLightsOff",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FXLightsOn",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FXMuzzleFlash",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FXMuzzleFlash",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"FXPlay01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FXPlay02",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"FXSprayImpactEffect",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"GammaStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"GammaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GetUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"GetUpEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"GetUpStart",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"gra",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"GravDashStart_Long",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"GravDashStart_Medium",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"GravDashStart_Short",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"GravityOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"GravityOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"GravJumpCancel",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"GravJumpExecute",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"GravJumpFinish",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"GravJumpInitiate",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"GrazeEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GrazeStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GrazeStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Green",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GunDownInterrupted",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GunDownStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"GunDownStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HalfClose01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"HalfClose02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"HalfOpen01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"HalfOpen02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"HandIKOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HandIKOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HeadLightPowerOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HeadLightPowerOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HeadTrackingOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HeadTrackingOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HelmetLightOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HelmetLightOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HelmetLightPowerOff",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HelmetLightPowerOn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HideEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HideStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HideStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"HideSubsegment",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"HitFrame",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"IdleArmCrossedEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleArtifactEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleBackTimer",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleBedSleeping",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"IdleBriefcaseEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleChairGetUp",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"IdleChairSitting",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"IdleCoffeeCupEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleCount",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"IdleCowerStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleDataSlateEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleDataSlateReceiveAndReadEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleDialogueAdditive",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleDigiPicEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleDuffleBagEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleFaceTimer",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleFlavor",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleFlavorAdditive",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleFlavorLarge",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleFlavorLargeEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleFlavorNPC",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleFlavorPartialBody",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleFlavorPlayer",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleGiveWatchStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleHandCuffedEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleHandsBehindEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleInterrupt",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdlePartialBodyEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdlePartialBodyStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdlePartialBodyStopInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdlePartialBodySwitch",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleQuantumCalibratorEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleShoppingBagEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleSitting",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"IdleStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleSurrenderEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"IdleSyncStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleSyncStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleTaunt",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleTechReader2handsBulkyEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IdleTechReader2handsEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleTechReaderEquip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IdleWoundedEquip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"InitializeCharGen",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"InitializeGunGraph",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InitializeInventory",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InitializeMeleeGraph",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InitializeMTGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InjuredBegin",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InjuredEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InjuredStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InjuredStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InstantInitializeCharGen",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"InstantInitializeGunGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InstantInitializeInventory",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InstantInitializeMeleeGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InstantInitializeMTGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InstantInitializeUnconscious",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"InterpolationMark",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"InventoryCameraBack",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryCameraBody",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryCameraFaceGen",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryCameraHead",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryCameraMain",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"InventoryCameraOutfit",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryCameraWeapon",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InventoryOpen",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"InventoryOpenDraw",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"InvertMirror",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"InvertMirrorAndCrouch",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"IotaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IotaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"IsOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"IsOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JiggleEvaluate",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JigglePitchDown",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JigglePitchUp",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JiggleRollLeft",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JiggleRollRight",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JiggleYawLeft",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JiggleYawRight",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JumpBoost",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JumpBoostBegin",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"JumpDown",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JumpFall",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JumpIdle01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JumpIdle02",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JumpLand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JumpLandAdditive",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"JumpLandStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"JumpStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JumpTraversal",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"JumpTraversalLand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"JumpUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"JunoReset",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"KappaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"KappaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"KillActor",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"KnockDownStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderClimb",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"LadderClimbDown",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderClimbIdle",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"LadderClimbLeftHandUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"LadderClimbRightHandUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"LadderClimbUp",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderEnterClimbDown",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderEnterClimbUp",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderExit",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LadderLeftHandUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"LadderRightHandUp",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LambdaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LambdaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Land",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Landed",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LandedClosed",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandedSlave",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandingFinished",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LandingFinishedFailsafe",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LandingKreet",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandingKreetPirates",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandKreetSlave",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandLarge",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandNPC",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandSlave",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"LandSlaveNPC",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"LandStarborn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandStarbornNPC",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LandVertical",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"LeftExitToRun",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LeftExitToStand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"LeftExitToWalk",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LeftQuickExitToStand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"LimbCrippleStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Link",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"LoadFurnitureGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LoadGunAssemblyGraph",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Loop01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LoopCCW01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"LoopCW01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MainTrack",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MantleCancel",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MantleEnd",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MantleHands",	{AgxEventType::Game_Event,	AgxTrueFalse::TRUE}},
	{"MantleStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MaxCumulativeAnimationsReached",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_1",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_2",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_3",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_4",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_5",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_A",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_B",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Bite01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Bite02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Bite03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Claw01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Claw02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Head01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Head02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Head03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttack_Left180",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Left90",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Right180",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Right90",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_RightArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_RightClaw01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Tail01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttack_Tail02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack03",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack04",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttack05",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackBashMiss",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttackBashStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttackFinisherStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_1",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_2",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_B",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_Bite01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_Bite02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_Claw01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_Head01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower_Tail01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower01",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPower02",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"MeleeAttackPowerStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttackPowerStartZeroG",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackSprintStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleeAttackStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"MeleeAttackStartInAir",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleeAttackStartZeroG",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MeleePowerAttack_Forward",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MeleePowerAttack_ForwardSlam",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"MinigameEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MinigameStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MuStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"MuStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"NuStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"NuStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"OffRamp",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"OmegaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"OmegaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"OmicronStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"OmicronStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"OnReloadExit",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Open",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Opened",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Orange",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"pa_KnifeAttack",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"pa_TerrormorphBiteHead",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"pa_TerrormorphBiteHead01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"pa_TerrormorphBiteHead02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"pa_TerrormorphSlamHead01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"pa_TerrormorphSlamHead02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PairedStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"ParasiteFXStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PartialBodyAnimObjUnequip",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PartialCoverStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PathingBegin",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"PathingEnd",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"PathTweenerEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PathTweenerEndTween",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PathTweenerStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PhiStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhiStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Boxer",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Cower",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Gift",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_HandsOnHips",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Hero",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Jump",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Kneeling",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_LookUp",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Love",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Propose",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_SitOnGround",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Sneak",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_ThumbsUp",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PhotoMode_Wave",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoMode_WhatsUp",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PhotoModeExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"PickNewIdle",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PipboyClosed",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PipboyOpened",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PiStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PiStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PitchOverrideEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PitchOverrideStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Play01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Play02",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Play03",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Play04",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Play05",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Play06",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayerTerminalEnter",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PlayerTerminalExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PlayImod",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"PlayLandedFX",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayLandingCamera",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"PlayLandingEngineFX",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayLandingEngineFXStatic",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayLandingFX",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"PlayReaction01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayReaction02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayReaction03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayReaction04",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayReaction05",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PlayTakeOffCamera",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"PlayTakeOffFX",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"PopToClose",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PopToOpen",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PositionChange",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Powered",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"PreHitFrame",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"PsiStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"PsiStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"QuickExitToStand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"RampClose",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RampOpen",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ReactionEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReactionStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReactionStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReadyStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ReadyState",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"RearExitToRun",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RearExitToStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RearExitToWalk",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RearQuickExitToStand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Recoil_LeftArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Recoil_RightArm",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RecoilStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Red",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReevaluateGraphState",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"RelaxedStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ReloadAbort",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReloadBegin",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ReloadComplete",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ReloadExit",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ReloadLoopStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ReloadStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ReloadState",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"ReloadTacticalStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RemoveCharacterControllerFromWorld",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Reset",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ResetCumulative",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ResurrectStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RhoStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"RhoStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"RightExitToRun",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RightExitToStand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"RightExitToWalk",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RightQuickExitToStand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"RingInner_CCW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingInner_CW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingInner_Success",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingMiddle_CCW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingMiddle_CW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingMiddle_Success",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingOuter_CCW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingOuter_CW",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RingOuter_Success",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"RootAnimOffStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"RootAnimOffStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Rotate_Long",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Rotate_Long01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Rotate_Short",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Rotate_Short01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Rotate_Single",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Rotate_Single01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateCCW_Loop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateCCW_Loop01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateCW_Loop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateCW_Loop01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateCW_Single01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RotateSimple01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RunAction",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"RunStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SceneAnimationDoneHandler",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SceneEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SceneNextEvent",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SceneNextPhase",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SceneStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SceneStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SequenceA01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA04",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA05",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA06",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceA07",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB04",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB05",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB06",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SequenceB07",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SetOrientBegin",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"SetOrientEnd",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"SetPosBegin",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"SetPosEnd",	{AgxEventType::Special,	AgxTrueFalse::BLANK}},
	{"SetupGraphRagdoll",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ShipImpactStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ShowSubsegment",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ShuffleBackward",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ShuffleEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ShuffleForward",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SightedEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SightedOverLowStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SightedOverStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SightedStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SightedState",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SightedStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SightedStopRequest",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SigmaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartFromStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"sitStartFromStandInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartFrontFromStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"sitStartFrontFromStandInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartFrontFromWalk",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"sitStartLeftFromStand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"sitStartLeftFromStandInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartLeftFromWalk",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"sitStartRearFromStand",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"sitStartRearFromStandInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartRearFromWalk",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"sitStartRightFromStand",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"sitStartRightFromStandInstant",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"sitStartRightFromWalk",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SlavedIdle",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SlavedIdleFlavor",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor02",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor03",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor04",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor05",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor06",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor07",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor08",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"SlavedIdleFlavor09",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Slide",	{AgxEventType::Game_Event,	AgxTrueFalse::TRUE}},
	{"SlideStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"SneakStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SneakState",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"SneakStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SoarStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SoarStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SoundPlay",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"SoundPlayAt",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"SoundStop",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"SpellFireLeftHand",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SpellFireRightHand",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SprintEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SprintStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SprintStartEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"SprintStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stage 2", {AgxEventType::Both,	AgxTrueFalse::TRUE	}},
	{"Stage01_Loop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage01_Off",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage01_On",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage01_Start",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage01_Stop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage02_Loop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage02_Off",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage02_On",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage02_Start",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage02_Stop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage03_Loop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage03_Off",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage03_On",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage03_Start",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage03_Stop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage04_Loop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage04_Off",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage04_On",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage04_Start",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage04_Stop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage05_Loop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage05_Off",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage05_On",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage05_Start",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage05_Stop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage1",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stage1NoTransition",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stage2",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stage2NoTransition",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stage3",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage3NoTransition",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage4",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Stage4NoTransition",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"StaggerEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::FALSE}},
	{"StaggerStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StaggerStop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StandStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"startAmbushSeq",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"StartAnimatedCamera",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StartAnimatedCameraDelta",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StartedWalking",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"startFloating",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StartImpactFX",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StartMeleeAttack",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"startRoar",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"StartSighted",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StartVoice",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"StateA_Idle",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StateA_Play",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StateB_Idle",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StateB_Play",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StateC_Idle",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StateC_Play",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StateD_Idle",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StateD_Play",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"Stop",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Stop01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StopAttackQueuingWhileReloading",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"StopEffect",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"stopFloating",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StopImod",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StopIMod",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StopImpactFX",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"StoppedWalking",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"StopSighted",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StunEnd",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StunStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"StunStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SupportAttackQueuingWhileReloading",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SwimStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SwimStart_Gun",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SwimStart_Melee",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"SwimStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SyncDeferDeath",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"SyncLeftFoot",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"SyncRightFoot",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"TakeOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TakeOff_Neon",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffE3",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffFinished",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"TakeOffNPC",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffSlave",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffStarborn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffStarbornNPC",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TakeOffVertical",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TauStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TauStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TerminateGraphRagdoll",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"TerrormorphRevealAnkylosaurusStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TerrormorphRevealStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"terrormorphSmash",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TerrormorphTransformationStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"test",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ThetaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ThetaStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ThrowGrenadeStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"ThrowInterrupt",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ThrowMineStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"to_PoseA",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"to_PoseB",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"to_PoseC",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"to_ShipDamageState01",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"to_ShipDamageState02",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"to_ShipFullHealth",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"to_ShipFullyDamagedEngine",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"to_ShipPartialHealthEngine",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Trans01",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Trans02",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Trans03",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Trans04",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TransitionComplete",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"TransitionStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TransitionStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TraversalFinished",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"TraversalStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TraversalStepDone",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TraversalStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TunnelingExit",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TunnelStart",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TunnelStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TurnColorBlue",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorDefault",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorGreen",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorOrange",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorRed",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorViolet",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorWhite",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnColorYellow",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnLeft",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TurnOff",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TurnOffFull",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TurnOn",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TurnOnFlicker",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TurnOnFull",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TurnOnPulse",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnOnPulseFast",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnOnPulseFlicker",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnOnPulseSlow",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"TurnRight",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"TurnStateDefault",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"TurnStop",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"Unassemble",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UnconsciousStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UnconsciousStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UncullBone",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"UnCullFurnitureBone",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"UncullWeapons",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"UnDock",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Undock_Bottom",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Undock_Nose",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Undock_Port",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Undock_Starboard",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"Undock_Top",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"UndockingFinished",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"UndockStarborn",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Unequip",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"UnequipComplete",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"UnequipInterrupted",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UnequipInterrupted_Death",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Unlink",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"Unlock",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Unpowered",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"UpsilonStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UpsilonStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"UseStimpak",	{AgxEventType::Game_Event,	AgxTrueFalse::TRUE}},
	{"VehicleDeath",	{AgxEventType::Graph_Event,	AgxTrueFalse::TRUE}},
	{"VehicleForwardCollisionHard",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VehicleIdleStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VehicleIdleStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VehicleJumpLand",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VehicleJumpStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot11",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot12",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot13",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot14",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot15",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot21",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"VideoOnly_Shot21b",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Violet",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"WalkStart",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"WeaponAttach",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"WeaponDraw",	{AgxEventType::Game_Event,	AgxTrueFalse::BLANK}},
	{"WeaponFire",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"WeaponFireEnd",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"WeaponFireStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"WeaponSheathe",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"WeaponSheatheInterrupt",	{AgxEventType::Graph_Event,	AgxTrueFalse::BLANK}},
	{"WeaponSwing",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"White",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"XiStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"XiStop",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"Yellow",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ZeroOutCameraPitch",	{AgxEventType::Both,	AgxTrueFalse::BLANK}},
	{"ZetaStart",	{AgxEventType::Both,	AgxTrueFalse::TRUE}},
	{"ZetaStop", { AgxEventType::Both,	AgxTrueFalse::TRUE }}
});

const QStringList AgxAnimPrefixes = {
	"Alert",
	"Analyze",
	"AnalyzeAngry",
	"Angry",
	"ArmsCrossed",
	"ArmsCrossedCasual",
	"ArmsCrossedMilitary",
	"ArmsFrontCasual",
	"ArmsFrontMilitary",
	"BleedOut",
	"Block",
	"Bored",
	"BoredLegsCrossed",
	"Boxer",
	"Captive",
	"Close",
	"Confident",
	"CoverCrouching",
	"CoverStanding",
	"Cower",
	"CrossLegs",
	"CryingStanding",
	"Depressed",
	"Drink",
	"DrinkBottle",
	"Driver",
	"Eat",
	"Executive",
	"ExecutiveHandClasp",
	"ExecutiveLean",
	"Fancy",
	"Furniture",
	"GiveWatch",
	"Graze",
	"Ground",
	"Gun",
	"HandsBehindCasual",
	"HandsBehindMilitary",
	"Hide",
	"Injured",
	"Jump",
	"Kneeling",
	"OathTaker",
	"Open",
	"Passenger",
	"PhotoModeGift",
	"PhotoModeHero",
	"PhotoModeLookUp",
	"PhotoModeLove",
	"PhotoModePropose",
	"PhotoModeThumbsUp",
	"Player",
	"Praying",
	"Read",
	"Ready",
	"Recline",
	"Relaxed",
	"RingInner",
	"RingMiddle",
	"RingOuter",
	"Sandwich",
	"Sighted",
	"SitOnGround",
	"Slouched",
	"Sneak",
	"SneakSighted",
	"Sprint",
	"StateA",
	"StateB",
	"Swim",
	"Sync",
	"Tablet",
	"Terminal",
	"TerminalRelaxed",
	"Thinking",
	"Unconscious",
	"Unpowered",
	"WhatsUp",
	"WindowShopping",
	"Writing",
	"ZeroG"
};

const QStringList AgxAnimSuffixes = {
	"Alpha",
	"ArmCrossed",
	"Artifact",
	"Back",
	"Background",
	"Backward",
	"Beta",
	"BlindFire",
	"BlindFireOver",
	"Body",
	"BookSlate",
	"Briefcase",
	"Chi",
	"CoffeeCup",
	"DataSlate",
	"DataSlateReceiveAndRead",
	"Delta",
	"DigiPic",
	"DuffleBags",
	"Epsilon",
	"Eta",
	"Face",
	"FaceGen",
	"Forward",
	"Front",
	"Gamma",
	"HandCuffed",
	"HandsBehind",
	"Head",
	"Heavy",
	"Inventory",
	"Iota",
	"Kappa",
	"Lambda",
	"Left",
	"Main",
	"Melee",
	"Mu",
	"NoteSlate",
	"Nu",
	"Off",
	"Omega",
	"Omicron",
	"On",
	"Outfit",
	"Partial",
	"PartialOverLow",
	"Phi",
	"Pi",
	"Pistol",
	"PoseB",
	"PoseC",
	"Presets",
	"Psi",
	"QuantumCalibrator",
	"Rho",
	"Rifle",
	"Right",
	"Run",
	"ShoppingBag",
	"Sighted",
	"SightedOver",
	"SightedOverLow",
	"Sigma",
	"Stand",
	"Stun",
	"Surrend",
	"Tactical",
	"Tau",
	"TechReader",
	"TechReader2hands",
	"TechReader2handsBulky",
	"Theta",
	"Traits",
	"Traits",
	"Upsilon",
	"Walk",
	"Weapon",
	"Wounded",
	"Xi",
	"ZeroG",
	"Zeta"
};

const QStringList AgxAnimTriggers = {
	"FootLeft",
	"ReloadStart",
	"DynamicIdle",
	"JumpStart",
	"JumpFall",
	"JumpLand",
	"SprintStart",
	"FireSingle",
	"FireAuto",
	"AttackEnd",
	"AttackRelease",
	"Equip",
	"UnEquip",
	"ReloadEnd",
	"ActionEnd",
	"MoveStart",
	"MoveStop",
	"StartedWalking",
	"StoppedWalking",
	"TurnRight",
	"TurnLeft",
	"TurnStop",
	"JumpLandStop",
	"sitStartLeftFromStand",
	"sitStartLeftFromWalk",
	"sitStartRearFromWalk",
	"sitStartFrontFromWalk",
	"sitStartRightFromStand",
	"sitStartRightFromWalk",
	"QuickExitToStand",
	"furnitureDeath",
	"idleStop",
	"dyn_Activation",
	"ReadyStart",
	"RelaxedStart",
	"SneakStart"
};

const QStringList AgxSyncs = {
	"SyncBoltCharge",
	"SyncCastPower",
	"SyncChargeDown",
	"SyncChargeHold",
	"SyncChargeUp",
	"SyncEnterFurniture",
	"SyncEquip",
	"SyncFireAuto",
	"SyncFireSingle",
	"SyncFootsteps",
	"SyncGunActions",
	"SyncIdle",
	"SyncIdle",
	"SyncJumpBoost",
	"SyncJumpFall",
	"SyncJumpFallLoop",
	"SyncJumpLand",
	"SyncJumpLandCamera",
	"SyncJumpStart",
	"SyncMeleeAttack",
	"SyncMeleeAttackPower",
	"SyncReload",
	"SyncReloadEnd",
	"SyncReloadLoop",
	"SyncSlide",
	"SyncStandToRun",
	"SyncThrowGrenade",
	"SyncThrowMine",
	"SyncTurning",
	"SyncUnequip"
};

size_t AgxAnimationFlags::GetValue() const {
	size_t output = 0;
	std::memcpy(&output, &data, sizeof(output));
	return output;
}

void AgxAnimationFlags::SetValue(size_t input) { 
	std::memcpy(&data, &input, sizeof(input)); 
}

QStringList AgxAnimationFlags::GetStringList(bool spaces)
{
	QString spcr = spaces ? " " : "_";
	QStringList list = {
		"Additive",
		"Adjust"+spcr+"1st"+spcr+"Person"+spcr+"FOV",
		"Allow"+spcr+"Rotation",
		"Animation"+spcr+"Driven",
		"Block"+spcr+"POV"+spcr+"Switch",
		"Blocking",
		"Cumulative"+spcr+"Rotation",
		"Disable"+spcr+"Foot"+spcr+"IK",
		"Disable"+spcr+"Interpolation",
		"Equip"+spcr+"Not"+spcr+"OK",
		"Force"+spcr+"Copy"+spcr+"Bone"+spcr+"Weights",
		"Force"+spcr+"Idle"+spcr+"Stop",
		"Full"+spcr+"Animation"+spcr+"No"+spcr+"Physics",
		"Graph"+spcr+"Supports"+spcr+"Strafe",
		"Is"+spcr+"Attack"+spcr+"Not"+spcr+"Ready",
		"Is"+spcr+"Busy",
		"Is"+spcr+"In"+spcr+"Flavor",
		"Is"+spcr+"Jumping",
		"Is"+spcr+"Reloading",
		"Is"+spcr+"Sprinting",
		"Is"+spcr+"Synced",
		"Manual"+spcr+"Graph"+spcr+"Change",
		"Motion"+spcr+"Driven",
		"No"+spcr+"IK",
		"Pathing"+spcr+"Interruptible"+spcr+"Idle",
		"Pipboy"+spcr+"Up",
		"Render"+spcr+"1st"+spcr+"Person"+spcr+"in"+spcr+"World",
		"Should"+spcr+"Pivot"+spcr+"to"+spcr+"Camera",
		"Speed"+spcr+"Synced",
		"Supported"+spcr+"Death"+spcr+"Anim",
		"Turn"+spcr+"Animation",
		"Want"+spcr+"Sprint",
		"Wants"+spcr+"Full"+spcr+"Ragdoll"
	};
	return list;
}

QString AgxAnimationFlags::ToString() const
{
	QString output;// = "AgxAnimationFlags {";
	output += "Additive: ";
	output += data.Additive ? "True" :  "False";
	output += "\nAdjust 1st Person FOV : ";
	output += data.Adjust_1st_Person_FOV ? "True" :  "False";
	output += "\nAllow Rotation: ";
	output += data.Allow_Rotation ? "True" :  "False";
	output += "\nAnimation Driven: ";
	output += data.Animation_Driven ? "True" :  "False";
	output += "\nBlock POV Switch: ";
	output += data.Block_POV_Switch ? "True" :  "False";
	output += "\nBlocking: ";
	output += data.Blocking ? "True" :  "False";
	output += "\nCumulative Rotation: ";
	output += data.Cumulative_Rotation ? "True" :  "False";
	output += "\nDisable Foot IK: ";
	output += data.Disable_Foot_IK ? "True" :  "False";
	output += "\nDisable Interpolation: ";
	output += data.Disable_Interpolation ? "True" :  "False";
	output += "\nEquip Not OK: ";
	output += data.Equip_Not_OK ? "True" :  "False";
	output += "\nForce Copy Bone Weights: ";
	output += data.Force_Copy_Bone_Weights ? "True" :  "False";
	output += "\nForce Idle Stop: ";
	output += data.Force_Idle_Stop ? "True" :  "False";
	output += "\nFull Animation No Physics: ";
	output += data.Full_Animation_No_Physics ? "True" :  "False";
	output += "\nGraph Supports Strafe: ";
	output += data.Graph_Supports_Strafe ? "True" :  "False";
	output += "\nIs Attack Not Ready: ";
	output += data.Is_Attack_Not_Ready ? "True" :  "False";
	output += "\nIs Busy: ";
	output += data.Is_Busy ? "True" :  "False";
	output += "\nIs In Flavor: ";
	output += data.Is_In_Flavor ? "True" :  "False";
	output += "\nIs Jumping: ";
	output += data.Is_Jumping ? "True" :  "False";
	output += "\nIs Reloading: ";
	output += data.Is_Reloading ? "True" :  "False";
	output += "\nIs Sprinting: ";
	output += data.Is_Sprinting ? "True" :  "False";
	output += "\nIs Synced: ";
	output += data.Is_Synced ? "True" :  "False";
	output += "\nManual Graph Change: ";
	output += data.Manual_Graph_Change ? "True" :  "False";
	output += "\nMotion Driven: ";
	output += data.Motion_Driven ? "True" :  "False";
	output += "\nNo IK: ";
	output += data.No_IK ? "True" :  "False";
	output += "\nPathing Interruptible Idle: ";
	output += data.Pathing_Interruptible_Idle ? "True" :  "False";
	output += "\nPipboy Up: ";
	output += data.Pipboy_Up ? "True" :  "False";
	output += "\ndata.Render 1st Person in World: ";
	output += data.Render_1st_Person_in_World ? "True" :  "False";
	output += "\nShould Pivot to Camera: ";
	output += data.Should_Pivot_to_Camera ? "True" :  "False";
	output += "\nSpeed Synced: ";
	output += data.Speed_Synced ? "True" :  "False";
	output += "\nSupported Death Anim: ";
	output += data.Supported_Death_Anim ? "True" :  "False";
	output += "\nTurn Animation: ";
	output += data.Turn_Animation ? "True" :  "False";
	output += "\nWant Sprint: ";
	output += data.Want_Sprint ? "True" :  "False";
	output += "\nWants Full Ragdoll: ";
	output += data.Wants_Full_Ragdoll ? "True" :  "False";
	//output += "}: ";

	return output;
}

QJsonValue AgxAnimationFlags::ToJson() const
{
	QJsonArray array;
	size_t copy = GetValue();
	QStringList list = GetStringList(false);
	for (size_t i = 0; i < sizeof(copy)*8; i++)
	{
		if (copy & 0b1) {
			array.append(list.at(i));
		}
		copy >>= 1;
	}
	return array;
}

void AgxAnimationFlags::FromJson(const QJsonValue& data)
{
	QJsonArray array = data.toArray();
		this->data.Additive = array.contains("Additive");
		this->data.Adjust_1st_Person_FOV = array.contains("Adjust_1st_Person_FOV");
		this->data.Allow_Rotation = array.contains("Allow_Rotation");
		this->data.Animation_Driven = array.contains("Animation_Driven");
		this->data.Block_POV_Switch = array.contains("Block_POV_Switch");
		this->data.Blocking = array.contains("Blocking");
		this->data.Cumulative_Rotation = array.contains("Cumulative_Rotation");
		this->data.Disable_Foot_IK = array.contains("Disable_Foot_IK");
		this->data.Disable_Interpolation = array.contains("Disable_Interpolation");
		this->data.Equip_Not_OK = array.contains("Equip_Not_OK");
		this->data.Force_Copy_Bone_Weights = array.contains("Force_Copy_Bone_Weights");
		this->data.Force_Idle_Stop = array.contains("Force_Idle_Stop");
		this->data.Full_Animation_No_Physics = array.contains("Full_Animation_No_Physics");
		this->data.Graph_Supports_Strafe = array.contains("Graph_Supports_Strafe");
		this->data.Is_Attack_Not_Ready = array.contains("Is_Attack_Not_Ready");
		this->data.Is_Busy = array.contains("Is_Busy");
		this->data.Is_In_Flavor = array.contains("Is_In_Flavor");
		this->data.Is_Jumping = array.contains("Is_Jumping");
		this->data.Is_Reloading = array.contains("Is_Reloading");
		this->data.Is_Sprinting = array.contains("Is_Sprinting");
		this->data.Is_Synced = array.contains("Is_Synced");
		this->data.Manual_Graph_Change = array.contains("Manual_Graph_Change");
		this->data.Motion_Driven = array.contains("Motion_Driven");
		this->data.No_IK = array.contains("No_IK");
		this->data.Pathing_Interruptible_Idle = array.contains("Pathing_Interruptible_Idle");
		this->data.Pipboy_Up = array.contains("Pipboy_Up");
		this->data.Render_1st_Person_in_World = array.contains("Render_1st_Person_in_World");
		this->data.Should_Pivot_to_Camera = array.contains("Should_Pivot_to_Camera");
		this->data.Speed_Synced = array.contains("Speed_Synced");
		this->data.Supported_Death_Anim = array.contains("Supported_Death_Anim");
		this->data.Turn_Animation = array.contains("Turn_Animation");
		this->data.Want_Sprint = array.contains("Want_Sprint");
		this->data.Wants_Full_Ragdoll = array.contains("Wants_Full_Ragdoll");
}

void AgxAnimationFlags::ToXML(pugi::xml_node& parent)
{
	size_t copy = GetValue();
	QStringList list = GetStringList(true);
	for (size_t i = 0; i < sizeof(copy) * 8; i++)
	{
		if (copy & 0b1) {
			auto flag = AgxAppendValue(parent, "flag", list.at(i), AgxFormat::None, 0);
		}
		copy >>= 1;
	}
}

void AgxAnimationFlags::FromXML(pugi::xml_node& node)
{
	size_t value = 0;
	QStringList list = GetStringList(true);
	int8_t idx = -1;
	for (pugi::xml_node flag = node.child("flag"); flag;)
	{
		pugi::xml_node next = flag.next_sibling("flag");
		idx = list.indexOf(flag.child_value(), Qt::CaseInsensitive);
		if (idx > -1 && idx < 64)
			value |= (1ULL << idx);

		node.remove_child(flag);

		flag = next;
	}

	SetValue(value);
}

#pragma region Property Block Data
AgxPropertyBlockData::AgxPropertyBlockData(const QList<AgxPropertyEntryDefinition>& types, QObject* parent) : QObject(parent), _columnDefinitions(types)
{}
AgxPropertyBlockData::AgxPropertyBlockData(const AgxPropertyBlockData& other) : AgxPropertyBlockData(other._columnDefinitions, other.parent())
{
	_data = other._data;
	propertyEnabled = other.propertyEnabled;
}
AgxPropertyBlockData& AgxPropertyBlockData::operator=(const AgxPropertyBlockData& other)
{
	setParent(other.parent());
	_columnDefinitions = other._columnDefinitions;
	_data = other._data;
	propertyEnabled = other.propertyEnabled;
	return *this;
}
void AgxPropertyBlockData::AddRow(int index)
{
	if (index < 0) return;

	index = index >= _data.size() ? _data.size() : index;

	_data.insert(index, GetDefaultRow());

	Q_EMIT RowAdded(index);
}

void AgxPropertyBlockData::RemoveRow(int index)
{
	if (index < 0) return;

	index = index >= _data.size() ? _data.size() : index;

	_data.remove(index);

	Q_EMIT RowRemoved(index);
}

bool AgxPropertyBlockData::HasRow(int index) const
{
	if (index < _data.size()) return true;

	return false;
}

void AgxPropertyBlockData::Reset()
{
	int count = GetRowCount();
	if (count <= 0) return;

	_data.clear();
	for (int i = 0; i < count; i++) {
		Q_EMIT RowRemoved(i);
	}
}

unsigned int AgxPropertyBlockData::GetColumnCount() const
{
	return _columnDefinitions.size();
}

void AgxPropertyBlockData::insertPropertyBlockData(const QJsonObject& data)
{
	unsigned int inputSize = 0;
	for (int i = 0; i < data.size(); i++)
	{
		bool ok;
		int num = data.keys().at(i).toInt(&ok);
		if (ok) {
			inputSize = num+1 > inputSize ? num+1 : inputSize;
		}
	}

	while (GetRowCount() < inputSize) AddRow(inputSize);

	for (int i = 0; i < _data.size(); i++)
	{	
		for (int j = 0; j < _data.at(i).size(); j++)
		{
			if (data.contains(std::to_string(i).c_str()))
			{
				if (data[std::to_string(i).c_str()].toObject().contains(std::to_string(j).c_str()))
				{
					QJsonObject entry = data[std::to_string(i).c_str()].toObject()[std::to_string(j).c_str()].toObject();
					AgxPropertyBlockData::Entry output;
					output.Type = GetAgxColumnTypeFromString(entry["type"].toString());
					output.Value = entry["value"].toString();
					_data[i][j] = output;
				}
			}
		}
	}
	Q_EMIT DataUpdated();
}

QJsonObject AgxPropertyBlockData::getPropertyBlockData(bool cleared) const
{
	const auto& data = cleared ? QList<QList<AgxPropertyBlockData::Entry>>(_data.size(), GetDefaultRow()) : _data;

	QJsonObject output;
	for (int i = 0; i < data.size(); i++)
	{
		QJsonObject row;
		for (int j = 0; j < data.at(i).size(); j++)
		{
			QJsonObject entry;
			entry["value"] = data.at(i).at(j).Value;
			
			if(GetColumnType(i) == AgxColumnTypes::BasicMultiVar || GetColumnType(i) == AgxColumnTypes::CustomMultiVar)
			{
				entry["type"] = GetAgxColumnTypeAsString(data.at(i).at(j).Type);
			}

			row[std::to_string(j).c_str()] = entry;
		}
		output[std::to_string(i).c_str()] = row;
	}
	return output;
}

void AgxPropertyBlockData::loadDefault(const QJsonObject& blockData)
{
	int rowCount = blockData.size();
	for (int i = 0; i < blockData.size(); i++)
	{
		rowCount = rowCount > blockData.keys().at(i).toInt() ? rowCount : blockData.keys().at(i).toInt() + 1;
	}

	QList<QStringList> holder(rowCount, QStringList(_columnDefinitions.size()));

	for (int i = 0; i < holder.size(); i++)
	{
		AddRow(i);
	}
}

void AgxPropertyBlockData::SetEnabledState(bool state)
{
	propertyEnabled = state;
	Q_EMIT StateUpdated(propertyEnabled);
}

void AgxPropertyBlockData::load(pugi::xml_node& blockNode)
{
	/*for (int i = 0; i < GetRowCount(); i++) {
		RemoveRow(0);
	}*/

	Reset();

	for (auto& row : blockNode.children("row")) {
		int column = 0;
		AddRow(GetRowCount());
		auto rowDef = GetDefaultRow();
		for (auto& prop : row.children("prop")) {
			if (GetColumnType(column) == AgxColumnTypes::CustomMultiVar) {
				rowDef[column].Type = GetAgxCustomVarType(prop.child_value("type"));
			} else if (GetColumnType(column) == AgxColumnTypes::BasicMultiVar) {
				rowDef[column].Type = GetAgxBasicVarType(prop.child_value("type"));
			}
			rowDef[column].Value = prop.child_value("value");
			column++;
		}
		SetRow(GetRowCount() - 1, rowDef);
		//propertySheet.remove_child("row");
	}
}

QList<AgxPropertyBlockData::Entry> AgxPropertyBlockData::GetDefaultRow() const
{
	QList<AgxPropertyBlockData::Entry> list;
	for (auto& type : _columnDefinitions)
	{
		AgxPropertyBlockData::Entry entry;
		entry.Value = type.value;
		if (type.columnType == AgxColumnTypes::CustomMultiVar) {
			entry.Type = AgxColumnTypes::CustomInteger;
		} else if (type.columnType == AgxColumnTypes::BasicMultiVar) {
			entry.Type = AgxColumnTypes::BasicInteger;
		} else {
			entry.Type = type.columnType;
		}
		list.append(entry);
	}
	return list;
}

void AgxPropertyBlockData::SetRow(int index, const QList<Entry>& data)
{
	if (index < 0 || index >= _data.size()) return;

	if (data.isEmpty())
		_data[index] = GetDefaultRow();
	else
		_data[index] = data;
}

void AgxPropertyBlockData::SetRow(int index, const QStringList& data)
{
	if (index < 0 || index >= _data.size()) return;

	QList<Entry> input = GetDefaultRow();
	auto count = data.size() > input.size() ? input.size() : data.size();

	for (qsizetype i = 0; i < count; i++) {
		input[i].Value = data.at(i);
	}

	_data[index] = input;
}

QList<AgxPropertyBlockData::Entry>* AgxPropertyBlockData::GetRow(int index)
{
	if (index < 0 || index >= _data.size()) return nullptr;

	return &_data[index];
}

const QList<AgxPropertyBlockData::Entry>* AgxPropertyBlockData::GetRow(int index) const
{
	if (index < 0 || index >= _data.size()) return nullptr;

	return &_data[index];
}

unsigned int AgxPropertyBlockData::GetRowCount() const
{
	return _data.size();
}

AgxColumnTypes AgxPropertyBlockData::GetColumnType(int column) const
{
	if (column < 0 || column >= _columnDefinitions.size()) return AgxColumnTypes::Default;

	return _columnDefinitions.at(column).columnType;
}

AgxPropertyEntryDefinition AgxPropertyBlockData::GetColumnDefinition(int column) const
{
	if (column < 0 || column >= _columnDefinitions.size()) return AgxPropertyEntryDefinition(&AgxDictionary::ErrorTerm, "", AgxColumnTypes::Default);

	return _columnDefinitions.at(column);
}
#pragma endregion

QString AgxGameTypeToString(const AgxGameType& type)
{
	switch (type)
	{
		case AgxGameType::SFBGS: return "STARFIELD";
		default:
			return "UNDEFINED";
	}
}

AgxGameType AgxGameTypeFromString(const QString& string)
{
	if (	_stricmp(string.toStdString().c_str(), "STARFIELD"	) ==0 
		||	_stricmp(string.toStdString().c_str(), "SFBGS"		) == 0 
		||	_stricmp(string.toStdString().c_str(), "SF1"		) == 0) return AgxGameType::SFBGS;

	return AgxGameType::None;
}

QString AgxFileTypeToString(const AgxFileType& type)
{
	switch (type)
	{
		case AgxFileType::BehaviorFile:
			return "Behavior Graph";
		case AgxFileType::AnimationFile:
			return "Animation File";
		case AgxFileType::AnimationComponent:
			return "Animation Component";
		case AgxFileType::RigFile:
			return "Rig File";
		default:
			return "UNKNOWN";
	}
}

AgxFileType AgxFileTypeFromString(const QString& str)
{
	QString text = str;
	text.remove(" ");

	if (text.compare("BehaviorGraph",Qt::CaseInsensitive) == 0 || text.compare("Behavior",Qt::CaseInsensitive) == 0)
		return AgxFileType::BehaviorFile;
	if (text.compare("AnimationFile", Qt::CaseInsensitive) == 0 || text.compare("Animation", Qt::CaseInsensitive) == 0)
		return AgxFileType::AnimationFile;
	if (text.compare("AnimationComponentFile",Qt::CaseInsensitive) == 0 || text.compare("AnimationComponent", Qt::CaseInsensitive)==0)
		return AgxFileType::AnimationComponent;
	if (text.compare("Rig", Qt::CaseInsensitive) == 0 || text.compare("RigFile", Qt::CaseInsensitive) == 0)
		return AgxFileType::RigFile;

	return AgxFileType::UNKNOWN;
}

QString GetAgxColumnTypeAsString(AgxColumnTypes type)
{
	switch (type)
	{
		case AgxColumnTypes::BasicString: return "BasicString";
		case AgxColumnTypes::BasicInteger: return "BasicInteger";
		case AgxColumnTypes::BasicFloat: return "BasicFloat";
		case AgxColumnTypes::BasicVector: return "BasicVector";
		case AgxColumnTypes::BasicBool: return "BasicBool";
		case AgxColumnTypes::BasicMultiVar: return "BasicMultiVar";
		case AgxColumnTypes::CustomDropDown: return "CustomDropDown";
		case AgxColumnTypes::CustomInteger: return "CustomInteger";
		case AgxColumnTypes::CustomFloat: return "CustomFloat";
		case AgxColumnTypes::CustomVector: return "CustomVector";
		case AgxColumnTypes::CustomBool: return "CustomBool";
		case AgxColumnTypes::CustomMultiVar: return "CustomMultiVar";
		case AgxColumnTypes::Event: return "Event";
		case AgxColumnTypes::Prefix: return	"Prefix";
		case AgxColumnTypes::Suffix: return	"Suffix";
		case AgxColumnTypes::State: return "State";
		case AgxColumnTypes::Action: return "Action";
		case AgxColumnTypes::SyncSystem: return "SyncSystem";
		//case AgxColumnTypes::CommentBox: return "CommentBox";
		default: return "Default";
	}
}

AgxColumnTypes GetAgxColumnTypeFromString(QString string)
{
	if (string == "BasicString")
		return AgxColumnTypes::BasicString;
	else if (string == "BasicInteger" || string == "BasicInt")
		return AgxColumnTypes::BasicInteger;
	else if (string == "BasicFloat")
		return AgxColumnTypes::BasicFloat;
	else if (string == "BasicVector" || string == "BasicVec")
		return AgxColumnTypes::BasicVector;
	else if (string == "BasicBool" || string == "BasicBoolean")
		return AgxColumnTypes::BasicBool;
	else if (string == "BasicMultiVar")
		return AgxColumnTypes::BasicMultiVar;
	else if (string == "CustomDropDown")
		return AgxColumnTypes::CustomDropDown;
	else if (string == "CustomInteger" || string == "CustomInt")
		return AgxColumnTypes::CustomInteger;
	else if (string == "CustomFloat")
		return AgxColumnTypes::CustomFloat;
	else if (string == "CustomVector" || string == "CustomVec")
		return AgxColumnTypes::CustomVector;
	else if (string == "CustomBool" || string == "CustomBoolean")
		return AgxColumnTypes::CustomBool;
	else if (string == "CustomMultiVar")
		return AgxColumnTypes::CustomMultiVar;
	else if (string == "Event")
		return AgxColumnTypes::Event;
	else if (string == "Prefix")
		return AgxColumnTypes::Prefix;
	else if (string == "Suffix")
		return AgxColumnTypes::Suffix;
	else if (string == "State")
		return AgxColumnTypes::State;
	else if (string == "Action")
		return AgxColumnTypes::Action;
	else if (string == "SyncSystem")
		return AgxColumnTypes::SyncSystem;
	//else if ("CommentBox")
		//return AgxColumnTypes::CommentBox;

	return AgxColumnTypes::Default;

}

AgxColumnTypes GetAgxCustomVarType(QString value)
{
	bool ok = false;
	int iVal = value.toInt(&ok);
	if (!ok) iVal = -1;

	switch (iVal)
	{
		case 0: return AgxColumnTypes::CustomInteger;
		case 1: return AgxColumnTypes::CustomFloat;
		case 3: return AgxColumnTypes::CustomBool;
		case 4: return AgxColumnTypes::CustomVector;
		default:
			return AgxColumnTypes::BasicString;
	}
}

AgxColumnTypes GetAgxBasicVarType(QString value)
{
	bool ok = false;
	int iVal = value.toInt(&ok);
	if (!ok) iVal = -1;

	switch (iVal)
	{
		case 0: return AgxColumnTypes::BasicInteger;
		case 1: return AgxColumnTypes::BasicFloat;
		case 3: return AgxColumnTypes::BasicBool;
		case 4: return AgxColumnTypes::BasicVector;
		default:
			return AgxColumnTypes::BasicString;
	}
}

QPair<QString, AgxVarType> GetSFBGSVarTypeFromColumnType(AgxColumnTypes type)
{
	switch (type)
	{
		case AgxColumnTypes::BasicInteger:
		case AgxColumnTypes::CustomInteger:
			return { "Integer", AgxVarType::Integer };
		case AgxColumnTypes::BasicFloat:
		case AgxColumnTypes::CustomFloat:
			return { "Float", AgxVarType::Float };
		case AgxColumnTypes::BasicBool:
		case AgxColumnTypes::CustomBool:
			return { "Bool", AgxVarType::Boolean };
		case AgxColumnTypes::BasicVector:
		case AgxColumnTypes::CustomVector:
			return { "Vector", AgxVarType::Vector };
		case AgxColumnTypes::BasicMultiVar:
		case AgxColumnTypes::CustomMultiVar:
			return { "Any", AgxVarType::Any};
		case AgxColumnTypes::CustomDropDown:
		case AgxColumnTypes::Event:
		case AgxColumnTypes::Prefix:
		case AgxColumnTypes::Suffix:
		case AgxColumnTypes::State:
		case AgxColumnTypes::Action:
		case AgxColumnTypes::SyncSystem:
		default:
			return { "String", AgxVarType::String };
	}
}

AgxVarType GetAgxVarTypeFromSFBGS(QString value)
{
	bool ok = false;
	uint8_t iVal = value.toInt(&ok);
	if (!ok)
		iVal = 0xFF;
	return GetAgxVarTypeFromSFBGS(iVal);
}

AgxVarType GetAgxVarTypeFromSFBGS(int value)
{
	switch (value)
	{
		case 0:
			return AgxVarType::Integer;
		case 1:
			return AgxVarType::Float;
		case 2:
			return AgxVarType::String;
		case 3:
			return AgxVarType::Boolean;
		case 4:
			return AgxVarType::Vector;
		case 5:
			return AgxVarType::Any;
		default:
			return AgxVarType::Undefined;
	}
}

QString AgxGraphTypeAsString(const AgxGraphType& type)
{
	switch (type)
	{
		case AgxGraphType::SFBGS_Default: return "Default";
		case AgxGraphType::SFBGS_StateMachine: return "State Machine";
		default:
			return "UNDEFINED";
	}
}

AgxGraphType AgxGetDefaultGraphType(const AgxGameType& type)
{
	switch (type)
	{
		case AgxGameType::SFBGS: return AgxGraphType::SFBGS_Default;
		default:
			return AgxGraphType::UNDEFINED;
	}
}

QString GetPropertyValue(const QVector<AgxPropertyEntryDefinition>& list, const QString& tag, const QString& defValue)
{
	for (auto& prop : list) {
		if (prop.Tag() == tag) {
			return prop.value;
		}
	}
	return defValue;
}

bool SetPropertyValue(QVector<AgxPropertyEntryDefinition>& list, const QString& tag, const QString& value)
{
	for (auto& prop : list) {
		if (prop.Tag() == tag) {
			prop.value = value;
			return true;
		}
	}
	return false;
}

QString ShortenString(const QString& string, int maxLength)
{
	if (string.length() <= maxLength)
	{
		return string;
	}

	return string.left(qMax(0, maxLength - 3)) + "...";
}
