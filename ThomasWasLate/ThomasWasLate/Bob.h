#pragma once
#include "Character.h" 

class Bob : public Character
{
public:
	// A constructor specific to Bob 
	Bob::Bob();

	// The overriden input handler for Bob 
	bool virtual handleInput();

};