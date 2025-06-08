#include "PeterPepperComponent.h"

#include "ColliderComponent.h"
#include "PeterPepperState.h"
#include "ResourceManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "SpriteAnimation.h"
#include "TextureComponent.h"
#include "Transform.h"

void PeterPepperComponent::Update(float deltaTime)
{
	if (auto newState = m_State->Update(deltaTime))
	{
		m_State->OnExit();
		m_State = std::move(newState);
		m_State->OnEnter();
	}
}

void PeterPepperComponent::LateUpdate(const float)
{
	if (m_ShootRequested)
		m_ShootRequested = false;
}

void PeterPepperComponent::RequestShoot()
{
	if (!m_ShootRequested)
		m_ShootRequested = true;
}

PeterPepperComponent::PeterPepperComponent(dae::GameObject& owner)
	: Component(owner)
{
	owner.AddComponent<ColliderComponent>(ColliderComponent::Rect{{0, 0}, 16, 16}, false);

	auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	owner.AddComponent<SpriteAnimation>(spriteSheet, SpriteAnimation::AnimationConfig{});


	m_State = std::make_unique<PeterIdleState>(*this);
	m_State->OnEnter();
}

void PeterPepperComponent::PlaySoundOnOverlap(const ColliderComponent*)
{

	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../gameResources/sounds/level_start.wav", 1.f);
}
