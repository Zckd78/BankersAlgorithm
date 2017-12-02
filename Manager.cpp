#include "stdafx.h"
#include "Manager.h"
#include "Includes.h"


// Constructor
Manager::Manager()
{
	// Initialize our ResourceStorage to 8 each
	// Also set Thread completion counts to 0
	for (int i = 0; i < MAX_RESOURCES; i++){
		AvailableResources[i] = MAX_UNITS;
		ThreadsFinished[i] = 0;
	}
}

// Update our ToBeAvail values to match current 
// available resources.
void Manager::SetupSafety(){

	ToBeAvail[resA] = AvailableResources[resA];
	ToBeAvail[resB] = AvailableResources[resB];
	ToBeAvail[resC] = AvailableResources[resC];
	ToBeAvail[resD] = AvailableResources[resD];
	ToBeAvail[resE] = AvailableResources[resE];

}

// Determine safety of the current state by observing all current resource allocations,
// the hypothetical ToBeAvailable if each job that could finish were to finish.
bool Manager::isSafe(){

	// ToBeAvail = copy of Avail
	SetupSafety();
	
	bool JobFound = false;
	Job * foundJob = nullptr, * currentJob = nullptr;

	// Tracks 
	bool JobsFinished[MAX_THREADS] = { false, false, false, false, false, false, false, false, false, false };

	while (true) {

		// Check all threads
		// j = ThreadID
		for (int j = 0; j < MAX_THREADS; j++){

			currentJob = Jobs[j];
			
			// Tracks which resource needs fit checks below
			bool JobNeeds[MAX_RESOURCES] = { false };

			// Check if this one is finished
			if (JobsFinished[j] == false){

				for (int t = 0; t < MAX_RESOURCES; t++){

					ResourceType type = (ResourceType)t;

					int need = (currentJob->resourceNeeds[type] - currentJob->resourcesAcquired[type]);
					if (need <= ToBeAvail[type]){
						JobNeeds[type] = true;
					}

					// End Resource Type check
				}

				// For all i, need[i][j] <= ToBeAvail[i]
				if (JobNeeds[resA] && JobNeeds[resB] && JobNeeds[resC] && JobNeeds[resD] && JobNeeds[resE] ){
					foundJob = currentJob;
					break;
				}
				
				// End Finished Check
			}		

		// End Job loop
		}

		// Didn't find a job
		if (foundJob == nullptr){

			// Determine if even one job is not finished
			for (int i = 0; i < MAX_THREADS; i++){
				if (!JobsFinished[i]){
					return false;
				}
			}
			// Else return true
			return true;
		}
		else {

			// Set this Job as Finished as not to process it again
			JobsFinished[foundJob->ID] = true;

			// Return resources	for each type
			for (int t = 0; t < MAX_RESOURCES; t++){
				ResourceType type = (ResourceType)t;
				if (ToBeAvail[type] < MAX_UNITS)
					ToBeAvail[type] += foundJob->resourcesAcquired[type];
			}			
			
			foundJob = nullptr;
			// End Else (Job found)
		}

	// End while true
	}

	return false;
}

// Acquire a resource, test if safe, and return the result
bool Manager::wouldBeSafe(ResourceType type, int threadID){

	bool result = false;
	Job * currentJob = Jobs[threadID];
	// Only test resource acquisition if the resource store can handle it.
	if (AvailableResources[type] > 0){
		// Acquire a resource for now 
		AvailableResources[type]--;
		currentJob->resourcesAcquired[type]++;
		// Would be safe if job finishes with that one resource.
		if (currentJob->isFinished()){	
			// Put temp resource back
			AvailableResources[type]++;
			currentJob->resourcesAcquired[type]--;
			return true;
		}
		// Test Safety of this scenario
		if (isSafe()) {
			result = true;
		}
		// Put resources back
		AvailableResources[type]++;
		currentJob->resourcesAcquired[type]--;

	}
	else {
		// Won't be safe if resources are drained to zero.
		return false;
	}

	return result;
}


void Manager::Begin(){

	// Run forever...
	while (true){
		
		BeginTime = clock();
		
		// Create our Job classes
		// Use a thread to start up the Jobs, which handle their own threads.
		SpinUpJobs();

		// Increment number of time run.
		ExecCount++;

		// Run all the jobs
		Go();
		PrintProgress();
		PrintStats();		
	}	

}

void Manager::SpinUpJobs(){

	for (int i = 0; i < MAX_THREADS; ++i){
		// Pass Job id to the Jobs.
		Jobs[i] = new Job(i);
	}
}

void Manager::Go(){

	// Setting up a system to stop running jobs.
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i] = thread(&Manager::DoRequests, this, i);
	}

	// Start all the threads
	cv.notify_all();

	// Join all threads
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i].join();			
	}		
	
}

void Manager::DoRequests(int id){

	// This thread will continue to acquire resources until finished,
	// then it will reset the associated Job object, and do it all over again...
	while (true){

		// Start the clock
		StartTimes[id] = clock();

		// Function Level variables for this task
		ResourceType type = resA;
		Job * job = Jobs[id];
		bool workDone = false;
		long printTime = 62.5;

		// Create our lock object
		unique_lock<mutex> ul(mux, defer_lock);
		ul.lock();

		// Stop all threads until we're ready to start.
		cv.wait(ul);

		// Let's run!
		ul.unlock();
		// Keep acquiring resources until we've 
		// gotten all 5 (A-E)
		while (!job->isFinished())
		{
			// Try to acquire one of each resource.
			for (int t = 0; t < MAX_RESOURCES; t++){
				type = (ResourceType)t;

				// Move onto the next resource if we've filled this one
				if (job->resourcesAcquired[type] == job->resourceNeeds[type])
				{
					continue;
				}

				ul.lock();

				// Check if have finished allocationg resources, and 
				// we're currently in a safe state for all resources
				if (isSafe()){

					while (!wouldBeSafe(type, id)){
						// This one isn't safe to proceed, let the next one waiting go.
						cv.notify_one();
						// Then this thread waits.
						job->jobWaiting = true;
						cv.wait(ul);
					}

					// No longer waiting
					job->jobWaiting = false;

					// Allocate the Resource to this Job
					AvailableResources[type]--;
					job->resourcesAcquired[type]++;

					PrintProgress();
					// Sleep for 1/16 of a sec to allow the buffer to catch up
					SleepingTime += printTime;
					this_thread::sleep_for(std::chrono::milliseconds(printTime));
					// Remove sleep time from calculations for RunTime
					StartTimes[id] -= printTime;

					// End isSafe check
				}

				// Let the next thread grab a resource
				cv.notify_one();
				ul.unlock();

				// End of Type loop
			}

			// End of while not finished
		}

		// Lock down so this job can "Run", and then release its resources.
		ul.lock();

		// Notify that a job finished.
		JobsCompleted++;

		// Calculate and update sleep time.
		// Avg time = 250ms
		int sleepTime = GetRand(200, 300);
		SleepingTime += sleepTime;
		// Sleep for 100 milliseconds for each resource consumed in this job.
		this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		// Remove sleep time from calculations for RunTime
		StartTimes[id] -= sleepTime;


		for (int t = 0; t < MAX_RESOURCES; t++){
			// Set Resource Type
			type = (ResourceType)t;
			// Return its resources		
			AvailableResources[type] += job->resourcesAcquired[type];
			// Job Resources now 0
			job->resourcesAcquired[type] = 0;
		}

		PrintProgress();

		// This job finished, let the threads try to acquire resources.
		ul.unlock();
		cv.notify_all();

		// Update Times this thread has completed
		ThreadsFinished[id]++;

		// Calculate Runtime
		ExecTimes.push_back((double)(clock() - StartTimes[id]) / CLOCKS_PER_SEC);
		
		// Reset this job, and run again.
		Jobs[id] = new Job(id);

		// End While(true)
	}

}

// Draw horizontal text for output
void Manager::DrawBar(BarType type){

	switch (type)
	{
	case Manager::BarHeader:
		cout << "/";
		for (int i = 0; i < 75; i++){ cout << "="; }
		cout << "\\" << endl;
		break;
	case Manager::BarFooter:
		cout << "\\";
		for (int i = 0; i < 75; i++){ cout << "="; }
		cout << "/" << endl;
		break;
	case Manager::BarLine:	
		for (int i = 0; i < 77; i++){ cout << "-"; }
		cout << endl;
		break;
	default:
		break;		
	}

}

// Prints out the current allocations of each Job/Thread, and remaining Resources
void Manager::PrintProgress(){

	ResourceType type;
	double runTime;

	PrintTitle();	
	double ExecTime = ((double)(clock() - BeginTime) / CLOCKS_PER_SEC);
	cout << " Time spent executing: " << ExecTime << "s";
	cout << "\t\t\tTime spent sleeping: " << to_string(SleepingTime) << "ms" << endl;

	// Calculate Average run time
	int a = (ExecTimes.size() - 1);
	double avg = 0.0;
	while (a >= 0){
		avg += ExecTimes.at(a);
		a--;
	}
	avg = avg / ExecTimes.size();

	
	cout << " Average thread run time: " << avg << "s" << endl;
	
	cout << " Resources: {";
	for (int t = 0; t < MAX_RESOURCES; t++){
		// Set Resource Type
		type = (ResourceType)t;
		if (type == resE){
			cout << AvailableResources[type];
		}
		else {
			cout << AvailableResources[type] << ",";
		}
	}
	cout << "} units";
	cout << "\t\t\tJobs finished: " << to_string(JobsCompleted) << endl;


	DrawBar(BarHeader);
	// Print out columns
	cout << " Thread# \tNeeds \t\tAlloc \t\t Waiting? \tCompleted#" << endl;
	DrawBar(BarLine);

	for (int i = 0; i < MAX_THREADS; i++){
		// Set current job
		Job * currentJob = Jobs[i];
		
		

		// Display Job ID, and needs.
		cout << " Thread_" << currentJob->ID << "\t{";
		for (int t = 0; t < MAX_RESOURCES; t++){
			// Set Resource Type
			type = (ResourceType)t;
			if (type == resE){
				cout << currentJob->resourceNeeds[type];
			}
			else {
				cout << currentJob->resourceNeeds[type] << ",";
			}
		}
		cout << "}\t{";
		for (int t = 0; t < MAX_RESOURCES; t++){
			// Set Resource Type
			type = (ResourceType)t;
			if (type == resE){
				cout << currentJob->resourcesAcquired[type];
			}
			else {
				cout << currentJob->resourcesAcquired[type] << ",";
			}
		}
		cout << "}\t " << (currentJob->jobWaiting ? "Waiting.." : "\t");
		cout << "\t" << ThreadsFinished[i] << (currentJob->jobComplete ? "++" : "");
			
		
		cout << endl;

	}

	DrawBar(BarLine); 
	DrawBar(BarFooter);
	
}

void Manager::PrintStats(){

	// Clear the screen
	system("cls");

	PrintTitle();

	cout << endl;
	
}

void Manager::PrintTitle(){

	// Clear some space
	system("cls");

	DrawBar(BarHeader);
	// Print out project info	
	cout << " Bankers Algorithm" << endl;
	cout << " Final project for CSCI 144 - Fall 2017" << endl;
	cout << " Written by Zachary Scott" << endl;
	DrawBar(BarFooter);

}