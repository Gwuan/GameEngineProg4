#include "PeterPepperState.h"

#include "ColliderComponent.h"
#include "GameCommands.h"
#include "GameObject.h"
#include "IRendererService.h"
#include "PeterPepperComponent.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ServiceAllocator.h"
#include "SoundSystem.hpp"
#include "SpriteAnimation.h"
#include "Transform.h"
#include "Utils.hpp"


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
		spriteAnim->m_Flip = TextureFlip::None;
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

	m_pPeter->AddObserver(this);
}

void PeterMoveState::OnNotify(dae::GameObject* object, EventID event)
{
	switch (event)
	{
	case HashUtils::make_sdbm_hash("OnLadderCountChange"):
		if (object == &m_pPeter->GetOwner())
		{
			const auto& ladderCount = m_pPeter->GetLadderCount();
			if (ladderCount == 1)
			{
				if (m_pPeter->OnLadderEntry())
					m_pPeterTransform->DisableHorizontalMovement(false);

				m_pPeterTransform->DisableVerticalMovement(false);
			}
			else if (ladderCount > 1)
			{
				m_pPeterTransform->DisableHorizontalMovement(true);
			}
			else
			{
				m_pPeterTransform->DisableVerticalMovement(true);
			}
		}
		break;
	}
}

void PeterMoveState::OnExit()
{
	PeterPepperState::OnExit();

	m_pPeter->RemoveObserver(this);
}

std::unique_ptr<PeterPepperState> PeterMoveState::Update(float)
{
	if (glm::length(m_pPeter->GetOwner().GetTransform()->GetVelocity()) <= 1.f)
	{
		return std::make_unique<PeterIdleState>(*m_pPeter);
	}

	if (m_pPeterTransform->GetForwardVector().x > 0.f && 
		m_pPeterAnimation->m_Flip == TextureFlip::None)
	{
		m_pPeterAnimation->m_Flip = TextureFlip::Horizontal;
		m_OnLadder = false;
	}
	else if (m_pPeterTransform->GetForwardVector().x < 0.f && 
			 m_pPeterAnimation->m_Flip != TextureFlip::Horizontal)
	{
		m_pPeterAnimation->m_Flip = TextureFlip::None;
		m_OnLadder = false;
	}

	if (!m_OnLadder && !AlmostZero(m_pPeterTransform->MoveDirection.y))
	{
		auto config = m_pPeterAnimation->GetConfig();

		config.startRow = 0;
		config.startColumn = 6;
		config.nrOfFrames = 3;

		m_pPeterAnimation->ChangeConfig(config);

		const auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

		if (activeScene && m_pPeterTransform)
		{
			const auto worldPos = m_pPeterTransform->GetWorldPosition();
			if (const auto gridResult = activeScene->WorldToGrid(worldPos); gridResult.first)
			{
				const auto worldResult = activeScene->GridToWorld(gridResult.second.x, gridResult.second.y);
				if (worldResult.first)
				{
					const auto& currentWorld = m_pPeterTransform->GetWorldPosition();
					m_pPeterTransform->SetPosition(worldResult.second.x, currentWorld.y);
				}
			}
		}

		m_OnLadder = true;
	}


	if (m_pPeter->IsShootRequested())
	{
		return std::make_unique<PeterThrowPepperState>(*m_pPeter);
	}

	return nullptr;
}
#pragma endregion

// PETER THROW PEPPER
#pragma region ThrowPepper
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

	m_pPepper->AddComponent<ColliderComponent>(glm::vec2{16, 16}, true);

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