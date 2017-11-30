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
	
	// Queue for resources
	queue<Resource> ResQueue;

	// To Be Available counts for all 5 resources
	int ToBeAvail;


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

public:

	enum BarType { BarHeader, BarFooter, BarLine };

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

	// Global Jobs Array
	Job * Jobs[MAX_THREADS];

	// Member Functions
	Manager();
	
	// Resource Related
	queue<Resource> * GetResourceStack();
	Resource GetResource();
	void PutResource(Resource res);
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(int threadID);

	// Job Related
	void Begin(int);
	void SpinUpJobs();
	void Go();
	void Request(int id);

	// Output Related
	void PrintProgress();
	void DrawBar(BarType);
};

