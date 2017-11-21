#pragma once

#include "Includes.h"
#include "Job.h"

// After a few iterations, I decided to create a "Static" object, 
// which I pass into both Manager Objects
class Statics 
{
public:

	static Statics& getInstance(){
		static Statics instance;
		return instance;
	}

	Statics();

	// Global Jobs Array
	Job Jobs[MAX_THREADS];


};