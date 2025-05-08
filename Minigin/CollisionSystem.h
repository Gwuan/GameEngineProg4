#pragma once
#include <unordered_set>
#include <vector>
#include "Singleton.h"

class ColliderComponent;

class CollisionSystem final : public dae::Singleton<CollisionSystem>
{
public:
	CollisionSystem() = default;
	~CollisionSystem() override;

	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem(CollisionSystem&&) noexcept = delete;
	CollisionSystem operator=(const CollisionSystem&) = delete;
	CollisionSystem operator=(CollisionSystem&&) noexcept = delete;

	void RegisterCollider(ColliderComponent& component);
	void UnregisterCollider(ColliderComponent& component);

	void Update();

private:
	static uint32_t MakePairID(uint16_t id1, uint16_t id2);

	static void ResolveCollision(ColliderComponent* colliderA, ColliderComponent* colliderB);


	std::vector<ColliderComponent*> m_pColliders;
	std::unordered_set<uint32_t> m_OverlappingPairs;
	std::unordered_set<uint32_t> m_HittingPairs;
};