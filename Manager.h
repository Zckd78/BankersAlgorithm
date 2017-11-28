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

	
	// ========== ========== ========== 
	// Resource Related
	// ========== ========== ==========
	
	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;

	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];



	// ========== ========== ========== 
	// Output / Tracking Related
	// ========== ========== ==========
	
	// # times run
	int ExecCount = 0;

	// Tracking variables for jobs to update
	int JobsCompleted = 0;
	int SleepingTime = 0;

	// Tracking execution time
	clock_t startTime, endTime;
	vector<double> ExecTimes;

	enum BarType { BarHeader, BarFooter, BarLine};

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
	void DrawBar(BarType);
};

