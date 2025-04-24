#pragma once
#include "Subject.h"

using EventID = size_t;

// Interface
class IObserver
{
public:
	// Use Hashing
	virtual void OnNotify(dae::GameObject* object, EventID event ) = 0;
};