#include "PeterPepperState.h"

#include "ColliderComponent.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"
#include "ResourceManager.h"
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
		
	// TODO: This need to work ASAP
	// Doesn't compile
	// if (m_pPeter->m_ShootRequested)
	// {
	// 	return std::make_unique<PeterThrowPepperState>(*m_pPeter);
	// }

	return nullptr;
}
#pragma endregion

// PETER THROW PEPPER
#pragma region
const float PeterThrowPepperState::m_MaxTime = 1.f;

void PeterThrowPepperState::OnEnter()
{
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

	// Creating components for the pepper that is thrown
	// TODO: This should be a GameObject, will update in the future
	if (auto peterTransform = m_pPeter->GetOwner().GetTransform())
	{
		constexpr float distance = 16.f; 
		const glm::vec2 pepperTriggerLocation = peterTransform->GetWorldPosition() + (peterTransform->GetForwardVector() * distance);

		m_PepperTrigger = m_pPeter->GetOwner().AddComponent<ColliderComponent>(ColliderComponent::Rect{pepperTriggerLocation, 16.f, 16.f}, true);

		const SpriteAnimation::AnimationConfig pepperAnimConfig {
			.frameSize = {16.f, 16.f},
			.nrOfFrames = 4,
			.startRow = 1,
			.startColumn = 12,
			.totalDuration = m_MaxTime
		};

		auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
		m_PepperAnimation = m_pPeter->GetOwner().AddComponent<SpriteAnimation>(spriteSheet, pepperAnimConfig);
		m_PepperAnimation->SetPositionOffset((peterTransform->GetForwardVector() * distance));
	}
}

std::unique_ptr<PeterPepperState> PeterThrowPepperState::Update(float deltaTime)
{
	if (m_TotalElapsedTime >= m_MaxTime)
	{
		return std::make_unique<PeterIdleState>(*m_pPeter);	
	}

	// Doesn't compile
	// if (m_pPeter->m_ShootRequested)
	// {
	// 	return std::make_unique<PeterThrowPepperState>(*m_pPeter);
	// }

	m_TotalElapsedTime += deltaTime;
	

	return nullptr;
}

void PeterThrowPepperState::OnExit()
{
	if (m_PepperTrigger)
	{
		m_PepperTrigger->Destroy();
	}

	if (m_PepperAnimation)
	{
		m_PepperAnimation->Destroy();
	}
}
#pragma endregion