#pragma once
#include "Component.h"
#include <glm.hpp>

enum class RotationDirection
{
	LEFT,
	RIGHT
};

class CircularMovement final : public Component
{
	void FixedUpdate(const float fixedTimeStep) override;
	void Update(const float deltaTime) override;
	void LateUpdate(const float deltaTime) override;
	void Render() const override;

protected:
	explicit CircularMovement(dae::GameObject& owner, RotationDirection desiredDirection = RotationDirection::LEFT);

	template <typename T, typename ... Args>
	friend T* dae::GameObject::AddComponent(Args&&... args);

private:
	const float m_Speed;
	const float m_Radius;
	float m_Angle;
	Transform* m_OwnerTransform;
	const glm::vec2 m_CenterPoint;
	const RotationDirection m_Direction;
};

