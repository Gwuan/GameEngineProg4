#include "EnemyComponent.h"


#include "AIMoveToPlayer.h"
#include "EnemyType.h"
#include "ResourceManager.h"
#include "ScoreManager.h"
#include "SpriteAnimation.h"
#include "Transform.h"
#include "Utils.hpp"

void EnemyComponent::BeginPlay()
{
	GetOwner().SetTag("Enemy");

	auto spriteSheet = dae::ResourceManager::GetInstance().LoadTexture("SpriteSheet.png");
	GetOwner().AddComponent<AIMoveToPlayer>();
	GetOwner().AddComponent<SpriteAnimation>(spriteSheet, m_Type->horizontalWalkAnimation);

	GetOwner().GetTransform()->SetMoveSpeed(m_Type->movementSpeed);

	m_pCollider = GetOwner().AddComponent<ColliderComponent>(glm::vec2{16, 16}, true);
	if (m_pCollider) m_pCollider->OnBeginOverlap += [this](ColliderComponent* other) { OnBoxOverlap(other); };

	m_pLadderCollider = GetOwner().AddComponent<ColliderComponent>(glm::vec2{4, 10}, true);

	if (m_pLadderCollider)
	{
		m_pLadderCollider->m_Offset.y = -4;

		m_pLadderCollider->OnBeginOverlap += [this](ColliderComponent* other) { LadderBeginOverlap(other); };
		m_pLadderCollider->OnEndOverlap   += [this](ColliderComponent* other)	{ LadderEndOverlap(other); };
	}

	AddObserver(&ScoreManager::GetInstance());

	m_State = std::make_unique<EnemyMoveState>(*this, *m_Type);
	m_State->OnEnter();
}

void EnemyComponent::Update(const float deltaTime)
{
	if (auto newState = m_State->Update(deltaTime))
	{
		m_State->OnExit();
		m_State = std::move(newState);
		m_State->OnEnter();
	}
}

EnemyComponent::~EnemyComponent()
{
	RemoveObserver(&ScoreManager::GetInstance());
}

EnemyComponent::EnemyComponent(dae::GameObject& owner, const EnemyType& type)
	: Component(owner),
	  m_Type(&type)
{}

void EnemyComponent::OnBoxOverlap(const ColliderComponent* otherCollider)
{
	if (otherCollider->GetOwner().GetTag() == "BurgerSlice")
	{
		if (otherCollider->GetOwner().GetTransform()->GetVelocity().y != 0)
			Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnCrush"));
	}
	else if (otherCollider->GetOwner().GetTag() == "Pepper")
	{
		Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnEnemyDeath"));
	}
}

void EnemyComponent::LadderBeginOverlap(const ColliderComponent* otherCollider)
{
	if (otherCollider->GetOwner().GetTag().find("Ladder") != std::string::npos)
	{
		m_LadderCounter++;
		if (otherCollider->GetOwner().GetTag() == "LadderEntry")
		{
			m_OnLadderEntry = true;
		}

		Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnLadderCountChange"));
	}
}

void EnemyComponent::LadderEndOverlap(const ColliderComponent* otherCollider)
{
	if (otherCollider->GetOwner().GetTag().find("Ladder") != std::string::npos)
	{
		m_LadderCounter--;
		m_LadderCounter = std::max(m_LadderCounter, 0);

		if (otherCollider->GetOwner().GetTag() == "LadderEntry")
		{
			m_OnLadderEntry = false;
		}

		Notify(&GetOwner(), HashUtils::make_sdbm_hash("OnLadderCountChange"));
	}
}