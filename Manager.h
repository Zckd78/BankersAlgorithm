#pragma once

#include "Includes.h"
#include "Objects.h"
#include "Globals.h"

using namespace std;

class Manager
{
private:

	// Global Data Object
	Globals glob = Globals::getInstance();

	// ========== ========== ========== 
	// Jobs Related
	// ========== ========== ========== 

	// Thread for starting the jobs
	thread threads[MAX_THREADS];

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

	// ========== ========== ========== 
	// Resource Related
	// ========== ========== ==========

	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;

	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];

public:

	// Global Jobs Array
	Job Jobs[MAX_THREADS];

	// Member Functions
	Manager();
	
	// Resource Related
	queue<Resource> * GetResourceStack(ResourceType type);
	Resource GetResource(ResourceType type);
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(ResourceType type);

	// Job Related
	void Manager::Begin();
	void SpinUpJobs();
	void Go();


};

