#include "PeterPepperComponent.h"

#include "ColliderComponent.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "TextureComponent.h"

PeterPepperComponent::PeterPepperComponent(dae::GameObject& owner)
	: Component(owner)
{
	owner.AddComponent<TextureComponent>("../gameResources/pepper.png");
	auto collider = owner.AddComponent<ColliderComponent>(ColliderComponent::Rect{{0, 0}, 32, 32}, true, false);

	collider->OnBeginOverlap += std::bind(&PeterPepperComponent::PlaySoundOnOverlap, this, std::placeholders::_1);
	
}

void PeterPepperComponent::PlaySoundOnOverlap(const ColliderComponent*)
{

	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 1.f);
}
