#pragma once
#include <Component.h>
#include "EnemyState.h"

#include "ColliderComponent.h"

class EnemyState;
struct EnemyType;

class EnemyComponent final : public Component, public Subject
{
public:
	void BeginPlay() override;
	void Update(const float deltaTime) override;
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {}

	~EnemyComponent() override;

	int GetLadderCount() const { return m_LadderCounter; }
	bool OnLadderEntry() const { return m_OnLadderEntry; }

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit EnemyComponent(dae::GameObject& owner, const EnemyType& type);

private:
	void OnBoxOverlap(const ColliderComponent* otherCollider);

	void LadderBeginOverlap(const ColliderComponent* otherCollider);
	void LadderEndOverlap(const ColliderComponent* otherCollider);


	ColliderComponent* m_pLadderCollider = nullptr;
	ColliderComponent* m_pCollider = nullptr;

	bool m_OnLadderEntry = false;
	int m_LadderCounter{};

	const EnemyType* const m_Type;

	std::unique_ptr<EnemyState> m_State;
};