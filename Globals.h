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
	
	
	
	Globals();

};

