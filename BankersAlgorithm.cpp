// BankersAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "JobManager.h"
#include "Job.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// Get the single instance
	ResourceManager resManager = ResourceManager::getInstance();
	
	// Create our Job Manager
	JobManager jobManager;
	
	// Start the Jobs!
	jobManager.Begin();
	
	// Adding this to test the program and actually have a chance to see the output.
	system("pause");
	return 0;
}


