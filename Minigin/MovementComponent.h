#pragma once
#include "Component.h"

enum class MoveDirection
{
	LEFT,
	RIGHT,
	DOWN,
	UP
};

class MovementComponent final : public Component
{
public:

	void FixedUpdate(const float fixedTimeStep) override;
	void Update(const float deltaTime) override;
	void LateUpdate(const float deltaTime) override;
	void Render() const override;

	void SetMovementSpeed(float speed);
	float GetMovementSpeed() const { return m_MovementSpeed; }

	void Move(MoveDirection desiredDirection);
	void Move(glm::vec2 direction);
	void Move(float x, float y);
protected:
	explicit MovementComponent(dae::GameObject& owner);

	template <typename  T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);
private:
	float m_MovementSpeed = 50.f;
	glm::vec2 m_MovementDirection = glm::vec2(0.f, 0.f);
	Transform* m_pOwnerTransform;

};

