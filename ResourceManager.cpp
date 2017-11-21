#include "stdafx.h"
#include "Includes.h"
#include "ResourceManager.h"
#include "Resource.h"
#include "Statics.h"
#include <stack>

// Constructor handles adding intitial resources to their 
ResourceManager::ResourceManager( )
{
	// Create our storage nodes.
	for (int i = MAX_UNITS; i > 0; i--){		
		ResQueueA.push(Resource("A" + to_string(i)));
		ResQueueB.push(Resource("B" + to_string(i)));
		ResQueueC.push(Resource("C" + to_string(i)));
		ResQueueD.push(Resource("D" + to_string(i)));
		ResQueueE.push(Resource("E" + to_string(i)));
	}
}

// Given a Resourcetype, Return a pointer to that Resource store
queue<Resource> * ResourceManager::GetResourceStack(ResourceType type){

	if (type == resA){
		return &ResQueueA;
	}
	if (type == resB){
		return &ResQueueB;
	}
	if (type == resC){
		return &ResQueueC;
	}
	if (type == resD){
		return &ResQueueD;
	}
	if (type == resE){
		return &ResQueueE;
	}

}

Resource ResourceManager::GetResource(ResourceType type){

	Resource curr = nullptr;
	// Check the type provided.
	if (type == resA){
		curr = ResQueueA.front();
		ResQueueA.pop();
		return curr;
	}
	if (type == resB){
		curr = ResQueueB.front();
		ResQueueB.pop();
		return curr;
	}
	if (type == resC){
		curr = ResQueueC.front();
		ResQueueC.pop();
		return curr;
	}
	if (type == resD){
		curr = ResQueueD.front();
		ResQueueD.pop();
		return curr;
	}
	if (type == resE){
		curr = ResQueueE.front();
		ResQueueE.pop();
		return curr;
	}

	// Didnt match any, return a blank one.
	return curr;
}

// Update our ToBeAvail values to match current 
// available resources
void ResourceManager::SetupSafety(){

	ToBeAvail[resA] = ResQueueA.size();
	ToBeAvail[resB] = ResQueueB.size();
	ToBeAvail[resC] = ResQueueC.size();
	ToBeAvail[resD] = ResQueueD.size();
	ToBeAvail[resE] = ResQueueE.size();

}

// Find any thread that isn't finished, and 
bool ResourceManager::isSafe(){

	// ToBeAvail = copy of all 5 resources
	SetupSafety();

	bool JobFound = false;
	bool JobsFinished[MAX_THREADS] = { false, false, false, false, false, false, false, false, false, false };
	
	Job * foundJob = nullptr;

	// Set the current type
	ResourceType type;

	while (true) {

		// Check all threads
		// j = ThreadID
		for (int j = 0; j < MAX_THREADS; j++){

			// Tracks which resource needs fit checks below
			bool JobNeeds[MAX_RESOURCES] = { false, false, false, false, false };

			// Check if this one is finished
			if (JobsFinished[j] == false){

				// Check all Resource Types
				// t = type
				for (int t = 0; t < MAX_RESOURCES; t++){

					// Set current iteration type
					type = (ResourceType)t;

					/*
					if (Jobs[j].resourceNeeds[type] <= ToBeAvail[type]){
						JobNeeds[type] = true;
					}
					*/


					// End Type loop
				}

				// Determine if this job fits the criteria
				if (JobNeeds[resA] && JobNeeds[resB] && JobNeeds[resC]
					&& JobNeeds[resD] && JobNeeds[resE]){
					// foundJob = &Jobs[j];
					JobFound = true;
					break;
				}


				// End Finished Check
			}


			// Didn't find a job
			if (foundJob == nullptr){

				bool foundFalse = false;

				// Determine if even one job is not finished
				for (int i = 0; i < MAX_THREADS; i++){
					if (!JobsFinished[i]) return false;
				}

				return true;
			}
			else {
				// This job could be finished 
				// Need to read the book again on this one.

				// Set this Job as Finished as not to process it again
				JobsFinished[j] = true;

				// Return its resources
				for (int t = 0; t < 5; t++){

					// Set current iteration type
					type = (ResourceType)t;

					// Return resources
					// ToBeAvail[type] += Jobs[j].resourceInitials[type];
				}


				// End Else (Job found)
			}


		// End Job loop
		}

	// End while true
	}


	return false;
}

// Acquire a resource, test if safe, and return the result
bool ResourceManager::wouldBeSafe(ResourceType type){

	return false;
}