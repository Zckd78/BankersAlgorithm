#include "stdafx.h"
#include "Manager.h"
#include "Includes.h"


// Constructor
Manager::Manager()
{
	// Create our storage nodes.
	for (int i = MAX_UNITS; i > 0; i--){
		ResQueue.push(Resource("A" + to_string(i)));
	}
}

queue<Resource> * Manager::GetResourceStack(){

	return &ResQueue;	

}

Resource Manager::GetResource(){

		Resource curr;
		if (ResQueue.empty())
		{
			cout << "Resource A has been exhausted!";
		}
		else {
			curr = ResQueue.front();
			ResQueue.pop();
			return curr;
		}

		// Didnt match any, return a blank one.
		return curr;
	}


void Manager::PutResource(Resource res){

	ResQueue.push(res);
	return;		
	
}


// Update our ToBeAvail values to match current 
// available resources.
void Manager::SetupSafety(){

	// Set ToBeAvail equal to the current 
	// size of each Resource Queue.
	ToBeAvail = ResQueue.size();

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

	while (true) {

		// Check all threads
		// j = ThreadID
		for (int j = 0; j < MAX_THREADS; j++){

			currentJob = Jobs[j];
			
			// Tracks which resource needs fit checks below
			bool JobNeeds = false;

			// Check if this one is finished
			if (JobsFinished[j] == false){

				int need = (currentJob->resourceNeeds - currentJob->resourcesAcquired);
				if (  need <= ToBeAvail){
					JobNeeds = true;
				}

				if (JobNeeds){
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

			// Return resources			
			ToBeAvail += currentJob->resourcesAcquired;
			
			foundJob = nullptr;
			// End Else (Job found)
		}

	// End while true
	}

	return false;
}

// Acquire a resource, test if safe, and return the result
bool Manager::wouldBeSafe(int threadID){

	bool result = false;
	Job * currentJob = Jobs[threadID];
	Resource tempRes;
	// Only test resource acquisition if the resource store can handle it.
	if (GetResourceStack()->size() > 0){
		tempRes = GetResource();
		currentJob->resources.push(tempRes);
		currentJob->resourcesAcquired++;
		// Would be safe if job finishes with that one resource.
		if (currentJob->isFinished()){			
			PutResource(tempRes);
			currentJob->resources.pop();
			currentJob->resourcesAcquired--;
			return true;
		}
		// Test Safety of this scenario
		if (isSafe()) {
			result = true;
		}
		// Put resources back
		PutResource(tempRes);
		currentJob->resources.pop();
		currentJob->resourcesAcquired--;

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

		// Increment number of time run.
		ExecCount++;

		// Run all the jobs
		cout << "Jobs started..." << endl;
		Go();
		PrintProgress();
		PrintStats();
		cout << endl << "Sleeping for 3 seconds before next run...";
		this_thread::sleep_for(std::chrono::milliseconds(3000));
	}

	

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

	// Record Runtime
	if (endTime != 0) {
		double runTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		ExecTimes.push_back(runTime);
	}

}

void Manager::Request(int id){

	// Function Level variables for this task
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

				while (!wouldBeSafe(id)){
					// This one isn't safe to proceed, let the next one waiting go.
					cv.notify_one();
					// Then this thread waits.
					job->jobWaiting = true;
					cv.wait(ul);					
				}
				
				// No longer waiting
				job->jobWaiting = false;
				// Allocate the Resource to this Job				
				Resource res = GetResource();
				resName = res.Name;
				job->resources.push(res);
				// Inc/Decement associated counters
				job->resourcesAcquired++;


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
	for (int r = job->resourcesAcquired; r > 0; r--) {
		GetResourceStack()->push(job->resources.top());
		resName = job->resources.top().Name;
		job->resources.pop();
		job->resourcesAcquired--;
	}
	PrintProgress();
	// }	

	// This job finished, let the threads try to acquire resources.
	ul.unlock();
	cv.notify_all();		
}

void Manager::DrawBar(BarType type){


	switch (type)
	{
	case Manager::BarHeader:
		cout << "/";
		for (int i = 0; i < 50; i++){ cout << "="; }
		cout << "\\" << endl;
		break;
	case Manager::BarFooter:
		cout << "\\";
		for (int i = 0; i < 50; i++){ cout << "="; }
		cout << "/" << endl;
		break;
	case Manager::BarLine:	
		for (int i = 0; i < 52; i++){ cout << "-"; }
		cout << endl;
		break;
	default:
		break;		
	}



}

// Prints out the current allocations of each Job/Thread, and remaining Resources
void Manager::PrintProgress(){

	int Acquired[MAX_THREADS];
	double runTime;

	PrintTitle();

	// Print the number of times run so far.
	cout << endl << "----------===========[ Run# " << ExecCount << " ]===========----------"  << endl;

	// Print out columns
	cout << " Job # \t Needs \t Alloc \t Waiting? \t Finished?" << endl;
	DrawBar(BarLine);

	for (int i = 0; i < MAX_THREADS; i++){
		// Set current job
		Job * currentJob = Jobs[i];
		
		if (currentJob->jobComplete)
		{
			// Display Job ID, and needs.
			cout << " Job_" << currentJob->ID << " \t " << currentJob->resourceNeeds;
			cout << "\t " << currentJob->resourceNeeds;
			cout << "\t" << (currentJob->jobWaiting ? "Waiting.." : "") << "\t\t DONE!";			
		}
		else {

			// Display Job ID, and needs.
			cout << " Job_" << currentJob->ID << " \t " << currentJob->resourceNeeds;
			cout << "\t " << currentJob->resourcesAcquired;
			cout << "\t " << (currentJob->jobWaiting ? "Waiting.." : "");
			
		}
		cout << endl;

	}

	DrawBar(BarLine); 
	cout << " Resource: " << GetResourceStack()->size() << " units";
	if (endTime != 0) {
		runTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
		cout << "\t RunTime: " << runTime << " seconds";
		ExecTimes.push_back(runTime);
		cout << endl;
		DrawBar(BarFooter);

		// Pause for a second to see.
		this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
	else {
		cout << endl;
		DrawBar(BarFooter);

	}
}

void Manager::PrintStats(){

	// Clear the screen
	system("cls");

	PrintTitle();

	cout << endl;
	DrawBar(BarHeader);
	cout << "  Statistics " << endl;
	DrawBar(BarLine);

	cout << endl;

	cout << " Jobs finished: " << to_string(JobsCompleted) << endl;
	cout << " Time spent Sleeping: " << to_string(SleepingTime) << "ms" << endl;

	// Calculate Average run time
	int a = (ExecTimes.size() - 1);
	double avg = 0.0;
	while (a >= 0){
		avg += ExecTimes.at(a);
		a--;
	}
	avg = avg / ExecTimes.size();

	cout << " Average Run time: " << avg << endl;
	cout << endl;
	DrawBar(BarLine);
	DrawBar(BarFooter);
}

void Manager::PrintTitle(){

	// Clear some space
	system("cls");

	DrawBar(BarHeader);
	// Print out project info	
	cout << " Bankers Algorithm" << endl;
	cout << " Final project for CSCI 144 - Fall 2017" << endl;
	cout << " Written by Zachary Scott" << endl;
	DrawBar(Manager::BarLine);

}