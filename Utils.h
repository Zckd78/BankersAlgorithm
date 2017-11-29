#pragma once

#include "stdafx.h"
#include "Includes.h"

using namespace std;

static int GetRand(int low, int high){

	int retVal = 0;

	// ====================================================================================================
	// The following random integer code is sourced from:
	// https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range#19728404

	// only used once to initialise (seed) engine
	random_device rd;
	// random-number engine used (Mersenne-Twister in this case)
	std::mt19937 rng(rd());
	// guaranteed unbiased
	std::uniform_int_distribution<int> uni(low, high);

	// Adding this part myself. Sometimes it still returns a 0 (zero)
	// We can't have a job requiring zero resources.
	while (retVal == 0)
		retVal = uni(rng);
	
	// ====================================================================================================

	return retVal;

}

// Make space for a new messsage
static void ClearTerminal(){
	int n;
	for (n = 0; n < 100; n++){
		cout << endl;
		
	}
}
