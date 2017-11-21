// BankersAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Includes.h"
#include "Utils.h"

using namespace std;

// Booleans to control execution of Jobs
bool jobsReady = false;

// Count of jobs waiting.
int jobsWaiting = 0;

// Tracking variables for jobs to update
int JobsCompleted = 0;
int SleepingTime = 0;


// <=======================================================================>
//	Base resource class 
//		Used to fill arrays of consumed resourced by 
//		instances of the Job class.
// <=======================================================================>
class Resource
{
public:
	Resource * next, *prev;
	// Give it a name for printing.
	string Name = "";

	Resource(){}
	// Constructor to set the name upon creation.
	Resource(string pName){
		Name = pName;
	}

};



// Inherited Resource types
// These represent the 5 Resource types 
// in our virtual system
class ResourceA : public Resource {
	ResourceA() { }
};

class ResourceB : public Resource {
	ResourceB() { }
};

class ResourceC : public Resource {
	ResourceC() { }
};

class ResourceD : public Resource {
	ResourceD() { }
};

class ResourceE : public Resource {
	ResourceE() { }
};


// Static Type enum for later use.
enum ResourceType { resA, resB, resC, resD, resE, };



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

	// Identifiers and Properties. 
	string name;
	int ID;

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
			if (resManager.isSafe()){

				// while (!wouldBeSafe() ){
				while (!jobsReady){
					jobsWaiting++;
					cv->wait(ul);
					cout << name << " just started!" << endl;
				}

				// Allocate the Resource to this Job
				resources.push(resManager.GetResource(currentType));

			}

			cout << name << " acquired 1 resource!" << endl;
		} // Finished with one resource

		// Notify that a job finished.
		JobsCompleted++;

		// Calculate and update sleep time.
		int sleepTime = resourceInitials[currentType] * 150;
		SleepingTime += sleepTime;
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


class ResourceManager
{
private:
	// Declare our five resources (A-E)
	// ReStack ResourceA, ResourceB, ResourceC, ResourceD, ResourceE;

	// New Stack based resources
	queue<Resource> ResQueueA, ResQueueB, ResQueueC, ResQueueD, ResQueueE;

	// To Be Available counts for all 5 resources
	int ToBeAvail[MAX_RESOURCES];

public:

	// Member Functions
	ResourceManager()
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
	queue<Resource> * GetResourceStack(ResourceType type){

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
	Resource ResourceManager::GetResource(ResourceType type){

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
	void ResourceManager::SetupSafety(){

		ToBeAvail[resA] = ResQueueA.size();
		ToBeAvail[resB] = ResQueueB.size();
		ToBeAvail[resC] = ResQueueC.size();
		ToBeAvail[resD] = ResQueueD.size();
		ToBeAvail[resE] = ResQueueE.size();

	}

	// Find any thread that isn't finished, and 
	bool ResourceManager::isSafe(){

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
	bool ResourceManager::wouldBeSafe(ResourceType type){

		return false;
	}
};

// Global Resource Manager
ResourceManager resManager;


// Handles the active threads
// Holds the static references to all thread activity.
class JobManager
{
private:

	// Thread for starting the jobs
	thread threads[MAX_THREADS];
	
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

		cout << "Jobs finished: " << to_string(JobsCompleted) << endl;
		cout << "Time spent Sleeping: " << to_string(SleepingTime) << "ms" << endl;
	}

	void SpinUpJobs(){

		for (int i = 0; i < 10; ++i){
			// Pass Job id to the Jobs.
			Jobs[i] = Job(i, &cv, &mux);
		}
	}
	
	void Go(){

		for (int i = 0; i < MAX_THREADS; i++){
			threads[i] = thread(&Job::DoWork, Jobs[i]);
		}

		unique_lock<mutex> ul(mux);

		// Give jobs some time to be created.
		this_thread::sleep_for(chrono::milliseconds(600));

		// Notify Jobs to begin.
		jobsReady = true;
		cv.notify_one();
	}

};

// Create our Job Manager
JobManager jobManager;






// <=======================================================================>
//	MAIN FUNCTION 
//		Entry point for the whole program.
// <=======================================================================>

int _tmain(int argc, _TCHAR* argv[])
{
	// Start the Jobs!
	jobManager.Begin();
	
	// Adding this to test the program and actually have a chance to see the output.
	system("pause");
	return 0;
}


