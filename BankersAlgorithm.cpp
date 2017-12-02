// BankersAlgorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Manager.h"

using namespace std;

// <=======================================================================>
//	MAIN FUNCTION 
//		Entry point for the whole program.
// <=======================================================================>

int _tmain(int argc, _TCHAR* argv[])
{
	// Manager class, which starts it all
	Manager man;

	// Start the Processes!
	man.Begin();
	
	// Adding this to test the program and actually have a chance to see the output.
	system("pause");
	return 0;
}


