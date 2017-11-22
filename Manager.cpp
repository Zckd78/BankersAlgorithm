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

		Resource curr;
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


void Manager::PutResource(ResourceType type, Resource res){

	// Check the type provided.
	if (type == resA){
		ResQueueA.push(res);
		return;
	}
	if (type == resB){
		ResQueueB.push(res);
		return;
	}
	if (type == resC){
		ResQueueC.push(res);
		return;
	}
	if (type == resD){
		ResQueueD.push(res);
		return;
	}
	if (type == resE){
		ResQueueE.push(res);
		return;
	}

	return;
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

	Job * foundJob = nullptr, * currentJob = nullptr;

	// Set the current type
	ResourceType type;

	while (true) {

		// Check all threads
		// j = ThreadID
		for (int j = 0; j < MAX_THREADS; j++){

			currentJob = Jobs[j];

			// Tracks which resource needs fit checks below
			bool JobNeeds[MAX_RESOURCES] = { false, false, false, false, false };

			// Check if this one is finished
			if (JobsFinished[j] == false){

				// Check all Resource Types
				// t = type
				for (int t = 0; t < MAX_RESOURCES; t++){

					// Set current iteration type
					type = (ResourceType)t;
					int need = (currentJob->resourceNeeds[type] - currentJob->resourcesAcquired[type]);
					if (  need <= ToBeAvail[type] ){
						JobNeeds[type] = true;
					}

				// End Type loop
				}

				// Determine if this job fits the criteria
				if (JobNeeds[resA] && JobNeeds[resB] && JobNeeds[resC]
					&& JobNeeds[resD] && JobNeeds[resE]){
					foundJob = currentJob;
				}

				// End Finished Check
			}

			// Didn't find a job
			if (foundJob == nullptr){

				bool foundFalse = false;

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
				JobsFinished[j] = true;

				// Return its resources
				for (int t = 0; t < 5; t++){

					// Set current iteration type
					type = (ResourceType)t;

					// Return resources
					ToBeAvail[type] += currentJob->resourcesAcquired[type];
				}

				foundJob = nullptr;
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

	bool result = false;

	ToBeAvail[type]--;
	Resource test = GetResource(type);
	if (isSafe()) {
		result = true;
	}
	PutResource(type, test);
	ToBeAvail[type]++;

	return result;
}


void Manager::Begin(){

	// Create our Job classes
	// Use a thread to start up the Jobs, which handle their own threads.
	SpinUpJobs();

	// Run all the jobs
	cout << "Jobs started..." << endl;
	Go();
	

	// Join all threads
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i].join();
	}

	cout << "Jobs finished: " << to_string(JobsCompleted) << endl;
	cout << "Time spent Sleeping: " << to_string(SleepingTime) << "ms" << endl;
}

void Manager::SpinUpJobs(){

	for (int i = 0; i < 10; ++i){
		// Pass Job id to the Jobs.
		Jobs[i] = new Job(i);
	}
}

void Manager::Go(){

	for (int i = 0; i < MAX_THREADS; i++){
		threads[i] = thread(&Manager::DoWork, this, i);
	}

	unique_lock<mutex> ul(mux);

	// Give jobs some time to be created.
	this_thread::sleep_for(chrono::milliseconds(600));

	// Notify Jobs to begin.
	jobsReady = true;
	cv.notify_one();
}

void Manager::DoWork(int id){

	// Function Level variables for this task
	ResourceType currentType = resA;
	string resName = "[BLANK]";
	Job * job = Jobs[id];

	// Create our lock object
	unique_lock<mutex> ul(mux, defer_lock);

	cout << "Job " << id << " stopped waiting!" << endl;
	int tInc = (int)currentType;

		
	// Keep acquiring resources until we've 
	// gotten all 5 (A-E)
	while (!job->isFinished())
	{

		// Loop to handle collection of one resource type
		for (int j = job->resourceNeeds[currentType]; j >= 0; j--) {

			ul.lock();

			// Check if we're currently in a safe state for all resources
			// and job needs
			if (isSafe()){

				while (!wouldBeSafe(currentType)){
					cout << job->name << " can't proceed (Unsafe)!" << endl;
					cv.wait(ul);
					cout << job->name << " was woken up!" << endl;
				}

				// Allocate the Resource to this Job
				Resource res = GetResource(currentType);
				resName = res.Name;
				job->resources.push(res);
				// Inc/Decement associated counters
				job->resourcesAcquired[currentType]++;
				ToBeAvail[currentType]--;
				// Report to cout
				cout << job->name << " acquired resource " << resName << "!" << endl;
				
				cv.notify_one();
				ul.unlock();
			}

		} // Finished with one resource

		// Move onto the next type.
		currentType = (ResourceType)tInc++;
		
		// End of Type loop
	}

	// Notify that a job finished.
	JobsCompleted++;

	// Calculate and update sleep time.
	int sleepTime = job->resourceInitials[currentType] * 150;
	SleepingTime += sleepTime;
	// Sleep for 100 milliseconds for each resource consumed in this job.
	this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

	
}