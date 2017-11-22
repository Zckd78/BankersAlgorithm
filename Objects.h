#pragma once
#include "Includes.h"
#include "Utils.h"
#include "Globals.h"

using namespace std;

Globals glob = Globals::getInstance();

// <=======================================================================>
//	Base resource class 
//		Used to fill arrays of consumed resourced by 
//		instances of the Job class.
// <=======================================================================>
class Resource
{
public:
	Resource * next, *prev;
	// Give it a name for printing.
	string Name = "";

	Resource(){}
	// Constructor to set the name upon creation.
	Resource(string pName){
		Name = pName;
	}

};



// Inherited Resource types
// These represent the 5 Resource types 
// in our virtual system
class ResourceA : public Resource {
	ResourceA() { }
};

class ResourceB : public Resource {
	ResourceB() { }
};

class ResourceC : public Resource {
	ResourceC() { }
};

class ResourceD : public Resource {
	ResourceD() { }
};

class ResourceE : public Resource {
	ResourceE() { }
};


// Static Type enum for later use.
enum ResourceType { resA, resB, resC, resD, resE, };


