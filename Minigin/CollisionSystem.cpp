#include "CollisionSystem.h"
#include "ColliderComponent.h"

CollisionSystem::~CollisionSystem()
{
	m_pColliders.clear();
}

void CollisionSystem::RegisterCollider(ColliderComponent& component)
{
	if (auto found = std::ranges::find(m_pColliders, &component); found == m_pColliders.end())
	{
		m_pColliders.emplace_back(&component);
	}
}

void CollisionSystem::UnregisterCollider(ColliderComponent& component)
{
	if (auto found = std::ranges::find(m_pColliders, &component); found != m_pColliders.end())
	{
		m_pColliders.erase(found);
	}
}

void CollisionSystem::Update()
{
	for (auto& collider : m_pColliders)
	{
		if (collider->IsStatic())
			continue;

		for (auto& secondCollider : m_pColliders)
		{
			if (secondCollider == collider)
				continue;

			#pragma region OnCollision
			if (collider->CheckCollision(secondCollider))
			{
				// Handle Trigger
				if ((collider->IsTrigger() || secondCollider->IsTrigger()))
				{
					const uint32_t pairID = MakePairID(collider->GetID(), secondCollider->GetID());

					if (!m_OverlappingPairs.contains(pairID))
					{
						m_OverlappingPairs.insert(pairID);

						if (collider->IsTrigger())
							collider->OnBeginOverlap.Broadcast(secondCollider);
						if (secondCollider->IsTrigger())
							collider->OnBeginOverlap.Broadcast(secondCollider);
					}
				}
				else
				{
					collider->OnHit.Broadcast(secondCollider);
					secondCollider->OnHit.Broadcast(collider);
				}
			}
			#pragma endregion
			#pragma region OnNoCollision
			else if ((collider->IsTrigger() || secondCollider->IsTrigger())) // Handle triggers only on no collision
			{
				const uint32_t pairID = MakePairID(collider->GetID(), secondCollider->GetID());

				if (m_OverlappingPairs.contains(pairID))
				{
					m_OverlappingPairs.erase(pairID);

					if (collider->IsTrigger())
						collider->OnEndOverlap.Broadcast(secondCollider);
					if (secondCollider->IsTrigger())
						collider->OnEndOverlap.Broadcast(secondCollider);
				}
			}
			#pragma endregion

		}
	}
}

uint32_t CollisionSystem::MakePairID(uint16_t id1, uint16_t id2)
{
	if (id1 > id2)
	{
		std::swap(id1, id2);
	}

	return (static_cast<uint32_t>(id1) << 16) | id2;
}
