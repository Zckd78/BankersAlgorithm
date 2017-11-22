#pragma once

#include "Includes.h"
#include "Objects.h"

using namespace std;

class Manager
{
private:

	// ========== ========== ========== 
	// Jobs Related
	// ========== ========== ========== 

	// Thread for starting the jobs
	thread threads[MAX_THREADS];

	// Booleans to control execution of Jobs
	bool jobsReady = false;

	// Count of jobs waiting.
	int jobsWaiting = 0;

	// Tracking variables for jobs to update
	int JobsCompleted = 0;
	int SleepingTime = 0;

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
	Job * Jobs[MAX_THREADS];

	// Member Functions
	Manager();
	
	// Resource Related
	queue<Resource> * GetResourceStack(ResourceType type);
	Resource GetResource(ResourceType type);
	void PutResource(ResourceType type, Resource res);
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(ResourceType type);

	// Job Related
	void Manager::Begin();
	void SpinUpJobs();
	void Go();
	void DoWork(int id);

};

