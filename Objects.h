#pragma once
#include "Includes.h"
#include "Utils.h"

using namespace std;

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
	int resourcesAcquired[MAX_RESOURCES];
	// Holds the initial requirements for each
	int resourceNeeds[MAX_RESOURCES];
	
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
