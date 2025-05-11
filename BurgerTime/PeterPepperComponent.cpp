#include "PeterPepperComponent.h"

#include "ColliderComponent.h"
#include "PeterPepperState.h"
#include "ResourceManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "SpriteAnimation.h"
#include "TextureComponent.h"
#include "Transform.h"

void PeterPepperComponent::Update(const float)
{
	//std::cout << glm::length(GetOwner().GetTransform()->velocity) << std::endl;

	if (auto newState = m_State->Update())
	{
		m_State->OnExit();
		m_State = std::move(newState);
		m_State->OnEnter();
	}
}

PeterPepperComponent::PeterPepperComponent(dae::GameObject& owner)
	: Component(owner)
{
	auto collider = owner.AddComponent<ColliderComponent>(ColliderComponent::Rect{{0, 0}, 32, 32}, false);

	auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	owner.AddComponent<SpriteAnimation>(spriteSheet, SpriteAnimation::AnimationConfig{});

	collider->OnBeginOverlap += std::bind(&PeterPepperComponent::PlaySoundOnOverlap, this, std::placeholders::_1);

	m_State = std::make_unique<PeterIdleState>(owner);

}

void PeterPepperComponent::PlaySoundOnOverlap(const ColliderComponent*)
{

	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 1.f);
}
