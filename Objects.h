#pragma once
#include "Includes.h"
#include "Utils.h"

using namespace std;

// Type enum for later use.
enum ResourceType { resA, resB, resC, resD, resE, };

// <=======================================================================>
//	 Process object
//		Holds the Resources needed, acquired so far, and object identifiers.
//		in our virtual system
// <=======================================================================>
class Process
{
public:
	// Holds the number of resources acquired
	int resourcesAcquired[MAX_RESOURCES];
	// Holds the initial requirements for each
	int resourceNeeds[MAX_RESOURCES];
	
	// Identifiers and Properties. 
	string name;
	bool Complete;
	bool Waiting;
	int ID;
	Process();
	// Constructor which completely configures a process to run.
	Process(int);
	bool isFinished();
};
