#include "stdafx.h"
#include "Objects.h"

// Empty Constructor
Job::Job(){

}

// Constructor which gives this Job and ID, and sets the 
// resource requirements
Job::Job(int i) {

	// Set Required Resources to complete 
	// based on randomness
	for (int i = 0; i < MAX_RESOURCES; i++){
		resourceNeeds[i] = GetRand(1, MAX_CLAIMS);
		resourcesAcquired[i] = 0;
	}

	// Define this Job
	name = "Job " + to_string(i);
	ID = i;
	jobComplete = false;
}


// Checks if all resource needs have been met.
bool Job::isFinished(){

	ResourceType type;
	for (int t = 0; t < MAX_RESOURCES; t++){
		// Set Resource Type
		 type = (ResourceType)t;

		// If resource is not finished acquiring, 
		// return false.
		if (resourcesAcquired[type] < resourceNeeds[type]){
			return false;
		}
	}

	// Finished looping with all resource needs fulfilled, return true.
	jobComplete = true;
	return true;

}