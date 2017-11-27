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

	// Array of jobs waiting.
	bool jobsWaiting[MAX_THREADS];

	// Tracking variables for jobs to update
	int JobsCompleted = 0;
	int SleepingTime = 0;

	// Tracking execution time
	clock_t startTime, endTime;

	
	// ========== ========== ========== 
	// Resource Related
	// ========== ========== ==========
	
	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;

	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];


public:

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

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
	bool wouldBeSafe(ResourceType type, int threadID);

	// Job Related
	void Manager::Begin();
	void SpinUpJobs();
	void Go();
	void Request(int id);

	// Output Related
	void PrintProgress();
};

