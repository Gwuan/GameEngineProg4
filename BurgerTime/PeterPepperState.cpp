#include "PeterPepperState.h"

#include "ColliderComponent.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "SpriteAnimation.h"
#include "Transform.h"

// Can't find a way to make peter shoot without making it a mess

// PETER IDLE
#pragma region PeterIdle
void PeterIdleState::OnEnter()
{
	if (auto spriteAnim = m_pPeter->GetOwner().GetComponent<SpriteAnimation>())
	{
		constexpr SpriteAnimation::AnimationConfig idleConfig {
			.frameSize= glm::vec2{16.f, 16.f},
			.nrOfFrames= 1,
			.startRow= 0,
			.startColumn= 1,
			.totalDuration= 1.f
		};

		spriteAnim->ChangeConfig(idleConfig);
		spriteAnim->m_Flip = SDL_FLIP_NONE;
	}
}

std::unique_ptr<PeterPepperState> PeterIdleState::Update(float)
{
	if (glm::length(m_pPeter->GetOwner().GetTransform()->GetVelocity()) >= 1.f)
	{
		return std::make_unique<PeterMoveState>(*m_pPeter);
	}

	return nullptr;
}
#pragma endregion

// PETER MOVE
#pragma region PeterMove
void PeterMoveState::OnEnter()
{
	if (auto spriteAnim = m_pPeter->GetOwner().GetComponent<SpriteAnimation>())
	{
		constexpr SpriteAnimation::AnimationConfig moveConfig {
			.frameSize= glm::vec2{16.f, 16.f},
			.nrOfFrames= 3,
			.startRow= 0,
			.startColumn= 3,
			.totalDuration= .3f
		};

		spriteAnim->ChangeConfig(moveConfig);
		m_pPeterAnimation = spriteAnim;
	}

	m_pPeterTransform = m_pPeter->GetOwner().GetTransform();
}

std::unique_ptr<PeterPepperState> PeterMoveState::Update(float)
{
	if (glm::length(m_pPeter->GetOwner().GetTransform()->GetVelocity()) <= 1.f)
	{
		return std::make_unique<PeterIdleState>(*m_pPeter);
	}

	if (m_pPeterTransform->GetForwardVector().x > 0.f && 
		m_pPeterAnimation->m_Flip == SDL_FLIP_NONE)
	{
		m_pPeterAnimation->m_Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (m_pPeterTransform->GetForwardVector().x < 0.f && 
			 m_pPeterAnimation->m_Flip != SDL_FLIP_NONE)
	{
		m_pPeterAnimation->m_Flip = SDL_FLIP_NONE;
	}
		
	if (m_pPeter->IsShootRequested())
	{
		return std::make_unique<PeterThrowPepperState>(*m_pPeter);
	}

	return nullptr;
}
#pragma endregion

// PETER THROW PEPPER
#pragma region
const float PeterThrowPepperState::m_MaxTime = 1.f;

void PeterThrowPepperState::OnEnter()
{
	ServiceAllocator::GetSoundSystem().PlaySoundEffect("../Data/Sounds/Pepper_Shake.wav", .6f);

	if (auto spriteAnimation = m_pPeter->GetOwner().GetComponent<SpriteAnimation>())
	{
		constexpr SpriteAnimation::AnimationConfig throwPepperConfig {
			.frameSize = glm::vec2{16.f, 16.f},
			.nrOfFrames = 1,
			.startRow = 1,
			.startColumn = 1,
			.totalDuration = 1.f
		};

		spriteAnimation->ChangeConfig(throwPepperConfig);
	}

	auto peterTransform = m_pPeter->GetOwner().GetTransform();

	if (!peterTransform)
		return;

	peterTransform->EnableMovement(false);
	constexpr float distance = 16.f; 
	const glm::vec2 pepperLocation = peterTransform->GetWorldPosition() + (peterTransform->GetForwardVector() * distance);

	m_pPepper = std::make_shared<dae::GameObject>(pepperLocation, false);
	m_pPepper->SetParent(&m_pPeter->GetOwner(), true);

	m_pPepper->AddComponent<ColliderComponent>(ColliderComponent::Rect({}, 16.f, 16.f), true);

	const SpriteAnimation::AnimationConfig pepperAnimConfig {
		.frameSize = {16.f, 16.f},
		.nrOfFrames = 4,
		.startRow = 1,
		.startColumn = 12,
		.totalDuration = m_MaxTime
	};

	auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	m_pPepper->AddComponent<SpriteAnimation>(spriteSheet, pepperAnimConfig);

	if (auto currentScene = dae::SceneManager::GetInstance().GetActiveScene())
		currentScene->Add(m_pPepper);
}

std::unique_ptr<PeterPepperState> PeterThrowPepperState::Update(float deltaTime)
{
	if (m_TotalElapsedTime >= m_MaxTime)
	{
		return std::make_unique<PeterIdleState>(*m_pPeter);	
	}

	m_TotalElapsedTime += deltaTime;

	return nullptr;
}

void PeterThrowPepperState::OnExit()
{
	m_pPeter->GetOwner().GetTransform()->EnableMovement(true);

	if (m_pPepper)
		m_pPepper->Destroy();
}
#pragma endregion