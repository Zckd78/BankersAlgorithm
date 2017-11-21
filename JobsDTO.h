#pragma once

#include "ResourceManager.h"

class JobsDTO
{
public:

	// Get the single instance of the Resource Manager
	// This instance is shared across all classes.
	ResourceManager resManager = ResourceManager::getInstance();

	// Booleans to control execution of Jobs
	bool jobsReady = false;

	// Count of jobs waiting.
	int jobsWaiting = 0;

	// Tracking variables for jobs to update
	int JobsCompleted = 0;
	int SleepingTime = 0;
	
	JobsDTO();

};

