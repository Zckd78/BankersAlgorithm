#pragma once

#include "Manager.h"

class Globals
{
public:
	
	// Singleton generator
	static Globals & getInstance(){
		static Globals instance;
		return instance;
	}
	
	// Booleans to control execution of Jobs
	bool jobsReady = false;

	// Count of jobs waiting.
	int jobsWaiting = 0;

	// Tracking variables for jobs to update
	int JobsCompleted = 0;
	int SleepingTime = 0;
	
	Globals();

};

