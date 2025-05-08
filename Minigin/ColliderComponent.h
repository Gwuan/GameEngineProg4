#pragma once
#include "Component.h"
#include "Event.h"
#include "Observer.h"
#include <array>
#include <vec2.hpp>


class ColliderComponent final : public Component
{
public:
	// TODO: Transfer the following struct to datatypes utils file 
	struct Rect final
	{
		glm::vec2 center;
		float width;
		float height;

		std::array<glm::vec2, 4> GetBounds(const glm::vec2& goPos) const
		{
			std::array<glm::vec2, 4> result;

			const glm::vec2 worldCenter = goPos + center;

			result[0].x = worldCenter.x - (width / 2);
			result[0].y = worldCenter.y - (height / 2);

			result[1].x = worldCenter.x + (width / 2);
			result[1].y = worldCenter.y - (height / 2);

			result[2].x = worldCenter.x + (width / 2);
			result[2].y = worldCenter.y + (height / 2);

			result[3].x = worldCenter.x - (width / 2);
			result[3].y = worldCenter.y + (height / 2);

			return result;
		}
	};

	void BeginPlay() override {}
	void Update(const float) override {}
	void FixedUpdate(const float) override {}
	void LateUpdate(const float) override {}
	void Render() const override;
	virtual void DebugRender() override {}

	uint16_t GetID() const { return m_ID; }
	Rect GetBoundingBox() const { return m_BoundingBox; }

	Event<ColliderComponent*> OnBeginOverlap;
	Event<ColliderComponent*> OnEndOverlap;

	Event<ColliderComponent*> OnHit;

	virtual ~ColliderComponent() override;

	bool CheckCollision(ColliderComponent* other);

	bool IsTrigger() const { return m_IsTrigger; }
	bool IsStatic() const { return m_IsStatic; }

protected:
	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

	explicit ColliderComponent(dae::GameObject& owner, const Rect& box, bool isTrigger, bool isStatic);

private:
	static uint16_t m_Counter;

	const uint16_t m_ID;
	const Rect m_BoundingBox;
	const bool m_IsTrigger;
	const bool m_IsStatic;  // TODO: the GameObject should be marked as static, not a colliderComponent
};
