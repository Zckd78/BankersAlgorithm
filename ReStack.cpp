#include "stdafx.h"
#include "ReStack.h"
#include "Resource.h"


ReStack::ReStack()
{
}

ReStack::ReStack(string name){
	ResName = name;
}

void ReStack::Push(Resource * target){

	// Handle empty head
	if (head == nullptr){
		head = target;
	}
	else {
		// Push our new Node to the front.
		head->prev = target;
		target->next = head;
		head = target;
	}
	// We added another resource to this pool.
	ResourcesLeft++;
}

Resource * ReStack::Pop(){

	//Start at the head node.
	Resource * curr = head;

	// Set new head node
	head = curr->next;

	// Deduct a resource.
	ResourcesLeft--;
	// Return head
	return curr;
}