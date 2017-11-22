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
