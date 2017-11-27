#pragma once
#include "Includes.h"
#include "Utils.h"

using namespace std;


// <=======================================================================>
//	Base resource class 
//		Used to fill arrays of consumed resourced by 
//		instances of the Job class.
// <=======================================================================>
class Resource
{
public:
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
	// Holds the number of resources acquired
	int resourcesAcquired[5];
	// Holds the initial requirements for each
	int resourceNeeds[5];
	
	// Stack of resources
	stack<Resource> resources;

	// Identifiers and Properties. 
	string name;
	bool jobComplete;
	int ID;
	Job();

	// Constructor which completely configures a job to run.
	Job(int i){

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
	bool isFinished(){

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


};
