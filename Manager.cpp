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
		if (type == resA ){
			if (ResQueueA.empty())
			{
				cout << "Resource A has been exhausted!";
			}
			else {
				curr = ResQueueA.front();
				ResQueueA.pop();
				return curr;
			}
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
// available resources.
void Manager::SetupSafety(){

	// Set ToBeAvail equal to the current 
	// size of each Resource Queue.
	ToBeAvail[resA] = ResQueueA.size();
	ToBeAvail[resB] = ResQueueB.size();
	ToBeAvail[resC] = ResQueueC.size();
	ToBeAvail[resD] = ResQueueD.size();
	ToBeAvail[resE] = ResQueueE.size();	

}

// Determine safety of the current state by observing all current resource allocations,
// the hypothetical ToBeAvailable if each job that could finish were to finish.
bool Manager::isSafe(){

	// ToBeAvail = copy of all 5 resources
	// Gets skipped when called from wouldBeSafe()
	SetupSafety();
	
	bool JobFound = false;
	Job * foundJob = nullptr, * currentJob = nullptr;

	// Tracks 
	bool JobsFinished[MAX_THREADS] = { false, false, false, false, false, false, false, false, false, false };

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
				// for (int t = 0; t < MAX_RESOURCES; t++){

					// Set current iteration type
					// type = (ResourceType)t;
					// Single Resource Branch
					// Removing resource checks other than resA for now.

					type = resA;
					int need = (currentJob->resourceNeeds[type] - currentJob->resourcesAcquired[type]);
					if (  need <= ToBeAvail[type] ){
						JobNeeds[type] = true;
					}

				// End Type loop
				// }

				// Determine if this job fits the criteria
				/* Single Resource Branch
				// Removing resource checks other than resA for now.

				if (JobNeeds[resA] && JobNeeds[resB] && JobNeeds[resC]
					&& JobNeeds[resD] && JobNeeds[resE]){
					foundJob = currentJob;
				}
				*/

				if (JobNeeds[resA]){
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

			// Return its resources
			// for (int t = 0; t < 5; t++){

			// Set current iteration type
			// type = (ResourceType)t;
			type = resA;

			// Return resources			
			ToBeAvail[type] += currentJob->resourcesAcquired[type];
			
			// }

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
	Resource tempRes;
	// Only test resource acquisition if the resource store can handle it.
	if (GetResourceStack(type)->size() > 0){
		tempRes = GetResource(type);
		currentJob->resources.push(tempRes);
		currentJob->resourcesAcquired[type]++;
		if (currentJob->isFinished()){
			// Would be safe if job finishes with that one resource.
			PutResource(type, tempRes);
			currentJob->resources.pop();
			currentJob->resourcesAcquired[type]--;
			return true;
		}

		if (isSafe()) {
			result = true;
		}
		PutResource(type, tempRes);
		currentJob->resources.pop();
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
		// Create our Job classes
		// Use a thread to start up the Jobs, which handle their own threads.
		SpinUpJobs();

		// Run all the jobs
		cout << "Jobs started..." << endl;
		Go();
		PrintProgress();
		cout << endl << "Sleeping for 5 seconds before next run...";
		this_thread::sleep_for(std::chrono::milliseconds(5000));

	}

	cout << "Jobs finished: " << to_string(JobsCompleted) << endl;
	cout << "Time spent Sleeping: " << to_string(SleepingTime) << "ms" << endl;
}

void Manager::SpinUpJobs(){

	for (int i = 0; i < MAX_THREADS; ++i){
		// Pass Job id to the Jobs.
		Jobs[i] = new Job(i);
	}
}

void Manager::Go(){

	// Set clocks
	endTime = 0;
	startTime = clock();

	// Setting up a system to stop running jobs.
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i] = thread(&Manager::Request, this, i);
	}

	// Join all threads
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i].join();			
	}		
	// Set end time
	endTime = clock();
}

void Manager::Request(int id){

	// Function Level variables for this task
	ResourceType currentType = resA;
	string resName = "[BLANK]";
	Job * job = Jobs[id];
	bool workDone = false;

	// Create our lock object
	unique_lock<mutex> ul(mux, defer_lock);

	// Keep acquiring resources until we've 
	// gotten all 5 (A-E)
	while (!job->isFinished())
	{

		// Loop to handle collection of one resource type
		// for (int j = job->resourceNeeds[currentType]; j >= 0; j--) {
			
			ul.lock();

			// Check if have finished allocationg resources, and 
			// we're currently in a safe state for all resources
			if (isSafe()){

				while (!wouldBeSafe(currentType, id)){
					// This one isn't safe to proceed, let the next one waiting go.
					cv.notify_one();
					// Then this thread waits.
					cv.wait(ul);					
				}
				
				// Allocate the Resource to this Job				
				Resource res = GetResource(currentType);
				resName = res.Name;
				job->resources.push(res);
				// Inc/Decement associated counters
				job->resourcesAcquired[currentType]++;


				// End isSafe check
			}

		// } // Finished with one resource
		
		// Single Resource Branch
		// Removing resource checks other than resA for now.
		// Move onto the next type.
		// currentType = (ResourceType)tInc++;
		
		ul.unlock();


		// End of Type loop
	}
	
	// Lock down so this job can "Run", and then release it's resources.
	ul.lock();

	// Notify that a job finished.
	JobsCompleted++;
	// Report to cout
	// cout << job->name << " has started WORK!" << endl;

	// Calculate and update sleep time.
	// Avg time = 200ms
	int sleepTime = GetRand(100,300); 
	SleepingTime += sleepTime;
	// Sleep for 100 milliseconds for each resource consumed in this job.
	this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

	// For each type
	// for (int t = 0; t <= 5; t++) {

	// currentType = (ResourceType)t;

	// Return its resources
	for (int r = job->resourcesAcquired[currentType]; r > 0; r--) {
		GetResourceStack(currentType)->push(job->resources.top());
		resName = job->resources.top().Name;
		job->resources.pop();
		job->resourcesAcquired[currentType]--;
	}
	PrintProgress();
	// }	

	// This job finished, let the threads try to acquire resources.
	ul.unlock();
	cv.notify_all();		
}

// Prints out the current allocations of each Job/Thread, and remaining Resources
void Manager::PrintProgress(){

	int Acquired[MAX_THREADS];

	// Clear some space
	system("cls");

	cout << "//====================================================\\\\" << endl;
	cout << " Job # \t Needs \t Alloc \t Finished?" << endl;
	cout << "--------------------------------------------------------" << endl;

	for (int i = 0; i < MAX_THREADS; i++){

		Job * currentJob = Jobs[i];

		cout << " Job_" << currentJob->ID << " \t " << currentJob->resourceNeeds[resA];
		
		if (currentJob->jobComplete)
		{
			cout << "\t " << currentJob->resourceNeeds[resA] << "\t DONE!";
		}
		else {
			cout << "\t " << currentJob->resourcesAcquired[resA];
		}
		cout << endl;

	}

	cout << "--------------------------------------------------------" << endl;
	cout << " Resource A: " << GetResourceStack(resA)->size() << " units";
	if (endTime != 0)
		cout << "\t RunTime: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " seconds";
	cout << endl;

	cout << "\\\\====================================================//" << endl;





}