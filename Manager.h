#pragma once

#include "Includes.h"
#include "Objects.h"
#include "Globals.h"

using namespace std;

class Manager
{
private:

	// Global Data Object
	Globals glob = Globals::getInstance();

	// ========== ========== ========== 
	// Jobs Related
	// ========== ========== ========== 

	// Thread for starting the jobs
	thread threads[MAX_THREADS];

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

	// ========== ========== ========== 
	// Resource Related
	// ========== ========== ==========

	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;

	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];



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

		Globals glob = Globals::getInstance();

		// Thread sync
		condition_variable * cv;
		mutex * mux;

		// Identifiers and Properties. 
		string name;
		int ID;

		Job();

		// Constructor which completely configures a job to run.
		Job(int i, condition_variable * cv, mutex * mux){

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
				if (isSafe()){

					// while (!wouldBeSafe() ){
					while (!glob.jobsReady){
						glob.jobsWaiting++;
						cv->wait(ul);
						cout << name << " just started!" << endl;
					}

					// Allocate the Resource to this Job
					resources.push(GetResource(currentType));

				}

				cout << name << " acquired 1 resource!" << endl;
			} // Finished with one resource

			// Notify that a job finished.
			glob.JobsCompleted++;

			// Calculate and update sleep time.
			int sleepTime = resourceInitials[currentType] * 150;
			glob.SleepingTime += sleepTime;
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


public:

	// Global Jobs Array
	Job Jobs[MAX_THREADS];

	// Member Functions
	Manager();
	
	// Resource Related
	queue<Resource> * GetResourceStack(ResourceType type);
	Resource GetResource(ResourceType type);
	void SetupSafety();
	bool isSafe();
	bool wouldBeSafe(ResourceType type);

	// Job Related
	void Manager::Begin();
	void SpinUpJobs();
	void Go();


};

