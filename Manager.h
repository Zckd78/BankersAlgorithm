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
	int ThreadsFinished[MAX_THREADS];
	
	// ========== ========== ========== 
	// Resource Related
	// ========== ========== ==========
	
	// Array for resources
	int AvailableResources[MAX_RESOURCES];

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
	clock_t BeginTime;
	clock_t StartTimes[MAX_THREADS];
	vector<double> ExecTimes;

public:

	enum BarType { BarHeader, BarFooter, BarLine };

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

	// Global Jobs Array
	Job * Jobs[MAX_THREADS];

	// Constructor
	Manager();
	
	// Resource Related	
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(ResourceType type, int threadID);

	// Job Related
	void Begin();
	void SpinUpJobs();
	void Go();
	void DoRequests(int id);

	// Output Related
	void PrintProgress();
	void DrawBar(BarType);
	void PrintStats();
	void PrintTitle();
};

