#include "stdafx.h"
#include "Includes.h"
#include "JobManager.h"
#include "Statics.h"

JobManager::JobManager()
{
	dto = new JobsDTO();
}
// Main function which starts all work.
void JobManager::Begin(){

	// Create our Job classes
	// Use a thread to start up the Jobs, which handle their own threads.
	SpinUpJobs();
	
	// Run all the jobs
	Go();
	cout << "Jobs started..." << endl;

	// Join all threads
	for (int i = 0; i < MAX_THREADS; i++){
		threads[i].join();
	}

	cout << "Jobs finished: " << to_string(dto->JobsCompleted) << endl;
	cout << "Time spent Sleeping: " << to_string(dto->SleepingTime) << "ms" << endl;
}

// Spawn our Jobs
void JobManager::SpinUpJobs(){

	for (int i = 0; i < 10; ++i){
		// Pass Job id to the Jobs.
		//Jobs[i] = Job(i, dto, &cv, &mux);
	}
}

void JobManager::Go(){


	for (int i = 0; i < MAX_THREADS; i++){
		//threads[i] = thread(&Job::DoWork, Jobs[i]);
	}

	unique_lock<mutex> ul(mux);
	this_thread::sleep_for(chrono::milliseconds(600));
	dto->jobsReady = true;
	cv.notify_one();

}

