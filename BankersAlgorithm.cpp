// BankersAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Includes.h"
#include "JobManager.h"

using namespace std;

// Job object.
// Handles making requests.
class Job
{
public:

	// Required resource counts
	// Decremented as we acquire resources of that type
	int resourceNeeds[5];
	// Holds the initial requirements for each
	int resourceInitials[5];
	// Stack of resources
	stack<Resource> resources;

	// Thread sync
	condition_variable * cv;
	mutex * mux;

	// Data Transfer Object
	JobsDTO * dto;

	// Identifiers and Properties. 
	string name;
	int ID;

	// Constructor which completely configures a job to run.
	Job(int i, JobsDTO * dto, condition_variable * cv, mutex * mux){

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
	
	void DoWork(){

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

	// Checks if all resource needs have been met.
	bool isFinished(){

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


};


// Global Jobs Array
Job Jobs[MAX_THREADS];



// Handles the active threads
// Holds the static references to all thread activity.
class JobManager
{
private:

	// Thread for starting the jobs
	thread threads[MAX_THREADS];

	// Data Transfer Object
	JobsDTO * dto;

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

public:

	// Default Construtor
	JobManager(){}

	void Begin(){

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

		cout << "Jobs finished: " << to_string(dto->JobsCompleted) << endl;
		cout << "Time spent Sleeping: " << to_string(dto->SleepingTime) << "ms" << endl;
	}

	void SpinUpJobs(){

		for (int i = 0; i < 10; ++i){
			// Pass Job id to the Jobs.
			Jobs[i] = Job(i, dto, &cv, &mux);
		}
	}
	
	void Go(){

		for (int i = 0; i < MAX_THREADS; i++){
			//threads[i] = thread(&Job::DoWork, Jobs[i]);
		}

		unique_lock<mutex> ul(mux);

		// Give jobs some time to be created.
		this_thread::sleep_for(chrono::milliseconds(600));

		// Notify Jobs to begin.
		dto->jobsReady = true;
		cv.notify_one();
	}

};








// <=======================================================================>
//	MAIN FUNCTION 
//		Entry point for the whole program.
// <=======================================================================>

int _tmain(int argc, _TCHAR* argv[])
{
	// Get the single instance
	ResourceManager resManager = ResourceManager::getInstance();
	
	// Create our Job Manager
	JobManager jobManager;
	
	// Start the Jobs!
	jobManager.Begin();
	
	// Adding this to test the program and actually have a chance to see the output.
	system("pause");
	return 0;
}


