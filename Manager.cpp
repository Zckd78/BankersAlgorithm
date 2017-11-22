#include "stdafx.h"
#include "Manager.h"

// Constructor
Manager::Manager()
{
	// Create our storage nodes.
	for (int i = MAX_UNITS; i > 0; i--){
		ResQueueA.push(Resource("A" + to_string(i)));
		ResQueueB.push(Resource("B" + to_string(i)));
		ResQueueC.push(Resource("C" + to_string(i)));
		ResQueueD.push(Resource("D" + to_string(i)));
		ResQueueE.push(Resource("E" + to_string(i)));
	}
}

queue<Resource> * Manager::GetResourceStack(ResourceType type){

	if (type == resA){
		return &ResQueueA;
	}
	if (type == resB){
		return &ResQueueB;
	}
	if (type == resC){
		return &ResQueueC;
	}
	if (type == resD){
		return &ResQueueD;
	}
	if (type == resE){
		return &ResQueueE;
	}

}

Resource Manager::GetResource(ResourceType type){

		Resource curr = nullptr;
		// Check the type provided.
		if (type == resA){
			curr = ResQueueA.front();
			ResQueueA.pop();
			return curr;
		}
		if (type == resB){
			curr = ResQueueB.front();
			ResQueueB.pop();
			return curr;
		}
		if (type == resC){
			curr = ResQueueC.front();
			ResQueueC.pop();
			return curr;
		}
		if (type == resD){
			curr = ResQueueD.front();
			ResQueueD.pop();
			return curr;
		}
		if (type == resE){
			curr = ResQueueE.front();
			ResQueueE.pop();
			return curr;
		}

		// Didnt match any, return a blank one.
		return curr;
	}

// Update our ToBeAvail values to match current 
// available resources
void Manager::SetupSafety(){

	ToBeAvail[resA] = ResQueueA.size();
	ToBeAvail[resB] = ResQueueB.size();
	ToBeAvail[resC] = ResQueueC.size();
	ToBeAvail[resD] = ResQueueD.size();
	ToBeAvail[resE] = ResQueueE.size();

}

// Find any thread that isn't finished, and 
bool Manager::isSafe(){

	// ToBeAvail = copy of all 5 resources
	SetupSafety();

	bool JobFound = false;
	bool JobsFinished[MAX_THREADS] = { false, false, false, false, false, false, false, false, false, false };

	Job * foundJob = nullptr;

	// Set the current type
	ResourceType type;

	while (true) {

		// Check all threads
		// j = ThreadID
		for (int j = 0; j < MAX_THREADS; j++){

			// Tracks which resource needs fit checks below
			bool JobNeeds[MAX_RESOURCES] = { false, false, false, false, false };

			// Check if this one is finished
			if (JobsFinished[j] == false){

				// Check all Resource Types
				// t = type
				for (int t = 0; t < MAX_RESOURCES; t++){

					// Set current iteration type
					type = (ResourceType)t;

					if (Jobs[j].resourceNeeds[type] <= ToBeAvail[type]){
						JobNeeds[type] = true;
					}


					// End Type loop
				}

				// Determine if this job fits the criteria
				if (JobNeeds[resA] && JobNeeds[resB] && JobNeeds[resC]
					&& JobNeeds[resD] && JobNeeds[resE]){
					foundJob = &Jobs[j];
					JobFound = true;
					break;
				}


				// End Finished Check
			}


			// Didn't find a job
			if (foundJob == nullptr){

				bool foundFalse = false;

				// Determine if even one job is not finished
				for (int i = 0; i < MAX_THREADS; i++){
					if (!JobsFinished[i]) return false;
				}

				return true;
			}
			else {
				// This job could be finished 
				// Need to read the book again on this one.

				// Set this Job as Finished as not to process it again
				JobsFinished[j] = true;

				// Return its resources
				for (int t = 0; t < 5; t++){

					// Set current iteration type
					type = (ResourceType)t;

					// Return resources
					ToBeAvail[type] += Jobs[j].resourceInitials[type];
				}


				// End Else (Job found)
			}


			// End Job loop
		}

		// End while true
	}


	return false;
}

// Acquire a resource, test if safe, and return the result
bool Manager::wouldBeSafe(ResourceType type){

	return false;
}


void Manager::Begin(){

	// Create our Job classes
	// Use a thread to start up the Jobs, which handle their own threads.
	SpinUpJobs();

	// Run all the jobs
	Go();
	cout << "Jobs started..." << endl;

	// Join all threads
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i].join();
	}

	cout << "Jobs finished: " << to_string(glob.JobsCompleted) << endl;
	cout << "Time spent Sleeping: " << to_string(glob.SleepingTime) << "ms" << endl;
}

void Manager::SpinUpJobs(){

	for (int i = 0; i < 10; ++i){
		// Pass Job id to the Jobs.
		Jobs[i] = Job(i, &cv, &mux);
	}
}

void Manager::Go(){

	for (int i = 0; i < MAX_THREADS; i++){
		threads[i] = thread(&Job::DoWork, Jobs[i]);
	}

	unique_lock<mutex> ul(mux);

	// Give jobs some time to be created.
	this_thread::sleep_for(chrono::milliseconds(600));

	// Notify Jobs to begin.
	glob.jobsReady = true;
	cv.notify_one();
}

