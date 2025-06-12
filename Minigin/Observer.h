#pragma once
#include "Subject.h"

using EventID = size_t;

namespace dae
{
	class GameObject;
}

// Interface
class IObserver
{
public:
	// Use Hashing
	virtual void OnNotify(dae::GameObject* object, EventID event ) = 0;
};