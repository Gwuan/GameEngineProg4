#include "LadderBehavior.h"

#include "ColliderComponent.h"

void LadderBehavior::BeginPlay()
{
	Component::BeginPlay();

	if (auto collider = GetOwner().GetComponent<ColliderComponent>())
	{
		
	}
}
