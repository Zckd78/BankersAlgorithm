#pragma once
#include "Resource.h"

class ReStack
{
public:

	// Head node, obviously.
	Resource * head;
	// Count of available resources, used for determining safe acquisition.
	int ResourcesLeft = 0;
	// Name to pass on to all Resources
	string ResName = "";

	// Functions
	ReStack();
	ReStack(string name);
	void Push(Resource * target);
	Resource * Pop();


};

