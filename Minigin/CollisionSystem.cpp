#include "CollisionSystem.h"
#include "ColliderComponent.h"
#include "Transform.h"
#include "../BurgerTime/GameCommands.h"

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
		if (collider->GetOwner().IsStatic())
			continue;

		for (auto& secondCollider : m_pColliders)
		{
			if (secondCollider == collider)
				continue;

			// Create a single pairID, we need them all cases
			const uint32_t pairID = MakePairID(collider->GetID(), secondCollider->GetID());

			#pragma region OnCollision
			if (collider->CheckCollision(secondCollider))
			{
				// Handle Trigger
				if ((collider->IsTrigger() || secondCollider->IsTrigger()))
				{
					if (m_OverlappingPairs.contains(pairID))
						continue;

					m_OverlappingPairs.insert(pairID);

					collider->OnBeginOverlap.Broadcast(secondCollider);
					secondCollider->OnBeginOverlap.Broadcast(collider);
				}
				else
				{
					ResolveCollision(collider, secondCollider);

					if (m_HittingPairs.contains(pairID))
						continue;

					m_HittingPairs.insert(pairID);

					collider->OnHit.Broadcast(secondCollider);
					secondCollider->OnHit.Broadcast(collider);
				}
			}
			#pragma endregion
			#pragma region OnNoCollision
			else if ((collider->IsTrigger() || secondCollider->IsTrigger())) // Handle triggers only on no collision
			{
				if (!m_OverlappingPairs.contains(pairID))
					continue;

				m_OverlappingPairs.erase(pairID);

				collider->OnEndOverlap.Broadcast(secondCollider);
				secondCollider->OnEndOverlap.Broadcast(collider);
			}
			else
			{
				if (!m_HittingPairs.contains(pairID))
					continue;

				m_HittingPairs.erase(pairID);
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

void CollisionSystem::ResolveCollision(ColliderComponent* colliderA, ColliderComponent* colliderB)
{
    if (colliderA->GetOwner().IsStatic() && colliderB->GetOwner().IsStatic()) 
		return;

	const glm::vec2 posA = colliderA->GetOwner().GetTransform()->GetWorldPosition();
    const glm::vec2 posB = colliderB->GetOwner().GetTransform()->GetWorldPosition();

    const auto boundsA = colliderA->GetBox().GetVertices();
    const auto boundsB = colliderB->GetBox().GetVertices();

    // Calculate overlap
    const float overlapX = std::min(boundsA[1].x, boundsB[1].x) - std::max(boundsA[0].x, boundsB[0].x);
    const float overlapY = std::min(boundsA[2].y, boundsB[2].y) - std::max(boundsA[0].y, boundsB[0].y);

    // Resolve along the axis of least penetration
    if (overlapX < overlapY)
    {
        const float correction = (posA.x < posB.x) ? -overlapX : overlapX;

    	colliderA->GetOwner().GetTransform()->SetPosition(posA.x + correction * 0.5f, posA.y);
    	colliderB->GetOwner().GetTransform()->SetPosition(posB.x - correction * 0.5f, posB.y);
    }
    else
    {
        const float correction = (posA.y < posB.y) ? -overlapY : overlapY;

    	colliderA->GetOwner().GetTransform()->SetPosition(posA.x, posA.y + correction * 0.5f);
    	colliderB->GetOwner().GetTransform()->SetPosition(posB.x, posB.y - correction * 0.5f);
    }	
}