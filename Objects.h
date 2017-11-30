#pragma once
#include "Includes.h"
#include "Utils.h"

using namespace std;

// <=======================================================================>
//	Rresource class 
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
//	 Job object
//		Holds the Resources needed, acquired so far, and object identifiers.
//		in our virtual system
// <=======================================================================>
class Job
{
public:
	// Holds the number of resources acquired
	int resourcesAcquired;
	// Holds the initial requirements for each
	int resourceNeeds;
	
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
