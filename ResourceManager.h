#pragma once

#include "Resource.h"
#include "ResourceDTO.h"
#include <queue>

class ResourceManager
{
private:
	// Declare our five resources (A-E)
	// ReStack ResourceA, ResourceB, ResourceC, ResourceD, ResourceE;
	
	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;
	
	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];
	
public:

	static ResourceManager& getInstance(){
		static ResourceManager instance;
		return instance;
	}

	// Member Functions
	ResourceManager();
	Resource GetResource(ResourceType type);
	queue<Resource> * GetResourceStack(ResourceType type);
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(ResourceType type);

};

