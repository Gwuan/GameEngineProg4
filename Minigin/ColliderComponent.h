#pragma once
#include "Component.h"
#include "Event.h"
#include "Observer.h"
#include <array>
#include <vec2.hpp>

#include "Datatypes.hpp"


class ColliderComponent final : public Component
{
public:
	void BeginPlay() override {}
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override {};
	void DebugRender() override;

	uint16_t GetID() const { return m_ID; }
	Rectf GetBox() const;

	Event<ColliderComponent*> OnBeginOverlap;
	Event<ColliderComponent*> OnEndOverlap;

	Event<ColliderComponent*> OnHit;

	virtual ~ColliderComponent() override;

	bool CheckCollision(ColliderComponent* other);

	bool IsTrigger() const { return m_IsTrigger; }

	glm::vec2 m_Offset;

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit ColliderComponent(dae::GameObject& owner, const glm::vec2& boxSize, bool isTrigger);

private:
	static uint16_t m_Counter;
	glm::vec2 m_BoxSize;
	const uint16_t m_ID;
	const bool m_IsTrigger;
};
