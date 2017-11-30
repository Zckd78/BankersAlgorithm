#include "stdafx.h"
#include "Objects.h"

// Sets the Resource Name
Resource::Resource(string pName){
	Name = pName;
}

// Empty Constructor
Job::Job(){


}

// Constructor which gives this Job and ID, and sets the 
// resource requirements
Job::Job(int i) {

		// Set Required Resources to complete 
		// based on randomness
		resourceNeeds = GetRand(1, MAX_CLAIMS);

		// Define this Job
		name = "Job " + to_string(i);
		ID = i;
}


// Checks if all resource needs have been met.
bool Job::isFinished(){

	// If resource is not finished acquiring, 
	// return false.
	if (resourcesAcquired < resourceNeeds){
		return false;
	}

	// Finished looping with all resource needs fulfilled, return true.
	jobComplete = true;
	return true;

}