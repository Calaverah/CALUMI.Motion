#include "stdafx.h"
#include "AgxNodeRegistry.h"
#include "AgxNodes/AgxNodes"
#include "AgxNodeDelegateModelRegistry.h"

AgxNodeRegistry::AgxNodeRegistry()
{
	Default_BuildRegistry();
	
	SFBGS_BuildRegistry();
}

std::shared_ptr<AgxNodeDelegateModelRegistry>& AgxNodeRegistry::GetRegistry(AgxGameType type)
{
	switch (type)
	{
		case AgxGameType::SFBGS:
			if (!_sfbgsRegistry.get())
				SFBGS_BuildRegistry();
			return _sfbgsRegistry;
		default:
			if (!_defaultRegistry.get())
				Default_BuildRegistry();

			return _defaultRegistry;
	}
}

void AgxNodeRegistry::Default_BuildRegistry()
{
	_defaultRegistry = std::make_shared<AgxNodeDelegateModelRegistry>();

	_defaultRegistry->registerModel<AgxCommentNode>("Extra");
}

void AgxNodeRegistry::SFBGS_BuildRegistry()
{
	_sfbgsRegistry = std::make_shared<AgxNodeDelegateModelRegistry>();
	_sfbgsRegistry->registerModel<AgxCommentNode>("Extra");

#ifdef DEBUGBUILD
	_sfbgsRegistry->registerModel<SFBGS::AgxNtDebugNode>("Debug");
#endif

	_sfbgsRegistry->registerModel<SFBGS::AgxNtAnimationNode>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRandomAnimationNode>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtDynamicAnimation>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMomentumAnimation>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtPathingAnimations>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtParticleSequence>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtGamebryoSequence>("Animations");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtEffectSequence>("Animations");

	_sfbgsRegistry->registerModel<SFBGS::AgxNtGraphRef>("Graphs");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtSwapGraph>("Graphs");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtDynamicGraphReference>("Graphs");

	_sfbgsRegistry->registerModel<SFBGS::AgxNtEmbeddedGraph>("Embedded");

	_sfbgsRegistry->registerModel<SFBGS::AgxNtBlendNode>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtEventController>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtPhysicsContactListener>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtPairedAnimation>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtEveryNEventsModifier>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtTwoBoneIK>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtNormalizeRotation>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtVariableCombiner>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtCurvedPathBlender>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtSpeedScale>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtFootIK>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtLocomotionBlend>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtAnimationImpactSelector>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtTranslationAdjustment>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMultiFootIK>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtDualDirectAt>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtDirectAt>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtLinearVariable>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtTimerEvent>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtSetPos>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtSetOrient>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtEvaluateConditionVariable>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRigSwitch>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMassSpringDampenVariable>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRagdollGetUp>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRootTwist>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtStateVariableControl>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtStaggerMeter>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtConvertBoneDataToVariables>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtClonePose>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtAssignIState>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtCopyBoneWeights>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtAssignVariable>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMirrorModifier>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtSwitchNode>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMergeNode>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtTagPropagation>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtCumulativeAnimation>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRotationVariable>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtBoneConstraint>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtPostBoneModifierControl>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtRagdollDrive>("Utilities");
	_sfbgsRegistry->registerModel<SFBGS::AgxNtMomentumSwitchback>("Utilities");
}
