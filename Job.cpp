#include "stdafx.h"
#include "Job.h"
#include "Utils.h"

Job::Job()
{
}

// Constructor which completely configures a job to run.
Job::Job(int i, JobsDTO * dto, condition_variable * cv, mutex * mux){
	
	// Set Required Resources to complete 
	// for each based on randomness
	resourceNeeds[resA] = GetRand();
	resourceNeeds[resB] = GetRand();
	resourceNeeds[resC] = GetRand();
	resourceNeeds[resD] = GetRand();
	resourceNeeds[resE] = GetRand();

	// Define this Job
	name = "Job " + to_string(i);
	ID = i;

	// Setup local refs to thread sync variables
	this->cv = cv;
	this->mux = mux;
	this->dto = dto;
}

// Start to acquire enough resources to finish,
// Then we sleep for awhile to simulate processing.
void Job::DoWork(){
	
	ResourceType currentType = resA;
		
	// Keep acquiring resources until we've 
	// gotten all 5 (A-E)
	// while ((int)currentType < 6)
	// { }

	// Acquire the same lock as Manager
	unique_lock<mutex> ul(*mux);
	

	// Loop to handle collection of one resource type
	for (int j = resourceNeeds[currentType]; j >= 0; j--) {

		// Check if we're currently in a safe state for all resources
		// and job needs
		if (dto->resManager.isSafe()){

			// while (!wouldBeSafe() ){
			while (!dto->jobsReady){
				dto->jobsWaiting++;
				cv->wait(ul);
				cout << name << " just started!" << endl;
			}

			// Allocate the Resource to this Job
			resources.push(dto->resManager.GetResource(currentType));

		}

		cout << name << " acquired 1 resource!" << endl;
	} // Finished with one resource

	

	// Notify that a job finished.
	dto->JobsCompleted++;

	// Calculate and update sleep time.
	int sleepTime = resourceInitials[currentType] * 150;
	dto->SleepingTime += sleepTime;
	// Sleep for 100 milliseconds for each resource consumed in this job.
	this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

	// Notify next job to start.
	cv->notify_one();

}

bool Job::isFinished(){
	
	for (int i = 0; i < 5; i++) {

		// If any resource is not finished acquiring, 
		// return false.
		if (resourceNeeds[i] > 0){
			return false;
		}
	}
	
	// Finished looping with all resource needs fulfilled, return true.
	return true;
}
