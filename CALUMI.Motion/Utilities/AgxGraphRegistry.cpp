#include "stdafx.h"
#include "AgxGraphRegistry.h"

AgxGraphRegistry::AgxGraphRegistry()
{
	_graphDefs.insert(AgxGraphType::SFBGS_Default, SFBGS_BuildGraph_BlendTree());
	_graphDefs.insert(AgxGraphType::SFBGS_StateMachine, SFBGS_BuildGraph_StateMachine());

}

AgxGraphDefinition AgxGraphRegistry::GetGraphDefinition(const AgxGraphType& graphType)
{
	if (!_graphDefs.contains(graphType))
		return AgxGraphDefinition();

	return _graphDefs[graphType];
}

AgxGraphDefinition AgxGraphRegistry::SFBGS_BuildGraph_BlendTree()
{
	AgxGraphDefinition output;

	output._tags.insert("Link_Style", "0");

	output._defaultEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::BlendTreeName, "", AgxColumnTypes::BasicString));

	AgxPropertyBlockData swapGraph({ 
		AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::Event,"",AgxColumnTypes::Event), 
		AgxPropertyEntryDefinition(&AgxDictionary::BlendTime,"0",AgxColumnTypes::BasicFloat)}, nullptr);

	output._defaultBlocks.insert(&AgxDictionary::SwapGraphEvents, swapGraph);
	
	output._blockOrder = { &AgxDictionary::SwapGraphEvents };

	return output;
}

AgxGraphDefinition AgxGraphRegistry::SFBGS_BuildGraph_StateMachine()
{
	AgxGraphDefinition output;

	output._tags.insert("Link_Style", "1");

	output._hasDefault = true;

	output._defaultEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartSelectionMethod, AgxDictionary::Default().tag, AgxColumnTypes::CustomDropDown, {&AgxDictionary::Default,&AgxDictionary::Sync,&AgxDictionary::Variable}));
	output._defaultEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::StartSelectionVariable, "", AgxColumnTypes::CustomInteger));
	output._defaultEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::MaintainSyncSelectionVariableWhenExiting, "False", AgxColumnTypes::BasicBool));
	output._defaultEntries.push_back(AgxPropertyEntryDefinition(&AgxDictionary::UpdateWhileInactive, "False", AgxColumnTypes::BasicBool));

	AgxPropertyBlockData states({
		AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::StateName,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::NodeID,"0",AgxColumnTypes::BasicInteger) }, nullptr);

	AgxPropertyBlockData enterEvents({
		AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::NonInstanced,"False",AgxColumnTypes::BasicBool),
		AgxPropertyEntryDefinition(&AgxDictionary::Event,"",AgxColumnTypes::Event),
		AgxPropertyEntryDefinition(&AgxDictionary::Payload,"",AgxColumnTypes::BasicString) }, nullptr);

	AgxPropertyBlockData exitEvents({
		AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::NonInstanced,"False",AgxColumnTypes::BasicBool),
		AgxPropertyEntryDefinition(&AgxDictionary::Event,"",AgxColumnTypes::Event),
		AgxPropertyEntryDefinition(&AgxDictionary::Payload,"",AgxColumnTypes::BasicString) }, nullptr);

	AgxPropertyBlockData swapGraph({
		AgxPropertyEntryDefinition(&AgxDictionary::BlankEntry,"",AgxColumnTypes::BasicString),
		AgxPropertyEntryDefinition(&AgxDictionary::Event,"False",AgxColumnTypes::Event),
		AgxPropertyEntryDefinition(&AgxDictionary::BlendTime,"0",AgxColumnTypes::BasicFloat) }, nullptr);

	output._defaultBlocks.insert(&AgxDictionary::States, states);
	output._defaultBlocks.insert(&AgxDictionary::EnterEvents, enterEvents);
	output._defaultBlocks.insert(&AgxDictionary::ExitEvents, exitEvents);
	output._defaultBlocks.insert(&AgxDictionary::SwapGraphEvents, swapGraph);

	output._blockOrder = { &AgxDictionary::States, &AgxDictionary::EnterEvents, &AgxDictionary::ExitEvents, &AgxDictionary::SwapGraphEvents };

	return output;
}
