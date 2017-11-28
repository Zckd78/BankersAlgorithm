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
		// for each based on randomness
		resourceNeeds[resA] = GetRand(1, MAX_CLAIMS);
		resourceNeeds[resB] = GetRand(1, MAX_CLAIMS);
		resourceNeeds[resC] = GetRand(1, MAX_CLAIMS);
		resourceNeeds[resD] = GetRand(1, MAX_CLAIMS);
		resourceNeeds[resE] = GetRand(1, MAX_CLAIMS);

		// Define this Job
		name = "Job " + to_string(i);
		ID = i;
}


// Checks if all resource needs have been met.
bool Job::isFinished(){

	// Single Resource Branch
	// Removing resource checks other than resA for now.
	// for (int i = 0; i < 5; i++) {

	// If any resource is not finished acquiring, 
	// return false.
	if (resourcesAcquired[resA] < resourceNeeds[resA]){
		return false;
	}
	// }

	// Finished looping with all resource needs fulfilled, return true.
	jobComplete = true;
	return true;

}