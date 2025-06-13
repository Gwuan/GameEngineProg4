#include "EnemyState.h"

#include <SceneManager.h>

#include "EnemyComponent.h"
#include "IRendererService.h"
#include "Scene.h"
#include "Transform.h"
#include "Utils.hpp"

void EnemyMoveState::OnNotify(dae::GameObject* object, EventID event)
{
	switch (event)
	{
	case HashUtils::make_sdbm_hash("OnLadderCountChange"):
		if (object == &m_pEnemy->GetOwner())
		{
			const auto& ladderCount = m_pEnemy->GetLadderCount();
			if (ladderCount == 1)
			{
				if (m_pEnemy->OnLadderEntry())
					m_pTransform->DisableHorizontalMovement(false);

				m_pTransform->DisableVerticalMovement(false);
			}
			else if (ladderCount > 1)
			{
				m_pTransform->DisableHorizontalMovement(true);
			}
			else
			{
				m_pTransform->DisableVerticalMovement(true);
			}
		}
		break;
	case HashUtils::make_sdbm_hash("OnCrush"):
		if (object == &m_pEnemy->GetOwner())
		{
			m_IsCrunched = true;
		}
		break;
	case HashUtils::make_sdbm_hash("OnDeath"):
		if (object == &m_pEnemy->GetOwner())
		{
			m_IsDead = true;
		}
		break;
	}
}

void EnemyMoveState::OnEnter()
{
	if (auto animation = m_pEnemy->GetOwner().GetComponent<SpriteAnimation>())
	{
		animation->ChangeConfig(m_pType->horizontalWalkAnimation);
		m_pAnimation = animation;
	}
	m_pTransform = m_pEnemy->GetOwner().GetTransform();

	m_pEnemy->AddObserver(this);
}

std::unique_ptr<EnemyState> EnemyMoveState::Update(float)
{
	if (m_pTransform->GetForwardVector().x > 0.f && 
		m_pAnimation->m_Flip == TextureFlip::None)
	{
		m_pAnimation->m_Flip = TextureFlip::Horizontal;
		m_OnLadder = false;
	}
	else if (m_pTransform->GetForwardVector().x < 0.f && 
			 m_pAnimation->m_Flip != TextureFlip::Horizontal)
	{
		m_pAnimation->m_Flip = TextureFlip::None;
		m_OnLadder = false;
	}

	if (!m_OnLadder && !AlmostZero(m_pTransform->MoveDirection.y))
	{
		m_pAnimation->ChangeConfig(m_pType->verticalWalkAnimation);
		auto config = m_pAnimation->GetConfig();

		const auto activeScene = dae::SceneManager::GetInstance().GetActiveScene();

		if (activeScene && m_pTransform)
		{
			const auto worldPos = m_pTransform->GetWorldPosition();
			if (const auto gridResult = activeScene->WorldToGrid(worldPos); gridResult.first)
			{
				const auto worldResult = activeScene->GridToWorld(gridResult.second.x, gridResult.second.y);
				if (worldResult.first)
				{
					const auto& currentWorld = m_pTransform->GetWorldPosition();
					m_pTransform->SetPosition(worldResult.second.x, currentWorld.y);
				}
			}
		}

		m_OnLadder = true;
	}

	if (m_IsCrunched)
		return std::make_unique<EnemyCrushState>(*m_pEnemy, *m_pType);

	if (m_IsDead)
		return std::make_unique<EnemyDieState>(*m_pEnemy, *m_pType);

	return nullptr;
}

void EnemyMoveState::OnExit()
{
	m_pEnemy->RemoveObserver(this);
}

void EnemyCrushState::OnEnter()
{
	if (auto animation = m_pEnemy->GetOwner().GetComponent<SpriteAnimation>())
	{
		animation->ChangeConfig(m_pType->crushAnimation);
	}

	m_pEnemy->GetOwner().GetTransform()->EnableMovement(false);
}

std::unique_ptr<EnemyState> EnemyCrushState::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_DestroyTime)
	{
		// TODO: ADD SCORE
		m_pEnemy->GetOwner().Destroy();
	}
	
	return nullptr;
}

void EnemyDieState::OnEnter()
{
	if (auto animation = m_pEnemy->GetOwner().GetComponent<SpriteAnimation>())
	{
		animation->ChangeConfig(m_pType->dieAnimation);
	}

	m_pEnemy->GetOwner().GetTransform()->EnableMovement(false);
}

std::unique_ptr<EnemyState> EnemyDieState::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;

	if (m_ElapsedTime >= m_DestroyTime)
	{
		// TODO: ADD SCORE
		m_pEnemy->GetOwner().Destroy();
	}

	return nullptr;
}