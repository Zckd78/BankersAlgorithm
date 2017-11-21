#pragma once
#include "stdafx.h"
#include "Includes.h"
#include "Resource.h"
#include "JobsDTO.h"
#include <stack>

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

	// Data Transfer Object
	JobsDTO * dto;

	// Identifiers and Properties. 
	string name;
	int ID;

	// Member Functions
	Job();
	Job(int i, JobsDTO * dto, condition_variable * cv, mutex * mux);
	void Run();
	void DoWork();
	bool isFinished();

};

