#pragma once

#pragma warning(push,0)
//#include "QtNodes/internal/NodeDelegateModelRegistry.hpp
#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"

class AgxGraphRegistry
{
private:
	AgxGraphRegistry();

	~AgxGraphRegistry() = default;

public:
	AgxGraphRegistry(const AgxGraphRegistry&) = delete;
	AgxGraphRegistry& operator=(const AgxGraphRegistry&) = delete;

	AgxGraphRegistry(AgxGraphRegistry&&) = delete;
	AgxGraphRegistry& operator=(AgxGraphRegistry&&) = delete;

	static AgxGraphRegistry& GetInstance() {
		static AgxGraphRegistry instance;
		return instance;
	}

	AgxGraphDefinition GetGraphDefinition(const AgxGraphType& graphType);

private:
	QHash<AgxGraphType, AgxGraphDefinition> _graphDefs;

	AgxGraphDefinition SFBGS_BuildGraph_BlendTree();
	AgxGraphDefinition SFBGS_BuildGraph_StateMachine();
};