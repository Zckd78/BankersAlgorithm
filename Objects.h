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
	Resource(string);

};


// <=======================================================================>
//	Inherited Resource types
//		These represent the 5 Resource types 
//		in our virtual system
// <=======================================================================>
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


// Type enum for later use.
enum ResourceType { resA, resB, resC, resD, resE, };


// <=======================================================================>
//	 Job object
//		Holds the Resources needed, acquired so far, and object identifiers.
//		in our virtual system
// <=======================================================================>
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
	bool jobWaiting;
	int ID;
	Job();
	// Constructor which completely configures a job to run.
	Job(int);
	bool isFinished();

};
