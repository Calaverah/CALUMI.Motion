#pragma once

#pragma warning(push,0)

#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
#include "Utilities/AgxNodeDelegateModelRegistry.h"

class AgxNodeRegistry
{
private:
	AgxNodeRegistry();

	~AgxNodeRegistry() = default;

public:
	AgxNodeRegistry(const AgxNodeRegistry&) = delete;
	AgxNodeRegistry& operator=(const AgxNodeRegistry&) = delete;

	AgxNodeRegistry(AgxNodeRegistry&&) = delete;
	AgxNodeRegistry& operator=(AgxNodeRegistry&&) = delete;

	static AgxNodeRegistry& GetInstance() {
		static AgxNodeRegistry instance;
		return instance;
	}

	std::shared_ptr<AgxNodeDelegateModelRegistry>& GetRegistry(AgxGameType type);

private:
	std::shared_ptr<AgxNodeDelegateModelRegistry> _defaultRegistry;
	void Default_BuildRegistry();

	std::shared_ptr<AgxNodeDelegateModelRegistry> _sfbgsRegistry;
	void SFBGS_BuildRegistry();

};

