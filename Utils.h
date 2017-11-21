#pragma once

#include "stdafx.h"
#include "Includes.h"

using namespace std;

static int GetRand(){

	// ====================================================================================================
	// The following random integer code is sourced from:
	// https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range#19728404

	// only used once to initialise (seed) engine
	random_device rd;
	// random-number engine used (Mersenne-Twister in this case)
	std::mt19937 rng(rd());
	// guaranteed unbiased
	std::uniform_int_distribution<int> uni(1, MAX_CLAIMS);

	return uni(rng);
	// ====================================================================================================

}

static void ClearTerminal(){
	int n;
	for (n = 0; n < 10; n++){
		printf("\n\n\n\n\n\n\n\n\n\n");
	}
}
