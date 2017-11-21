#pragma once

#include "stdafx.h"
#include "Includes.h"
#include "Utils.h"
#include "Job.h"

using namespace std;

// Handles the active threads
// Holds the static references to all thread activity.
class JobManager
{
private:

	// Manager of Resources
	ResourceManager resManager = ResourceManager::getInstance();
	
	// Thread for starting the jobs
	thread threads[MAX_THREADS];

	// Data Transfer Object
	JobsDTO * dto;

	// Needed for thread synchronization 
	condition_variable cv;
	mutex mux;

public:

	// Default Construtor
	JobManager();
	void Begin();
	void SpinUpJobs();
	void Acquire(string type);
	void Go();

};

